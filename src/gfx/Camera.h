#ifndef GFX_CAMERA_H
#define GFX_CAMERA_H

#include "Model.h"

namespace Gfx {

class Camera
{
  public:
    Camera(Model& aModel);
    void Translate(float aStep);
    void Rotate(float aDegrees);
    void ConfigureView(unsigned int aScreenWidth, unsigned int aScreenHeight);
    glm::vec3 GetPosition() const;
  private:
    Model& iModel;
    glm::vec3 iCameraPos;
    glm::vec3 iCameraNormal;
    glm::vec3 iCameraUp;    
};

}

#endif
