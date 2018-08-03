#ifndef GRAPHICS_SETUP_H
#define GRAPHICS_SETUP_H

#include "context.h"

#include <vector>

namespace Graphics
{

void initGraphics(Context::Graphics const &context);
std::vector<char const *> getRequiredExtensions(Context::Graphics const &context);
void cleanup(Context::Graphics const &context);

#ifdef USING_VULKAN
  namespace Vulkan
  {
    void createInstance(Context::Graphics const &context);
    void retrieveExtensionList(Context::Graphics &context);
    bool verifyExtensionList(
      Context::Graphics const &context,
      char const * const * const extensions,
      std::uint32_t extension_count
    );
    bool checkValidationLayerSupport(Context::Graphics const &context, std::string &message);
    void pickPhysicalDevice(Context::Graphics &context);
    void ratePhysicalDeviceSuitability(VkPhysicalDevice device);
    void cleanup(Context::Graphics const &context);
  }
#endif

}

#endif // GRAPHICS_SETUP_H
