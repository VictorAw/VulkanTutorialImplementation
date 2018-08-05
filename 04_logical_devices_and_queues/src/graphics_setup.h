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
      std::uint32_t extension_count,
      char const * const * const extensions
    );
    bool checkValidationLayerSupport(Context::Graphics const &context, std::string &message);
    void pickPhysicalDevice(Context::Graphics &context);
    void ratePhysicalDeviceSuitability(VkPhysicalDevice device);
    #ifndef NDEBUG
      void setupDebugCallbacks(Context::Graphics const &context);
      void destroyDebugCallbacks(Context::Graphics const &context);
    #endif
    void cleanup(Context::Graphics const &context);
  }
#endif

}

#endif // GRAPHICS_SETUP_H
