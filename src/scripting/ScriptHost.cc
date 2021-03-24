#include "ScriptHost.h"
#include <iostream>
#include "gfx/Mesh.h"

namespace Scripting {

//std::string Script::KTimeStampIdentifier = std::string{"TIME"};
//std::string Script::KLastRunIdentifier = std::string{"LASTRUN"};                                                  

ScriptHost::ScriptHost(App::Controller& aController, Gfx::Model& aModel)
  : iController(aController), iModel(aModel)
{
  iVM = std::make_unique<squall::VMStd>();
  // Bind functions and make them available to Squirrel scripts
  iVM->defun("RotateY", [=](std::string aMesh, float aAngle){this->RotateY(aMesh, aAngle);});
  iVM->defun("ToggleLight", [=](std::string aLight){this->ToggleLight(aLight);});
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
  iModel.ToggleLight(aLight);
}


} // namespace


