#ifndef INIT_H
#define INIT_H

//#define USING_OPENGL
#define USING_VULKAN

#define USING_GLFW

#define USING_GLM

#ifdef USING_GFLW
  #define GLFW_INCLUDE_VULKAN
  #include <GLFW/glfw3.h>
#endif

#ifdef USING_GLM
  #define GLM_FORCE_RADIANS
  #define GLM_FORCE_DEPTH_ZERO_TO_ONE
  #include <glm/vec4.hpp>
  #include <glm/mat4x4.hpp>
#endif

#endif // INIT_H
