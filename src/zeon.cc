#include "zeon.hh"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
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
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
    printf("Error: SDL_Init(): %s\n", SDL_GetError());
    return -1;
  }

  // Create window with Vulkan graphics context
  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_WindowFlags window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
                                 SDL_WINDOW_HIDDEN |
                                 SDL_WINDOW_HIGH_PIXEL_DENSITY;
  window = SDL_CreateWindow("Dear ImGui SDL3+Vulkan example",
                            (int)(1280 * main_scale), (int)(720 * main_scale),
                            window_flags);
  if (window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return -1;
  }

  ImVector<const char *> extensions;
  {
    uint32_t sdl_extensions_count = 0;
    const char *const *sdl_extensions =
        SDL_Vulkan_GetInstanceExtensions(&sdl_extensions_count);
    for (uint32_t n = 0; n < sdl_extensions_count; n++)
      extensions.push_back(sdl_extensions[n]);
  }
  SetupVulkan(extensions);

  // Create Window Surface
  VkSurfaceKHR surface;
  VkResult err;
  if (SDL_Vulkan_CreateSurface(window, g_Instance, g_Allocator, &surface) ==
      0) {
    printf("Failed to create Vulkan surface.\n");
    return 1;
  }

  // Create Framebuffers
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  wd = &g_MainWindowData;
  SetupVulkanWindow(wd, surface, w, h);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_ShowWindow(window);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup scaling
  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(
      main_scale); // Bake a fixed style scale. (until we have a solution for
                   // dynamic style scaling, changing this requires resetting
                   // Style + calling this again)
  style.FontScaleDpi =
      main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true
                  // makes this unnecessary. We leave both here for
                  // documentation purpose)

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForVulkan(window);
  ImGui_ImplVulkan_InitInfo init_info = {};
  // init_info.ApiVersion = VK_API_VERSION_1_3;              // Pass in your
  // value of VkApplicationInfo::apiVersion, otherwise will default to header
  // version.
  init_info.Instance = g_Instance;
  init_info.PhysicalDevice = g_PhysicalDevice;
  init_info.Device = g_Device;
  init_info.QueueFamily = g_QueueFamily;
  init_info.Queue = g_Queue;
  init_info.PipelineCache = g_PipelineCache;
  init_info.DescriptorPool = g_DescriptorPool;
  init_info.RenderPass = wd->RenderPass;
  init_info.Subpass = 0;
  init_info.MinImageCount = g_MinImageCount;
  init_info.ImageCount = wd->ImageCount;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator = g_Allocator;
  init_info.CheckVkResultFn = check_vk_result;
  ImGui_ImplVulkan_Init(&init_info);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return a nullptr. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
  // for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // style.FontSizeBase = 20.0f;
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
  // IM_ASSERT(font != nullptr);
  InitAssets();

  io.IniFilename = nullptr;
  io.LogFilename = nullptr;

  return 0;
}

void z::Zeon::InitAssets() {
  auto &io = ImGui::GetIO();
  ImFont *font = io.Fonts->AddFontFromFileTTF("assets/SFMonoRegular.otf");
  IM_ASSERT(font != nullptr);
}

void z::Zeon::Cleanup() {
  VkResult err;
  err = vkDeviceWaitIdle(g_Device);
  check_vk_result(err);
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  CleanupVulkanWindow();
  CleanupVulkan();

  SDL_DestroyWindow(window);
  SDL_Quit();
}

void z::Zeon::Run() {
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10);
      continue;
    }

    int fb_width, fb_height;
    SDL_GetWindowSize(window, &fb_width, &fb_height);
    if (fb_width > 0 && fb_height > 0 &&
        (g_SwapChainRebuild || g_MainWindowData.Width != fb_width ||
         g_MainWindowData.Height != fb_height)) {
      ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
      ImGui_ImplVulkanH_CreateOrResizeWindow(
          g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData,
          g_QueueFamily, g_Allocator, fb_width, fb_height, g_MinImageCount);
      g_MainWindowData.FrameIndex = 0;
      g_SwapChainRebuild = false;
    }

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    Draw();

    // Rendering
    ImGui::Render();
    ImDrawData *draw_data = ImGui::GetDrawData();
    const bool is_minimized =
        (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    if (!is_minimized) {
      wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
      wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
      wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
      wd->ClearValue.color.float32[3] = clear_color.w;
      FrameRender(wd, draw_data);
      FramePresent(wd);
    }
  }
}
