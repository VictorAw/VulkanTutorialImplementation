#include "debug.h"

#include <iostream>

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugPrintCallback(
  VkDebugReportFlagsEXT flags,
  VkDebugReportObjectTypeEXT obj_type,
  uint64_t obj,
  size_t location,
  int32_t code,
  char const *layer_prefix,
  char const *msg,
  void *user_data)
{
  std::cerr << "DEBUG: validation layer: " << msg << std::endl;

  return VK_FALSE;
}

VkResult createVulkanDebugReportCallbackEXT(
  VkInstance instance,
  VkDebugReportCallbackCreateInfoEXT const *create_info,
  VkAllocationCallbacks const *allocator,
  VkDebugReportCallbackEXT *callback)
{
  auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(
    instance, "vkCreateDebugReportCallbackEXT"
  );

  if (func != nullptr)
  {
    return func(instance, create_info, allocator, callback);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void destroyVulkanDebugReportCallbackEXT(
  VkInstance instance,
  VkDebugReportCallbackEXT callback,
  VkAllocationCallbacks const *allocator)
{
  auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(
    instance, "vkDestroyDebugReportCallbackEXT"
  );

  if (func != nullptr)
  {
    func(instance, callback, allocator);
  }
}

