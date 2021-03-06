#ifndef SCRIPTING_SCRIPTHOST_H
#define SCRIPTING_SCRIPTHOST_H

#include "gfx/Model.h"
#include <memory>
#include "squall/squall_vmstd.hpp"
#include <string>

namespace Scripting {

class ScriptHost
{
  public:
    ScriptHost(Gfx::Model& aModel);
    void RunScripts() const;  
    // Squirrel bindings 
    // Rotate mesh; axis through centroid paralley with Y-axis
    void RotateY(const std::string& aMesh, double angle) const; 
    // Toggle light source
    void ToggleLight(const std::string& aLight) const;
    bool GetLightState(const std::string& aLight) const;
    void SetLightIntensity(const std::string& aLight, float aIntensity) const;
    unsigned long GetTimeMilliseconds() const;
    
  private:
    Gfx::Model& iModel;
    std::unique_ptr<squall::VMStd> iVM;
};


} // namespace


#endif // SCRIPTING_SCRIPTHOST_H
