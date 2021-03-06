#include "Controller.h"

#include "gfx/DefaultModel.h"
#include "gfx/JSONModel.h"

#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <sstream>
#include <cstdlib>

namespace App {

Controller::Controller(unsigned int aScreenWidth, unsigned int aScreenHeight)
  : iScreenWidth(aScreenWidth), iScreenHeight(aScreenHeight)
{
  iSoundManager = std::make_shared<Sfx::SoundManager>();
  iWindowManager = std::make_shared<Gfx::WindowManager>(this, aScreenWidth, aScreenHeight, "Demo");
  iModel = std::make_shared<Gfx::JSONModel>("../resources/models/samplemodel.json");
  iScriptHost = std::make_shared<Scripting::ScriptHost>(*iModel);
  iSpeed = 0.1;
  iCamera = std::make_shared<Gfx::Camera>(*iModel);
  iFontManager = std::make_shared<Gfx::FontManager>(aScreenWidth, aScreenHeight);
  iPressedKeys.LeftPressed = false;
  iPressedKeys.RightPressed = false;
  iPressedKeys.UpPressed = false;
  iPressedKeys.DownPressed = false;
  iMenu = false;
  srand(time(NULL));
}

void Controller::Run()
{
  iSoundManager->LoadTrack("../resources/sfx/track.wav");
  iSoundManager->StartMusic();
  iFontManager->LoadFont("../resources/fonts/font.ttf");
  iModel->Load();
  iModel->DumpNormals();
  iModel->SetDim(1.0f);
  while (!iWindowManager->ShouldClose())
  {
    // Run scripts
    iScriptHost->RunScripts();

    // Draw
    iWindowManager->Clear();
    iCamera->ConfigureView(iScreenWidth, iScreenHeight);
    iModel->Draw();

    if (iMenu)
    {
      for (size_t i = 0; i < 3; ++i)
      {
	std::stringstream s;
        s << "Menu Item " << i;
        if (i == iActiveMenuItem)
          iFontManager->Render("-", 250, 330-50*i, 1, {1.0,1.0,1.0});

        iFontManager->Render(s.str(), 300, 300-50*i, 1, {1.0,0.0,0.0});
      }
    }
    iWindowManager->SwapBuffers();
    iWindowManager->ProcessInput();
    ProcessPressedKeys();
  }
}

void Controller::ProcessKey(Key aKey, KeyEvent aEvent)
{
  if (!iMenu)
  {
    if (aKey == Key::KEY_ESCAPE && aEvent == KeyEvent::EVENT_PRESS)
      iWindowManager->TryClose();

    if (aEvent == KeyEvent::EVENT_RELEASE && (aKey == Key::KEY_UP || aKey == Key::KEY_DOWN))
      iSpeed = 0.1f;

    if (aEvent == KeyEvent::EVENT_PRESS && aKey == Key::KEY_SPACE)
    {
      iMenu = true;
      iActiveMenuItem = 0;
      iModel->SetDim(0.3f);
    }
    switch(aKey)
    {
      case Key::KEY_LEFT:
        iPressedKeys.LeftPressed = !(aEvent == KeyEvent::EVENT_RELEASE);
        break;
      case Key::KEY_RIGHT:
        iPressedKeys.RightPressed = !(aEvent == KeyEvent::EVENT_RELEASE);
        break;
      case Key::KEY_UP:
        iPressedKeys.UpPressed = !(aEvent == KeyEvent::EVENT_RELEASE);
        break;
      case Key::KEY_DOWN:
        iPressedKeys.DownPressed = !(aEvent == KeyEvent::EVENT_RELEASE);
        break;
    }
  }
  else
  {
    // Menu
    if (aKey == Key::KEY_ESCAPE && aEvent == KeyEvent::EVENT_PRESS)
    {
      iMenu = false; 
      iModel->SetDim(1.0f);
    }
    if (aKey == Key::KEY_DOWN && (aEvent == KeyEvent::EVENT_PRESS || aEvent == KeyEvent::EVENT_REPEAT))
    { 
      size_t oldActiveMenuItem = iActiveMenuItem;
      iActiveMenuItem = (iActiveMenuItem < 2 ? (iActiveMenuItem + 1) % 3 : 2);
      if (oldActiveMenuItem != iActiveMenuItem)
        iSoundManager->PlayEffect("../resources/sfx/menu.wav");
    }
    if (aKey == Key::KEY_UP && (aEvent == KeyEvent::EVENT_PRESS || aEvent == KeyEvent::EVENT_REPEAT))
    {
      size_t oldActiveMenuItem = iActiveMenuItem;	    
      iActiveMenuItem = (iActiveMenuItem > 0 ? (iActiveMenuItem-1)%3 : 0);
      if (oldActiveMenuItem != iActiveMenuItem)
        iSoundManager->PlayEffect("../resources/sfx/menu.wav");
    }
  }
}

void Controller::ProcessPressedKeys() 
{
  if (!iMenu)
  {
    Gfx::Camera currentCam(*iCamera);	  
    if (iPressedKeys.LeftPressed)
      iCamera->Rotate(2.0f);
    if (iPressedKeys.RightPressed)
      iCamera->Rotate(-2.0f);
    if (iPressedKeys.UpPressed) 
    {
      Gfx::Camera newCamera(*iCamera);
      newCamera.Translate(iSpeed);
      Gfx::Camera deltaCamera(*iCamera);
      deltaCamera.Translate(KMinSpeed);
      if (iModel->InModel(newCamera.GetPosition()) && iModel->InModel(deltaCamera.GetPosition()))
      {
        iCamera->Translate(iSpeed);
        iSpeed = 1.2*iSpeed;
        if (iSpeed > KMaxSpeed)
	  iSpeed = KMaxSpeed;
      }
      else if (iSpeed > KMinSpeed)
      {
        iSpeed = KMinSpeed;
        iSoundManager->PlayEffect("../resources/sfx/bump.wav");
      }
    }
    if (iPressedKeys.DownPressed)
    {
      Gfx::Camera newCamera(*iCamera);
      newCamera.Translate(-iSpeed);
      Gfx::Camera deltaCamera(*iCamera);
      deltaCamera.Translate(-KMinSpeed);
      if (iModel->InModel(newCamera.GetPosition()) && iModel->InModel(deltaCamera.GetPosition())) 
      {
        iCamera->Translate(-iSpeed);
        iSpeed = 1.2*iSpeed;
        if (iSpeed > KMaxSpeed)
          iSpeed = KMaxSpeed;	      
      }
      else if (iSpeed > KMinSpeed) 
      {
        iSpeed = KMinSpeed;
        iSoundManager->PlayEffect("../resources/sfx/bump.wav");
      }
    }
  }
  else
  {
    // Menu
  }
}

const double Controller::KMinSpeed = 0.01;
const double Controller::KMaxSpeed = 0.3;
}
