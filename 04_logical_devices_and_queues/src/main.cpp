#include "context.h"
#include "window_setup.h"
#include "graphics_setup.h"
#include "debug.h"

#include <cstdio>

void init(Context const &context);

int main()
{
  Debug::Log("TRACE", "testing");

  Context context;
  init(context); 

  return EXIT_SUCCESS;
}

void init(Context const &context)
{
  initWindow(context.window);
  initGraphics(context.graphics);
}

