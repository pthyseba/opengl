#ifndef GFX_WINDOWMANAGER_H
#define GFX_WINDOWMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace App {
  class Controller;
}

namespace Gfx {

class WindowManager;

struct Environment
{
  App::Controller* iController;
  Gfx::WindowManager* iWindowManager;
};

class WindowManager
{
  public:
    WindowManager(App::Controller* aController, unsigned int aScreenWidth, unsigned int aScreenHeight, const std::string& aTitle);
    ~WindowManager();

    void TryClose() const
    {
      glfwSetWindowShouldClose(iWindow, GLFW_TRUE);
    }

    bool ShouldClose() const
    {
      return glfwWindowShouldClose(iWindow);
    }

    void SwapBuffers() const
    {
      glfwSwapBuffers(iWindow);
    }

    void ProcessInput() const
    {
      glfwPollEvents();
    }

    void Clear();

  private:
    static void framebuffer_size_callback(GLFWwindow* aWindow, int aWidth, int aHeight)
    {
      // make sure the viewport matches the new window dimensions; note that width and
      // height will be significantly larger than specified on retina displays.
      glViewport(0, 0, aWidth, aHeight);
    }

    static void key_callback(GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aMods);

    GLFWwindow* iWindow;  
    Environment iEnvironment;
};

}

#endif
