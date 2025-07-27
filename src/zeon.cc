#include "zeon.hh"
#include "zeondefs.hh"
#include <SDL3/SDL.h>
#include <cef_app.h>
#include <cef_parser.h>
#include <iostream>
#include <stdio.h>

using namespace z;

std::string Zeon::encodeUrlIntoGetParameter(const std::string& value) {
	return CefURIEncode(CefString(value), true);
}

Zeon::Zeon() : settingsManager(this) {
	settingsManager.load();
};

Zeon::~Zeon() {
	settingsManager.save();
};

int Zeon::OpenTab(const std::string& url) {
	auto handler = new RenderHandler(renderer, 1, 1);
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	handler->resize(w, h - ZEON_TOPBAR_HEIGHT);
	renderHandlers.push_back(handler);
	auto client = new BrowserClient(handler);
	browserClients.push_back(client);
	CefWindowInfo window_info;
	CefBrowserSettings browserSettings;
	window_info.SetAsWindowless(kNullWindowHandle);
	window_info.windowless_rendering_enabled = true;
	browserSettings.background_color = 0xff;		// solid color
	browserSettings.windowless_frame_rate = 60; // smooooooth
	auto browser = CefBrowserHost::CreateBrowserSync(window_info, client, url, browserSettings,
																									 nullptr, CefRequestContext::GetGlobalContext());
	browsers.push_back(browser);
	active_tab = browsers.size() - 1;
	return active_tab;
}

void Zeon::CloseTab(int idx) {
	if (idx < 0 || idx >= (int)browsers.size())
		return;
	browsers[idx]->GetHost()->CloseBrowser(true);
	browsers.erase(browsers.begin() + idx);
	browserClients.erase(browserClients.begin() + idx);
	renderHandlers.erase(renderHandlers.begin() + idx);
	if (active_tab >= (int)browsers.size())
		active_tab = browsers.size() - 1;
	if (active_tab < 0 && !browsers.empty())
		active_tab = 0;
}

void Zeon::SwitchTab(int idx) {
	if (idx < 0 || idx >= (int)browsers.size())
		return;
	active_tab = idx;
}

int Zeon::Init() {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return -1;
	}

	// create window with SDL_Renderer graphics context
	float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	SDL_WindowFlags window_flags =
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	window = SDL_CreateWindow("Dear ImGui SDL3+SDL_Renderer example", (int)(1280 * main_scale),
														(int)(720 * main_scale), window_flags);
	if (window == nullptr) {
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}
	renderer = SDL_CreateRenderer(window, nullptr);
	SDL_SetRenderVSync(renderer, 1);
	if (renderer == nullptr) {
		SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
		return -1;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// enable keyboard
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);

	style.FontScaleDpi = main_scale;
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	InitAssets();
	SetupStyle();

	std::cout << "DEVELOPER NOTE: Zeon::Init() assumes CefExecuteProcess is already called\n";

	CefSettings settings;
	{
		std::ostringstream ss;
		ss << SDL_GetBasePath() << "locales/";
		std::cout << ss.str() << std::endl;
		CefString(&settings.locales_dir_path) = ss.str();
	}

	CefString(&settings.resources_dir_path) = SDL_GetBasePath();
	CefString(&settings.root_cache_path) = SDL_GetBasePath();
	settings.no_sandbox = true;
	settings.windowless_rendering_enabled = true; // osr

	bool result = CefInitialize(cef_args, settings, nullptr, nullptr);
	if (!result) {
		return -1;
	}

	OpenTab(searchEngines[currentSearchEngine].defaultUrl);
	return 0;
}

void Zeon::InitAssets() {
	auto& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("assets/font.otf");
	IM_ASSERT(font != nullptr);
}

void Zeon::Cleanup() {
	browsers.clear();
	browserClients.clear();
	renderHandlers.clear();
	CefShutdown();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Zeon::Run() {
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	auto& io = ImGui::GetIO();

	// Main loop
	done = false;
	while (!browserClients.empty() && !browserClients[active_tab]->closeAllowed() && !done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ProcessEvent(event);
		}

		if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
			SDL_Delay(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		Draw();

		// Renderin'
		ImGui::Render();
		SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z,
																clear_color.w);

		CefDoMessageLoopWork();

		SDL_RenderClear(renderer);

		// ordering makes imgui draw on top of the browser
		renderHandlers[active_tab]->render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

		SDL_RenderPresent(renderer);
	}
}
