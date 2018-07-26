#ifndef DEBUG_H
#define DEBUG_H

#include "context.h"

#ifndef NDEBUG
  void setupVulkanDebugCallbacks(Context::Vulkan &context);
  void destroyVulkanDebugCallbacks(Context::Vulkan &context);
#endif

#endif // DEBUG_H
