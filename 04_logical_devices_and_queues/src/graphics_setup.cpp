#include "graphics_setup.h"
#include "debug.h"

namespace Graphics
{
  void initGraphcis(Context::Graphics const &context)
  {
    Debug::Log("TRACE", "Init graphics");

    #ifdef USING_VULKAN
    namespace Vulkan
    {
      createInstance(context);
      #ifndef NDEBUG
        setupDebugCallbacks(context);
      #endif
      pickPhysicalDevice(context);
    }
    #endif

    Debug::Log("TRACE", "Graphics initialized");
  }

  void cleanup(Context::Graphics const &context)
  {
    Debug::Log("TRACE", "Clean up graphics");

    #ifdef USING_VULKAN
    namespace Vulkan
    {
      cleanup(context);
    }
    #endif

    Debug::Log("TRACE", "Graphics cleaned up");
  }

  namespace Vulkan
  {
    void createInstance(Context::Graphics const &context)
    {
      Debug::Log("TRACE", "Creating Vulkan instance");

      #ifndef NDEBUG
        std::string error_message;
        if (!checkValidationLayerSupport(context, error_message))
        {
          throw std::runtime_error("ERROR: " + error_message);
        }
        
        Debug::Log("TRACE", "All requested Vulkan validation layers available");
      #endif

      VkApplicationInfo app_info = {};
      app_info.sType = VK_STRUCTURE_TYPE_APP_INFO;
      app_info.pApplicationName = "Logical Devices and Queues";
      app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      app_info.pEngineName = "No Engine";
      app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      app_info.apiVersion = VK_API_VERSION_1_0;
      Debug::Log("TRACE", "Vulkan app info set");

      VkInstanceCreateInfo instance_create_info = {};
      instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      // The create info is only needed to create the instance, so making a reference to something on the stack
      // is fine since the create info is not stored after being used to create the instance
      instance_create_info.pApplicationInfo = &app_info; 
      std::vector<char const *> extensions = std::move(getRequiredExtensions(context));
      std::uint32_t extension_count = static_cast<std::uint32_t>(extensions.size());
      retrieveExtensionList(context);
      if (verifyExtensionList(context, extension_count, extensions.data()))
      {
        Debug::Log("TRACE", "All window extensions are supported by Vulkan");
      }
      create_info.enabledExtensionCount = extension_count;
      create_info.ppEnabledLayerNames = extensions.data();
      #ifndef NDEBUG
        create_info.enabledLayerCount = static_cast<std::uint32_t>(context.validation_layers.size());
        create_info.ppEnabledLayerNames = context.validation_layers.data();
      #else
        create_info.enabledLayerCount = 0;
      #endif

      VkResult result = vkCreateInstance(
        &create_info, 
        nullptr,  // Custom allocator callback
        &(context.instance)
      );

      if (result != VK_SUCCESS)
      {
        throw std::runtime_error("ERROR: Failed to create Vulkan instance");
      }

      Debug::Log("TRACE", "Vulkan instance created");
    }

    void cleanup(Context::Graphics const &context)
    {
      Debug::Log("TRACE", "Clean up Vulkan");

      #ifndef NDEBUG
        destroyDebugCallbacks(context);
      #endif

      // Destroy Vulkan instance after all other resources are cleaned up
      vkDestroyInstance(context.instance);
  
      Debug::Log("TRACE", "Vulkan cleaned up");
    }
  }
}
