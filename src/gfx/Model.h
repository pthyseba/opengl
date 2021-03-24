#ifndef GFX_MODEL_H
#define GFX_MODEL_H

#include "Light.h"
#include "Mesh.h"
#include "Script.h"
#include "Shader.h"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace Gfx {

class Model
{
  public: 
    Model()
    {
      iDim = 1.0f;
      iShader = std::make_unique<Gfx::Shader>();
      iLightsChanged = true;
    }

    void SetProjection(glm::mat4& aProjection)
    {
      iShader->Activate();
      iShader->SetMatrix("projection", aProjection);
    }

    void SetView(glm::mat4& aView)
    {
      iShader->Activate();
      iShader->SetMatrix("view", aView);
    }

    virtual void Load() = 0;
    virtual bool InModel(glm::vec3 aPosition) = 0;
    
    void Draw() const
    {
      iShader->Activate();
      iShader->SetFloat("dim", iDim);

      if (iLightsChanged)
      {
        SetupLights();
      }

      for (const auto& mesh : iMeshes)
      {
	mesh.Draw(*iShader);
      }
    }

    Mesh* GetMesh(const std::string& aName);

    const std::vector<Script>& GetScripts() const
    {
      return iScripts;
    }

    void DumpNormals();

    void SetDim(float aDim)
    {
      iDim = aDim;
    }

    void ToggleLight(const std::string& aLight)
    {
      if (iLights.count(aLight) > 0)
      {
        iLights.at(aLight).Toggle();
        iLightsChanged = true;	
      }
    }

  private:
    void SetupLights() const 
    {
       int lights = 0;                                                                                                                                                                                                                              std::vector<glm::vec3> positions;                                                                                                                                                                                                            for(const auto& light : iLights)                                                                                                                                                                                                             {                                                                                                                                                                                                                                              if(light.second.IsEnabled())                                                                                                                                                                                                                 {                                                                                                                                                                                                                                              const auto pos = light.second.GetPosition();                                                                                                                                                                                                 std::stringstream s;                                                                                                                                                                                                                         s << "PointLights[" << lights << "]";                                                                                                                                                                                                        iShader->SetVec3(s.str().c_str(), pos.x, pos.y, pos.z);                                                                                                                                                                                      lights++;                                                                                                                                                                                                                                  }                                                                                                                                                                                                                                          }                                                                                                                                                                                                                                            iShader->SetInt("nrPointLights", lights);
       iLightsChanged = false;
    }

  protected:
    unsigned int CreateTexture(const std::string& aFilename);
    std::unique_ptr<Shader> iShader;
    std::vector<Mesh> iMeshes;
    std::vector<Script> iScripts;
    std::map<std::string, Light> iLights;
    float iDim;
    mutable bool iLightsChanged;
};

}

#endif
