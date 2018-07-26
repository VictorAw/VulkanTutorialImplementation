#include "debug.h"
#include "utilities.h"

#ifndef NDEBUG

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugPrintCallback(
  VkDebugReportFlagsEXT flags,
  VkDebugReportObjectTypeEXT obj_type,
  std::uint64_t obj,
  size_t location,
  std::int32_t code,
  char const *layer_prefix,
  char const *msg,
  void *user_data)
{
  std::cerr << "DEBUG: validation layer " << layer_prefix << ":\n" 
            << "  Error code: " << code << '\n'
            << "  Message: " << msg << std::endl;

  return VK_FALSE;
}

VkResult createVulkanDebugReportCallbackEXT(
  VkInstance instance,
  VkDebugReportCallbackCreateInfoEXT const *create_info,
  VkAllocationCallbacks const *allocator,
  VkDebugReportCallbackEXT *callback)
{
  PFN_vkCreateDebugReportCallbackEXT func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(
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
  PFN_vkDestroyDebugReportCallbackEXT func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(
    instance, "vkDestroyDebugReportCallbackEXT"
  );

  if (func != nullptr)
  {
    func(instance, callback, allocator);
  }
}

void setupVulkanDebugCallbacks(Context::Vulkan &context)
{
  utilities::log("TRACE", "Setup Vulkan debug callbacks");

  VkDebugReportCallbackCreateInfoEXT create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  create_info.pfnCallback = vulkanDebugPrintCallback;

  utilities::log("TRACE", "Vulkan debug create info set");

  VkDebugReportCallbackEXT print_callback;
  VkResult result = createVulkanDebugReportCallbackEXT(
    context.instance, &create_info, nullptr, &print_callback
  );
  context.debug_report_callbacks.push_back(print_callback);

  if (result != VK_SUCCESS)
  {
    throw std::runtime_error("ERROR: Failed to set up debug callbacks");
  }

  utilities::log("TRACE", "Setup Vulkan create debug report callbacks success");
}

void destroyVulkanDebugCallbacks(Context::Vulkan &context)
{
  utilities::log("TRACE", "Destroy Vulkan debug callbacks");

  for (VkDebugReportCallbackEXT &callback : context.debug_report_callbacks)
  {
    destroyVulkanDebugReportCallbackEXT(context.instance, callback, nullptr);
  }

  context.debug_report_callbacks.clear();
}

#endif
