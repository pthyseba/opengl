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
  }
}

void Controller::ProcessKey(Key aKey, KeyEvent aEvent)
{
  if (aKey == Key::KEY_ESCAPE && aEvent == KeyEvent::EVENT_PRESS)
    iWindowManager->TryClose();	  
  if (aKey == Key::KEY_LEFT && (aEvent == KeyEvent::EVENT_PRESS || aEvent == KeyEvent::EVENT_REPEAT))
    iCamera->Rotate(2.0f);
  if (aKey == Key::KEY_RIGHT && (aEvent == KeyEvent::EVENT_PRESS || aEvent == KeyEvent::EVENT_REPEAT))
    iCamera->Rotate(-2.0f);
  if (aKey == Key::KEY_UP && (aEvent == KeyEvent::EVENT_PRESS || aEvent == KeyEvent::EVENT_REPEAT)) 
  {
    Gfx::Camera newCamera(*iCamera);
    newCamera.Translate(iSpeed);
    if (iModel->InModel(newCamera.GetPosition()))
    {
      iCamera->Translate(iSpeed);
      iSpeed = 1.2*iSpeed;
    }
    else if (iSpeed > 0.1f)
    {
      iSpeed = 0.1f;
      iSoundManager->PlayEffect("../resources/sfx/bump.wav");
    }
  }
  if (aKey == Key::KEY_DOWN && (aEvent == KeyEvent::EVENT_PRESS || aEvent == KeyEvent::EVENT_REPEAT))
  {
    Gfx::Camera newCamera(*iCamera);
    newCamera.Translate(-iSpeed);
    if (iModel->InModel(newCamera.GetPosition())) 
    {
      iCamera->Translate(-iSpeed);
      iSpeed = 1.2*iSpeed;
    }
    else if (iSpeed > 0.1f) 
    {
      iSpeed = 0.1f;
      iSoundManager->PlayEffect("../resources/sfx/bump.wav");
    }
  }

  if ((aKey == Key::KEY_DOWN || aKey == Key::KEY_UP) && aEvent == KeyEvent::EVENT_RELEASE)
  {
    iSpeed = 0.1f;
  } 
}

}
