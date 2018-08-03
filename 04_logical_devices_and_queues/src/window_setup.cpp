#include "window_setup.h"

#include "init.h"
#include "constants.h"
#include "debug.h"

void initWindow(Context::Window const &context)
{
  Debug::Log("TRACE", "Init window");

  glfwInit();

  #ifdef USING_VULKAN
    // Tell GLFW not to create on OpenGL Context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // For now, we disable window resizing since it will become complicated
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  #endif

  context.window = glfwCreateWindow(
    Constants::WIDTH, Constants::HEIGHT,
    "Logical Devices and Queues",
    nullptr, // Monitor
    nullptr  // OpenGL specific
  );

  Debug::Log("TRACE", "Window created");
}
