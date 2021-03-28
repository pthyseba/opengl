#include "ScriptHost.h"
#include <iostream>
#include "gfx/Mesh.h"

namespace Scripting {

ScriptHost::ScriptHost(Gfx::Model& aModel)
  :iModel(aModel)
{
  iVM = std::make_unique<squall::VMStd>();
  // Bind functions and make them available to Squirrel scripts
  iVM->defun("RotateY", [=](std::string aMesh, float aAngle){this->RotateY(aMesh, aAngle);});
  iVM->defun("ToggleLight", [=](std::string aLight){this->ToggleLight(aLight);});
  iVM->defun("GetLightState", [=](std::string aLight){return this->GetLightState(aLight);});
  iVM->defun("SetLightIntensity", [=](std::string aLight, float aIntensity){this->SetLightIntensity(aLight, aIntensity);});
  iVM->defun("GetTimeMilliseconds", [=](){return this->GetTimeMilliseconds();});
}

void ScriptHost::RunScripts() const
{
  for(const auto& script: iModel.GetScripts())
  {
    iVM->dostring(script.GetSource().c_str());
    iVM->call<void>("run");
  }
}

void ScriptHost::RotateY(const std::string& aMesh, double angle) const
{
  Gfx::Mesh* m = iModel.GetMesh(aMesh);
  if (m != nullptr)
  {
    m->RotateY(angle);
  }
}

void ScriptHost::ToggleLight(const std::string& aLight) const
{
  Gfx::Light* l = iModel.GetLight(aLight);
  if (l != nullptr)
  {
    l->Toggle();
  }
}

bool ScriptHost::GetLightState(const std::string& aLight) const
{
  Gfx::Light* l = iModel.GetLight(aLight);
  if (l != nullptr)
  {
    return l->IsEnabled();
  }
  return false;
}

void ScriptHost::SetLightIntensity(const std::string& aLight, float aIntensity) const
{
  Gfx::Light* l = iModel.GetLight(aLight);
  if (l != nullptr)
  {
    l->SetIntensity(aIntensity);
  }
}

unsigned long ScriptHost::GetTimeMilliseconds() const
{
  unsigned long ms; // Milliseconds
  time_t s;  // Seconds
  struct timespec spec;

  clock_gettime(CLOCK_REALTIME, &spec);
  s  = spec.tv_sec;
  ms = round(spec.tv_nsec / 1.0e6); 

  return (s*1000 + ms); 
}

} // namespace


