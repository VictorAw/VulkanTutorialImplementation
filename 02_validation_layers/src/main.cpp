//#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <functional>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>

#include "debug.h"

int const WIDTH = 800;
int const HEIGHT = 600;

//======================
// Variables
//======================

struct Context
{
  struct GLFW
  {
    GLFWwindow* window;
  };

  struct Vulkan
  {
    VkInstance instance;
    std::vector<VkExtensionProperties> extensions;
    std::vector<char const *> validation_layers = {
      "VK_LAYER_LUNARG_standard_validation"
    };
    
    std::vector<VkDebugReportCallbackEXT> debug_report_callbacks;

    #ifdef NDEBUG
      bool const enable_validation_layers = false;
    #else
      bool const enable_validation_layers = true;
    #endif
  };

  GLFW glfw;
  Vulkan vulkan;
};

//======================
// Function Declarations
//======================

void init(Context &context);
void initWindow(Context::GLFW &context);
std::vector<char const *> getRequiredExtensions(Context::Vulkan const &context);
void initVulkan(Context::Vulkan &context);
void setupVulkanDebugCallbacks(Context::Vulkan &context);
void destroyVulkanDebugCallbacks(Context::Vulkan &context);
void createVulkanInstance(Context::Vulkan &context);
inline void retrieveVulkanExtensionList(Context::Vulkan &context);
inline bool verifyVulkanExtensionList(
  Context::Vulkan &context, 
  char const * const * const extensions,
  std::uint32_t extension_count
);
inline bool checkValidationLayerSupport(Context::Vulkan const &context, std::string &message);

void run(Context &context);

void cleanup(Context &context);
inline void cleanupVulkan(Context::Vulkan &context);
inline void cleanupWindow(Context::GLFW &context);

//======================
// Function Definitions
//======================

int main()
{
  Context context;

  #ifdef NDEBUG
    std::cout << "MODE: Release\n";
  #else
    std::cout << "MODE: Debug\n";
  #endif
  std::cout << '\n';

  try
  {
    init(context);

    std::cout << "DEBUG: Vulkan context extensions size: "
              << context.vulkan.extensions.size()
              << '\n';

    run(context);
    cleanup(context);
  }
  catch (std::runtime_error const& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void init(Context &context)
{
  std::cout << "DEBUG: Init\n";

  initWindow(context.glfw);
  initVulkan(context.vulkan);
}

void initWindow(Context::GLFW &context)
{
  std::cout << "DEBUG: Init Window\n";
  glfwInit();

  // Tell GLFW not to create an OpenGL context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  
  std::cout << "DEBUG: Window hint no OpenGL\n";

  // For now, we disable resizing windows since handling resizable windows
  // is more complicated
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  
  std::cout << "DEBUG: Window hint no resize\n";

  context.window = glfwCreateWindow(
    WIDTH,
    HEIGHT,
    "Triangle", // Title
    nullptr, // Monitor
    nullptr // OpenGL specific
  );
  
  std::cout << "DEBUG: Window created\n";
}

std::vector<char const *> getRequiredExtensions(Context::Vulkan const &context)
{
  std::cout << "DEBUG: Get required extensions\n";

  uint32_t glfw_extension_count = 0;
  char const * const *glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  // Range constructor
  std::vector<char const *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

  if (context.enable_validation_layers)
  {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME); 
  }
  
  std::cout << "DEBUG: Extension count: " << extensions.size() << '\n';

  return extensions;
}

void initVulkan(Context::Vulkan &context)
{
  std::cout << "DEBUG: Init Vulkan\n";

  createVulkanInstance(context);
  setupVulkanDebugCallbacks(context);
}

void setupVulkanDebugCallbacks(Context::Vulkan &context)
{
  if (!context.enable_validation_layers) { return; }

  std::cout << "DEBUG: Setup Vulkan debug callbacks\n";

  VkDebugReportCallbackCreateInfoEXT create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  create_info.pfnCallback = vulkanDebugPrintCallback;
  
  std::cout << "DEBUG: Vulkan debug create info set\n";

  VkDebugReportCallbackEXT print_callback;
  VkResult result = createVulkanDebugReportCallbackEXT(
    context.instance, &create_info, nullptr, &print_callback
  );
  context.debug_report_callbacks.push_back(print_callback);
  
  std::cout << "DEBUG: Setup Vulkan create debug report callback called\n";

  if (result != VK_SUCCESS)
  {
    throw std::runtime_error("ERROR: Failed to set up debug callbacks");
  }
  
  std::cout << "DEBUG: Setup Vulkan create debug report callback success\n";
}

void destroyVulkanDebugCallbacks(Context::Vulkan &context)
{
  for (VkDebugReportCallbackEXT callback : context.debug_report_callbacks)
  {
    destroyVulkanDebugReportCallbackEXT(
      context.instance, callback, nullptr
    );
  }

  context.debug_report_callbacks.clear();
}

void createVulkanInstance(Context::Vulkan &context)
{
  std::cout << "DEBUG: Create Vulkan instance\n";

  std::string error_message;
  if (context.enable_validation_layers)
  {
    if (!checkValidationLayerSupport(context, error_message))
    {
      throw std::runtime_error("ERROR: " + error_message);
    }
    
    std::cout << "DEBUG: All requested validation layers available\n";
  }

  VkApplicationInfo app_info;
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Triangle";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  std::cout << "DEBUG: Vulkan instance app info set\n";

  VkInstanceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
 
  std::vector<char const *> extensions = std::move(getRequiredExtensions(context)); 

  uint32_t extension_count = static_cast<uint32_t>(extensions.size());
  create_info.enabledExtensionCount = extension_count;
  create_info.ppEnabledExtensionNames = extensions.data();
  if (context.enable_validation_layers)
  {
    create_info.enabledLayerCount = static_cast<uint32_t>(context.validation_layers.size());
    create_info.ppEnabledLayerNames = context.validation_layers.data(); 
  }
  else
  {
    create_info.enabledLayerCount = 0;
  }

  std::cout << "DEBUG: Vulkan instance create info set\n";

  VkResult result = vkCreateInstance(
    &create_info, 
    nullptr, // Custom allocator callback
    &(context.instance)
  );
  
  std::cout << "DEBUG: vkCreateInstance called\n";

  if (result != VK_SUCCESS)
  {
    std::cout << "DEBUG: Create Vulkan instance error\n";

    throw std::runtime_error("ERROR: Failed to create Vulkan instance!");
  }
  
  std::cout << "DEBUG: vkCreateInstance success\n";

  retrieveVulkanExtensionList(context);  
  if (verifyVulkanExtensionList(context, extensions.data(), extension_count))
  {
    std::cout << "DEBUG: All glfw extensions are supported by Vulkan\n";
  }

  std::cout << "DEBUG: Create Vulkan instance success\n";
}

inline void retrieveVulkanExtensionList(Context::Vulkan &context)
{
  std::cout << "DEBUG: Retrieving Vulkan extension list\n";
  std::uint32_t extension_count = 0;
  vkEnumerateInstanceExtensionProperties(
    nullptr, &extension_count, nullptr
  );

  auto &extensions = context.extensions;
  extensions.resize(extension_count);
  vkEnumerateInstanceExtensionProperties(
    nullptr, &extension_count, extensions.data()
  );

  std::cout << "DEBUG: Available extensions:\n";
  for (auto const &extension : extensions)
  {
    std::cout << "  " << extension.extensionName << '\n';
  }
  if (extensions.size() == 0)
  {
    std::cout << '\n';
  }
}

inline bool verifyVulkanExtensionList(
  Context::Vulkan &context, 
  char const * const * const extensions,
  std::uint32_t extension_count)
{
  bool all_extensions_supported = true;

  auto const & supported_extensions = context.extensions;
  size_t supported_extension_count = supported_extensions.size();

  // Naive O(n*m*o) algorithm where 
  // n is the number of requested extensions,
  // m is the number of supported extensions, and
  // o is the length of the shorter of the two strings being compared
  // to check if all requested extensions are within supported extensions
  for (std::uint32_t i=0; i<extension_count; ++i)
  {
    bool found = false;
    for (size_t j=0; j<supported_extension_count; ++j)
    {
      if (strcmp(extensions[i], supported_extensions[j].extensionName) == 0)
      {
        found = true;
        break;
      }
    }

    if (!found)
    {
      std::cout << "Extension '" << extensions[i] << "' not supported!\n";
      all_extensions_supported = false;
    }
    found = true;
  }

  return all_extensions_supported;
}

inline bool checkValidationLayerSupport(Context::Vulkan const &context, std::string &message)
{
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  std::vector<char const *> const &validation_layers = context.validation_layers;
  for (char const *layer_name : validation_layers)
  {
    bool layer_found = false;
    for (auto const &layer_properties : available_layers)
    {
      if (strcmp(layer_name, layer_properties.layerName) == 0)
      {
        layer_found = true;
        break;
      }
    }

    if (!layer_found)
    {
      message = std::string(layer_name) + " validation layer requested, but not available";
      return false;
    }
  }

  return true;
}

void run(Context &context)
{
  std::cout << "DEBUG: Run\n";
  
  while (!glfwWindowShouldClose(context.glfw.window))
  {
    glfwPollEvents();
  }

  std::cout << "DEBUG: Run loop end\n";
}

void cleanup(Context &context)
{
  std::cout << "DEBUG: Cleanup\n";

  cleanupVulkan(context.vulkan);
  cleanupWindow(context.glfw);
}

inline void cleanupVulkan(Context::Vulkan & context)
{
  std::cout << "DEBUG: Cleanup vulkan\n";

  // Clean up Vulkan resources here
  if (context.enable_validation_layers)
  {
    destroyVulkanDebugCallbacks(context);
  }

  // Destroy the Vulkan instance after all other resources are cleaned up 
  vkDestroyInstance(
    context.instance, 
    nullptr // Allocator callback
  );
}

inline void cleanupWindow(Context::GLFW & context)
{
  std::cout << "DEBUG: cleanup window\n";

  glfwDestroyWindow(context.window);
  glfwTerminate();
}

