#include "../Controller.h"
#include "WindowManager.h"
#include <stdexcept>

namespace Gfx {

WindowManager::WindowManager(App::Controller* aController, unsigned int aScreenWidth, unsigned int aScreenHeight, const std::string& aTitle)
{
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
  iWindow = glfwCreateWindow(aScreenWidth, aScreenHeight, aTitle.c_str(), NULL, NULL);
  if (iWindow == NULL)
  {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(iWindow);
  glfwSetFramebufferSizeCallback(iWindow, framebuffer_size_callback);

  iEnvironment.iController = aController;
  iEnvironment.iWindowManager = this;
  glfwSetWindowUserPointer(iWindow, reinterpret_cast<void*>(&iEnvironment));
  glfwSetKeyCallback(iWindow, key_callback);
  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    throw std::runtime_error("Failed to initialize GLAD");
  }
  glEnable(GL_DEPTH_TEST);  
}

WindowManager::~WindowManager()
{
  glfwTerminate();
}

void WindowManager::Clear() 
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowManager::key_callback(GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aMods)
{
  Gfx::Environment* env = reinterpret_cast<Gfx::Environment*>(glfwGetWindowUserPointer(aWindow));
  App::Controller::Key key;
  switch(aKey)
  {  
    case GLFW_KEY_ESCAPE:
      key = App::Controller::Key::KEY_ESCAPE;
      break;
    case GLFW_KEY_UP:
      key = App::Controller::Key::KEY_UP;
      break;
    case GLFW_KEY_DOWN:
      key = App::Controller::Key::KEY_DOWN;
      break;
    case GLFW_KEY_LEFT:
      key = App::Controller::Key::KEY_LEFT;
      break;
    case GLFW_KEY_RIGHT:
      key = App::Controller::Key::KEY_RIGHT;
      break;
    case GLFW_KEY_SPACE:
      key = App::Controller::Key::KEY_SPACE;
      break;
  }
  App::Controller::KeyEvent event;
  switch(aAction)
  {
    case GLFW_RELEASE:
      event = App::Controller::KeyEvent::EVENT_RELEASE;
      break;
    case GLFW_PRESS:
      event = App::Controller::KeyEvent::EVENT_PRESS;
      break;
    case GLFW_REPEAT:
      event = App::Controller::KeyEvent::EVENT_REPEAT;
      break;
  }
  env->iController->ProcessKey(key, event); 
}

}
