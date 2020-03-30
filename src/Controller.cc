#include "Controller.h"

#include "gfx/DefaultModel.h"

namespace App {

Controller::Controller(unsigned int aScreenWidth, unsigned int aScreenHeight)
  : iScreenWidth(aScreenWidth), iScreenHeight(aScreenHeight)
{
  iSoundManager = std::make_shared<Sfx::SoundManager>();
  iWindowManager = std::make_shared<Gfx::WindowManager>(this, aScreenWidth, aScreenHeight, "Demo");
  iShader = std::make_shared<Gfx::Shader>();
  iModel = std::make_shared<Gfx::DefaultModel>(); 
  iSpeed = 0.1;
  iCamera = std::make_shared<Gfx::Camera>();
  iFontManager = std::make_shared<Gfx::FontManager>(aScreenWidth, aScreenHeight);
  iPressedKeys.LeftPressed = false;
  iPressedKeys.RightPressed = false;
  iPressedKeys.UpPressed = false;
  iPressedKeys.DownPressed = false;
  iMenu = false;
  iDim = 1.0f;
}

void Controller::Run()
{
  iSoundManager->LoadTrack("../resources/sfx/track.wav");
  iSoundManager->StartMusic();
  iFontManager->LoadFont("../resources/fonts/font.ttf");
  iModel->Load();
  while (!iWindowManager->ShouldClose())
  {
    iWindowManager->Clear();

    iShader->Activate();

    iShader->SetFloat("dim", iDim);   
    iCamera->ConfigureShader(*iShader, iScreenWidth, iScreenHeight);

    iModel->Draw(*iShader);

    if (iMenu)
      iFontManager->Render("Menu Item 1", 300, 250, 1, {1.0,0,0});
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
      iDim = 0.3f;
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
      iDim = 1.0f;
    }
  }
}

void Controller::ProcessPressedKeys() 
{
  if (!iMenu)
  {
    if (iPressedKeys.LeftPressed)
      iCamera->Rotate(2.0f);
    if (iPressedKeys.RightPressed)
      iCamera->Rotate(-2.0f);
    if (iPressedKeys.UpPressed) 
    {
      Gfx::Camera newCamera(*iCamera);
      newCamera.Translate(iSpeed);
      if (iModel->InModel(newCamera.GetPosition()))
      {
        iCamera->Translate(iSpeed);
        iSpeed = 1.2*iSpeed;
        if (iSpeed > 0.5f)
	  iSpeed = 0.5f;
      }
      else if (iSpeed > 0.1f)
      {
        iSpeed = 0.1f;
        iSoundManager->PlayEffect("../resources/sfx/bump.wav");
      }
    }
    if (iPressedKeys.DownPressed)
    {
      Gfx::Camera newCamera(*iCamera);
      newCamera.Translate(-iSpeed);
      if (iModel->InModel(newCamera.GetPosition())) 
      {
        iCamera->Translate(-iSpeed);
        iSpeed = 1.2*iSpeed;
        if (iSpeed > 0.5f)
          iSpeed = 0.5f;	      
      }
      else if (iSpeed > 0.1f) 
      {
        iSpeed = 0.1f;
        iSoundManager->PlayEffect("../resources/sfx/bump.wav");
      }
    }
  }
  else
  {
    // Menu
  }
}

}
