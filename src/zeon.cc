#include "zeon.hh"
#include <SDL3/SDL.h>
#include <cef_app.h>
#include <iostream>
#include <stdio.h>

void z::check_vk_result(VkResult err) {
	if (err == VK_SUCCESS)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}

z::Zeon::Zeon() = default;
z::Zeon::~Zeon() = default;

int z::Zeon::Zeon::Init() {
	// Setup SDL
	// [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop starts would likely be
	// your SDL_AppInit() function]
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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();

	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(
			main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling,
									 // changing this requires resetting Style + calling this again)
	style.FontScaleDpi =
			main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this
									// unnecessary. We leave both here for documentation purpose)

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple
	// fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the
	// font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors
	// in your application (e.g. use an assertion, or display an error and quit).
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality
	// font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to
	// write a double backslash \\ !
	// - Our Emscripten build process allows embedding fonts to be accessible at runtime from the
	// "fonts/" folder. See Makefile.emscripten for details.
	// style.FontSizeBase = 20.0f;
	// io.Fonts->AddFontDefault();
	// io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
	// ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
	// IM_ASSERT(font != nullptr);

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	InitAssets();
	{
		int r = InitCef();
		if (r != 0)
			return r;
	}

	return 0;
}

void z::Zeon::InitAssets() {
	auto& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("assets/SFMonoRegular.otf");
	IM_ASSERT(font != nullptr);
}

int z::Zeon::InitCef() {
	CefMainArgs args;

	{
		int result = CefExecuteProcess(args, nullptr, nullptr);
		if (result >= 0) {
			return result;
		} else if (result == -1) {
		}
	}

	{
		CefSettings settings;

		{
			std::ostringstream ss;
			ss << SDL_GetBasePath() << "locales/";
			std::cout << ss.str() << std::endl;
			CefString(&settings.locales_dir_path) = ss.str();
		}

		CefString(&settings.resources_dir_path) = SDL_GetBasePath();

		bool result = CefInitialize(args, settings, nullptr, nullptr);
		if (!result) {
			return -1;
		}
	}
	return 0;
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
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	auto& io = ImGui::GetIO();

	// Main loop
	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
					event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		// [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppIterate()
		// function]
		if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
			SDL_Delay(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		Draw();

		// Rendering
		ImGui::Render();
		SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z,
																clear_color.w);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_RenderPresent(renderer);
	}
}
