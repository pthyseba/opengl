#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "gfx/Camera.h"
#include "gfx/FontManager.h"
#include "gfx/Model.h"
#include "gfx/Shader.h"
#include "gfx/WindowManager.h"
#include "sfx/SoundManager.h"
#include "scripting/ScriptHost.h"
#include <memory>

namespace App {

class Controller
{
  public:
    Controller(unsigned int aScreenWidth, unsigned int aScreenHeight);
    
    void Run();
    
    enum class Key {
      KEY_UP,
      KEY_DOWN,
      KEY_LEFT,
      KEY_RIGHT,
      KEY_ESCAPE,
      KEY_SPACE
    };
    enum class KeyEvent {
      EVENT_PRESS,
      EVENT_RELEASE,
      EVENT_REPEAT
    };

    struct PressedKeys 
    {
      bool UpPressed;
      bool DownPressed;
      bool LeftPressed;
      bool RightPressed;
    };

    void ProcessKey(Key aKey, KeyEvent aKeyEvent);
  private:
    void ProcessPressedKeys();
    std::shared_ptr<Sfx::SoundManager> iSoundManager;
    std::shared_ptr<Gfx::WindowManager> iWindowManager;
    //std::shared_ptr<Gfx::Shader> iShader;
    std::shared_ptr<Gfx::Model> iModel;
    std::shared_ptr<Scripting::ScriptHost> iScriptHost;
    std::shared_ptr<Gfx::FontManager> iFontManager;
    bool iMenu;
    size_t iActiveMenuItem;
    std::shared_ptr<Gfx::Camera> iCamera;
    float iSpeed;
    //float iDim;
    unsigned int iScreenWidth;
    unsigned int iScreenHeight;
    PressedKeys iPressedKeys;
};

}

#endif
