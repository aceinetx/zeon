#pragma once
#include <vulkan/vulkan.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
#define VOLK_IMPLEMENTATION
#include <volk.h>
#endif

#ifdef _DEBUG
#define APP_USE_VULKAN_DEBUG_REPORT
VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
#endif

namespace z {
void check_vk_result(VkResult err);

class Zeon {
private:
  VkAllocationCallbacks *g_Allocator = nullptr;
  VkInstance g_Instance = VK_NULL_HANDLE;
  VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
  VkDevice g_Device = VK_NULL_HANDLE;
  uint32_t g_QueueFamily = (uint32_t)-1;
  VkQueue g_Queue = VK_NULL_HANDLE;
  VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
  VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
  ImGui_ImplVulkanH_Window g_MainWindowData;
  uint32_t g_MinImageCount = 2;
  bool g_SwapChainRebuild = false;
  SDL_Window *window;
  ImGui_ImplVulkanH_Window *wd;

private:
  bool IsExtensionAvailable(const ImVector<VkExtensionProperties> &properties,
                            const char *extension);
  void SetupVulkan(ImVector<const char *> instance_extensions);
  void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface,
                         int width, int height);
  void CleanupVulkan();
  void CleanupVulkanWindow();
  void FrameRender(ImGui_ImplVulkanH_Window *wd, ImDrawData *draw_data);
  void FramePresent(ImGui_ImplVulkanH_Window *wd);

  void InitAssets();
  void Draw();

public:
  Zeon();
  int Init();
  void Cleanup();
  void Run();
  ~Zeon();
};
} // namespace z
