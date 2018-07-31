#include "vulkan_setup.h"
#include "utilities.h"

#include <vector>

bool QueueFamilyIndices::isComplete()
{
  return graphics_family >= 0;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
{
  utilities::log("TRACE", "Finding vulkan queue families from device");
  QueueFamilyIndices indices;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

  for (uint32_t i=0; i<queue_family_count; ++i)
  {
    VkQueueFamilyProperties &queue_family = queue_families[i];
    if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphics_family = i;
    }

    if (indices.isComplete())
    {
      break;
    }
  }

  utilities::log("TRACE", "Found vulkan queue families");
  return indices;
}

