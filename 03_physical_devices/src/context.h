#ifndef CONTEXT_H
#define CONTEXT_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

struct Context
{
  struct GLFW
  {
    GLFWwindow *window;
  } glfw;

  struct Vulkan
  {
    VkInstance instance;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    std::vector<VkExtensionProperties> extensions;

    #ifndef NDEBUG
      std::vector<char const *> validation_layers = {
        "VK_LAYER_LUNARG_standard_validation"
      };
      std::vector<VkDebugReportCallbackEXT> debug_report_callbacks;
    #endif
  } vulkan;
};

#endif // CONTEXT_H
