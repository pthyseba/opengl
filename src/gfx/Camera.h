#ifndef GFX_CAMERA_H
#define GFX_CAMERA_H

#include "Shader.h"

namespace Gfx {

class Camera
{
  public:
    Camera();
    void Translate(float aStep);
    void Rotate(float aDegrees);
    void ConfigureShader(Gfx::Shader& aShader, unsigned int aScreenWidth, unsigned int aScreenHeight);
    glm::vec3 GetPosition() const;
  private:
    glm::vec3 iCameraPos;
    glm::vec3 iCameraNormal;
    glm::vec3 iCameraUp;    
};

}

#endif
