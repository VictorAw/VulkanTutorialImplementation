#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

struct Context
{
  GLFWwindow* window;
};

void init(Context& context);
void cleanup(Context& context);

int main()
{
  Context context;
  init(context);

  glm::mat4 matrix;
  glm::vec4 vec;
  glm::vec4 test = matrix * vec;

  while (!glfwWindowShouldClose(context.window))
  {
    glfwPollEvents();
  }

  cleanup(context);

  return 0;
}

void init(Context& context)
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  context.window = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
  
  std::uint32_t extension_count = 0;
  vkEnumerateInstanceExtensionProperties(
    nullptr, 
    &extension_count, 
    nullptr
  );
  std::cout << extension_count << " extensions supported\n";
}

void cleanup(Context& context)
{
  glfwDestroyWindow(context.window);
  context.window = nullptr;

  glfwTerminate();
}


