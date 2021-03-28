#ifndef GFX_LIGHT_H
#define GFX_LIGHT_H

#include <glm/glm.hpp>

namespace Gfx
{

class Light
{
  public:
    Light(float aX, float aY, float aZ, float aColorR, float aColorG, float aColorB, float aIntensity, float aAttenuationConstant, float aAttenuationLinear, float aAttenuationQuadratic)
    {
      iPosition.x = aX;
      iPosition.y = aY;
      iPosition.z = aZ;
      iColor.x = aColorR;
      iColor.y = aColorG;
      iColor.z = aColorB;
      iIntensity = aIntensity;
      iAttenuationConstant = aAttenuationConstant;
      iAttenuationLinear = aAttenuationLinear;
      iAttenuationQuadratic = aAttenuationQuadratic; 
      iEnabled = true;
    }

    void Toggle()
    {
      iEnabled = !iEnabled;
    }

    bool IsEnabled() const
    {
      return iEnabled;
    }

    glm::vec3 GetPosition() const
    {
      return iPosition;
    }

    glm::vec3 GetColor() const
    {
      return iColor;
    }

    float GetIntensity() const
    {
      return iIntensity;
    }

    float GetAttenuationConstant() const
    {
      return iAttenuationConstant;
    }

    float GetAttenuationLinear() const
    {
      return iAttenuationLinear;
    }

    float GetAttenuationQuadratic() const
    {
      return iAttenuationQuadratic;
    }

    void SetIntensity(const float aIntensity)
    {
      iIntensity = aIntensity;
    }

  private:
    bool iEnabled;
    glm::vec3 iPosition;    
    glm::vec3 iColor;
    float iIntensity;
    float iAttenuationLinear;
    float iAttenuationQuadratic;
    float iAttenuationConstant;
};

} // namespace

#endif
