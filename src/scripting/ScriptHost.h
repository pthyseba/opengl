#ifndef SCRIPTING_SCRIPTHOST_H
#define SCRIPTING_SCRIPTHOST_H

#include "gfx/Model.h"
#include <memory>
#include "squall/squall_vmstd.hpp"
#include <string>

namespace App {
  class Controller;
}

namespace Scripting {

class ScriptHost
{
  public:
    ScriptHost(App::Controller& aController, Gfx::Model& aModel);
    void RunScripts() const;  
    // Squirrel bindings 
    // Rotate mesh; axis through centroid paralley with Y-axis
    void RotateY(const std::string& aMesh, double angle) const; 
    // Toggle light source
    void ToggleLight(const std::string& aLight) const;

  private:
    App::Controller& iController;
    Gfx::Model& iModel;
    std::unique_ptr<squall::VMStd> iVM;
};


} // namespace


#endif // SCRIPTING_SCRIPTHOST_H
