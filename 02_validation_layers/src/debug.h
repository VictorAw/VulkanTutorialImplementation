#ifndef DEBUG_H
#define DEBUG_H

#include <vulkan/vulkan.h>

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugPrintCallback(
  VkDebugReportFlagsEXT flags,
  VkDebugReportObjectTypeEXT obj_type,
  uint64_t obj,
  size_t location,
  int32_t code,
  char const *layer_prefix,
  char const *msg,
  void *user_data
);

VkResult createVulkanDebugReportCallbackEXT(
  VkInstance instance,
  VkDebugReportCallbackCreateInfoEXT const *create_info,
  VkAllocationCallbacks const *allocator,
  VkDebugReportCallbackEXT *callback
);

void destroyVulkanDebugReportCallbackEXT(
  VkInstance instance,
  VkDebugReportCallbackEXT callback,
  VkAllocationCallbacks const *allocator
);


#endif // DEBUG_H

