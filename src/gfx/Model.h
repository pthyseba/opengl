#ifndef GFX_MODEL_H
#define GFX_MODEL_H

#include "Mesh.h"
#include "Script.h"
#include "Shader.h"
#include <string>
#include <vector>

namespace Gfx {

class Model
{
  public:
    virtual void Load() = 0;
    virtual bool InModel(glm::vec3 aPosition) = 0;
    
    void Draw(Shader& aShader) const
    {
      aShader.Activate();
      for (const auto& mesh : iMeshes)
      {
	mesh.Draw(aShader);
      }
    }

    Mesh* GetMesh(const std::string& aName);

    const std::vector<Script>& GetScripts() const
    {
      return iScripts;
    }

    
  protected:
    unsigned int CreateTexture(const std::string& aFilename);
    std::vector<Mesh> iMeshes;
    std::vector<Script> iScripts;
};

}

#endif
