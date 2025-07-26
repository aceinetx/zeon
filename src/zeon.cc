#include "zeon.hh"
#include "sdl_cef_events.hh"
#include "zeon_kb.hh"
#include "zeondefs.hh"
#include <SDL3/SDL.h>
#include <cef_app.h>
#include <cef_parser.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>

std::string z::Zeon::encodeUrlIntoGetParameter(const std::string& value) {
	return CefURIEncode(CefString(value), true);
}

z::Zeon::Zeon() {};

z::Zeon::~Zeon() = default;

int z::Zeon::Zeon::Init() {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return -1;
	}

	// Create window with SDL_Renderer graphics context
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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

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

	std::cout << "DEVELOPER NOTE: z::Zeon::Init() assumes CefExecuteProcess is already called\n";
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
	settings.windowless_rendering_enabled = true;

	bool result = CefInitialize(cef_args, settings, nullptr, nullptr);
	if (!result) {
		return -1;
	}

	{
		renderHandler = new RenderHandler(renderer, 1, 1);
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		renderHandler->resize(w, h - ZEON_TOPBAR_HEIGHT);
	}

	{
		CefWindowInfo window_info;
		CefBrowserSettings browserSettings;
		window_info.SetAsWindowless(kNullWindowHandle);
		window_info.windowless_rendering_enabled = true;
		browserSettings.background_color = 0xff; // allows for transparency
		browserSettings.windowless_frame_rate = 60;

		browserClient = new BrowserClient(renderHandler);

		browser =
				CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "", browserSettings,
																					nullptr, CefRequestContext::GetGlobalContext());
	}
	return 0;
}

void z::Zeon::InitAssets() {
	auto& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("assets/font.otf");
	IM_ASSERT(font != nullptr);
}

void z::Zeon::Cleanup() {
	browser = nullptr;
	browserClient = nullptr;
	renderHandler = nullptr;

	CefShutdown();

	SDL_DestroyWindow(window);
	SDL_Quit();
}

void z::Zeon::Run() {
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	auto& io = ImGui::GetIO();

	// Main loop
	bool done = false;
	while (!browserClient->closeAllowed() && !done) {
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

		// Renderin
		ImGui::Render();
		SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z,
																clear_color.w);
		CefDoMessageLoopWork();
		SDL_RenderClear(renderer);
		renderHandler->render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_RenderPresent(renderer);
	}
}
