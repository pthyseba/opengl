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
#include <iostream>
#include "glm/gtx/string_cast.hpp"
namespace Gfx {

class Model
{
  public: 
    virtual ~Model()
    {}

    Model()
    {
      iDim = 1.0f;
      iShader = std::make_unique<Gfx::Shader>();
     
      static constexpr const char* KSimpleShadowVertexShader =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "uniform mat4 view;\n"
      "uniform mat4 projection;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = projection*view*vec4(aPos, 1.0f);\n"
      "}\0"; 
      static constexpr const char* KSimpleShadowFragmentShader =
      "#version 330 core\n"
      "void main()\n"
      "{\n"
      "}\0";
      
      iShadowShader = std::make_unique<Gfx::Shader>(KSimpleShadowVertexShader, KSimpleShadowFragmentShader);
      SetupShadowMap();
    }

    void SetProjection(glm::mat4& aProjection);
    void SetView(glm::mat4& aView);
    void SetDim(float aDim);

    virtual void Load() = 0;
    virtual bool InModel(glm::vec3 aPosition) = 0;
   
    void Draw() const;

    Mesh* GetMesh(const std::string& aName);
    Light* GetLight(const std::string& aName);
    const std::vector<Script>& GetScripts() const;

    void DumpNormals();

  private:
    void SetupShadowMap();
    void GenerateShadowMap(unsigned int aIndex, glm::vec3 aLightPos) const;
    void SetupLights() const;

  protected:
    unsigned int CreateTexture(const std::string& aFilename);
    std::unique_ptr<Shader> iShader;
    std::unique_ptr<Shader> iShadowShader;
    std::vector<Mesh> iMeshes;
    std::vector<Script> iScripts;
    std::map<std::string, Light> iLights;
    float iDim;
    unsigned int iDepthMapFBO[10];
    unsigned int iDepthMap[10];
};

}

#endif
