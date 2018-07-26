#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <string>
#include <set>

#include "utilities.h"
#include "context.h"

#ifndef NDEBUG
  #include "debug.h"
#endif

int const WIDTH = 800;
int const HEIGHT = 600;

//======================
// Function Declarations
//======================

// Initialization
void init(Context &context);
void initWindow(Context::GLFW &context);
std::vector<char const *> getRequiredExtensions(Context::Vulkan const &context);
void initVulkan(Context::Vulkan &context);
void createVulkanInstance(Context::Vulkan &context);
void retrieveVulkanExtensionList(Context::Vulkan &context);
bool verifyVulkanExtensionList(
  Context::Vulkan &context, 
  char const * const * const extenions, 
  std::uint32_t extension_count
);
inline bool checkValidationLayerSupport(Context::Vulkan const &context, std::string &message);

// Runtime
void run(Context &context);

// Cleanup
void cleanup(Context &context);
void cleanupVulkan(Context::Vulkan &context);
void cleanupWindow(Context::GLFW &context);

//=====================
// Function Definitions
//=====================

int main()
{
  Context context;

  #ifndef NDEBUG
    std::cout << "MODE: Debug\n\n";
  #else
    std::cout << "MODE: Release\n\n";
  #endif

  try
  {
    init(context);

    run(context);
    cleanup(context);
  }
  catch(std::runtime_error const &e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void init(Context &context)
{
  utilities::log("TRACE", "Init");

  initWindow(context.glfw);
  initVulkan(context.vulkan);
}

void initWindow(Context::GLFW &context)
{
  utilities::log("TRACE", "Init window");

  glfwInit();

  // Tell GLFW not to create an OpenGL context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  // For now, we disable window resizing since it will become complicated
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  utilities::log("TRACE", "Window hints set");

  context.window = glfwCreateWindow(WIDTH, HEIGHT, "Physical Device", nullptr, nullptr);

  utilities::log("TRACE", "Window created");
}

std::vector<char const *> getRequiredExtensions(Context::Vulkan const &context)
{
  utilities::log("TRACE", "Get required extensions");

  uint32_t glfw_extension_count = 0;
  char const * const *glfw_extensions;

  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  // Range constructor
  std::vector<char const *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

  #ifndef NDEBUG
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  #endif

  utilities::log("TRACE", "Extension count: " + std::to_string(extensions.size()));

  return extensions;
}

void initVulkan(Context::Vulkan &context)
{
  utilities::log("TRACE", "Init Vulkan");

  createVulkanInstance(context);
  #ifndef NDEBUG
    setupVulkanDebugCallbacks(context);
  #endif
}

void createVulkanInstance(Context::Vulkan &context)
{
  utilities::log("TRACE", "Create Vulkan instance");

  std::string error_message;
  #ifndef NDEBUG
    if (!checkValidationLayerSupport(context, error_message))
    {
      throw std::runtime_error("ERROR: " + error_message);
    }

    utilities::log("TRACE", "All requested validation layers available");
  #endif

  VkApplicationInfo app_info;
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Physical Devices";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  utilities::log("TRACE", "Vulkan instance app info set");

  VkInstanceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  std::vector<char const *> extensions = std::move(getRequiredExtensions(context));
  std::uint32_t extension_count = static_cast<std::uint32_t>(extensions.size());
  retrieveVulkanExtensionList(context);
  if (verifyVulkanExtensionList(context, extensions.data(), extension_count))
  {
    utilities::log("TRACE", "All glfw extensions are supported by Vulkan");
  }
  create_info.enabledExtensionCount = extension_count;
  create_info.ppEnabledExtensionNames = extensions.data();
  #ifndef NDEBUG
    create_info.enabledLayerCount = static_cast<std::uint32_t>(context.validation_layers.size());
    create_info.ppEnabledLayerNames = context.validation_layers.data();
  #else
    create_info.enabledLayerCount = 0;
  #endif

  utilities::log("TRACE", "Vulkan instance create info set");

  VkResult result = vkCreateInstance(&create_info, nullptr, &(context.instance));

  if (result != VK_SUCCESS)
  {
    throw std::runtime_error("ERROR: Failed to create Vulkan instance!");
  }

  utilities::log("TRACE", "Create Vulkan instance success");
}

inline void retrieveVulkanExtensionList(Context::Vulkan &context)
{
  utilities::log("TRACE", "Retrieving Vulkan extension list");
  std::uint32_t extension_count = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

  std::vector<VkExtensionProperties> &extensions = context.extensions;
  extensions.resize(extension_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

  utilities::log("TRACE", "Available extensions:");
  for (VkExtensionProperties const &extension : extensions)
  {
    utilities::log(std::string("  ") + extension.extensionName);
  }
  if (extensions.size() == 0)
  {
    utilities::log();
  }
}

inline bool verifyVulkanExtensionList(
  Context::Vulkan &context,
  char const * const * const extensions,
  std::uint32_t extension_count)
{
  bool all_extensions_supported = true;

  std::vector<VkExtensionProperties> &supported_extensions = context.extensions;
  size_t supported_extension_count = supported_extensions.size();

  std::set<size_t> seen_supported_extensions;

  for (std::uint32_t i=0; i<extension_count; ++i)
  {
    bool found = false;

    for (size_t j=0; j<supported_extension_count; ++j)
    {
      if (seen_supported_extensions.find(j) != seen_supported_extensions.end())
      {
        continue;
      }

      if (strcmp(extensions[i], supported_extensions[j].extensionName) == 0)
      {
        found = true;
        seen_supported_extensions.insert(j);
        break;
      }
    }

    if (!found)
    {
      utilities::log("TRACE", "Extension '" + std::string(extensions[i]) + "' not supported!");
      all_extensions_supported = false;
    }
  }

  return all_extensions_supported;
}

inline bool checkValidationLayerSupport(Context::Vulkan const &context, std::string &message)
{
  std::uint32_t available_layer_count;
  vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(available_layer_count);
  vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers.data());

  std::vector<char const *> const &validation_layers = context.validation_layers;

  std::set<size_t> seen_available_layers;
  for (char const *layer_name : validation_layers)
  {
    bool found = false;
    for (size_t j=0; j<available_layer_count; ++j)
    {
      if (seen_available_layers.find(j) != seen_available_layers.end())
      {
        continue;
      }

      if (strcmp(layer_name, available_layers[j].layerName) == 0)
      {
        found = true;
        seen_available_layers.insert(j);
        break;
      }
    }

    if (!found)
    {
      message = std::string(layer_name) + " validation layer requested, but not available";
      return false;
    }
  }

  return true;
}

void run(Context &context)
{
  utilities::log("TRACE", "Run");

  while (!glfwWindowShouldClose(context.glfw.window))
  {
    glfwPollEvents();
  }

  utilities::log("TRACE", "Run loop end");
}

void cleanup(Context &context)
{
  utilities::log("TRACE", "Cleanup");

  cleanupVulkan(context.vulkan);
  cleanupWindow(context.glfw);
}

void cleanupVulkan(Context::Vulkan &context)
{
  utilities::log("TRACE", "Cleanup Vulkan");

  #ifndef NDEBUG
    destroyVulkanDebugCallbacks(context);
  #endif

  // Destroy the Vulkan instance after all other resources are cleaned up
  vkDestroyInstance(context.instance, nullptr);
}

void cleanupWindow(Context::GLFW &context)
{
  utilities::log("TRACE", "Cleanup window");

  glfwDestroyWindow(context.window);
  glfwTerminate();
}

