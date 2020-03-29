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
  iPressedKeys.LeftPressed = false;
  iPressedKeys.RightPressed = false;
  iPressedKeys.UpPressed = false;
  iPressedKeys.DownPressed = false;
}

void Controller::Run()
{
  iSoundManager->LoadTrack("../resources/sfx/track.wav");
  iSoundManager->StartMusic();
  iModel->Load();
  while (!iWindowManager->ShouldClose())
  {
    iWindowManager->Clear();

    iShader->Activate();
   
    iCamera->ConfigureShader(*iShader, iScreenWidth, iScreenHeight);

    iModel->Draw(*iShader);

    iWindowManager->SwapBuffers();
    iWindowManager->ProcessInput();
    ProcessPressedKeys();
  }
}

void Controller::ProcessKey(Key aKey, KeyEvent aEvent)
{
  if (aKey == Key::KEY_ESCAPE && aEvent == KeyEvent::EVENT_PRESS)
    iWindowManager->TryClose();

  if (aEvent == KeyEvent::EVENT_RELEASE && (aKey == Key::KEY_UP || aKey == Key::KEY_DOWN))
    iSpeed = 0.1f;

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

void Controller::ProcessPressedKeys() 
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

}
