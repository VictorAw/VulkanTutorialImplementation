#ifndef VULKAN_SETUP_H
#define VULKAN_SETUP_H

#include <vulkan/vulkan.h>

struct QueueFamilyIndices
{
  int graphics_family = -1;

  bool isComplete();
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

#endif // VULKAN_SETUP_H
