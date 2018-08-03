#ifndef CONTEXT_H
#define CONTEXT_H

#include <vector>

struct Context
{
  struct Window
  {
    #ifdef USING_GLFW
      GLFWwindow *window;
    #endif
  };

  struct Graphics
  {
    #ifdef USING_VULKAN
      VkInstance instance;
      VkPhysicalDevice physical_device = VK_NULL_HANDLE; // For now we only pick one physical device
      std::vector<VkExtensionProperties> extensions;

      #ifndef NDEBUG
        std::vector<char const *> validation_layers = {
          "VK_LAYER_LUNARG_standard_validation"
        };
        std::vector<VkDebugReportCallbackEXT> debug_report_callbacks;
      #endif
    #endif
  };

  Window window;
  Graphics graphics;

};

#endif // CONTEXT_H
