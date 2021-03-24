#ifndef GFX_LIGHT_H
#define GFX_LIGHT_H

#include <glm/glm.hpp>

namespace Gfx
{

class Light
{
  public:
    Light(float aX, float aY, float aZ)
    {
      iPosition.x = aX;
      iPosition.y = aY;
      iPosition.z = aZ;
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

  private:
    bool iEnabled;
    glm::vec3 iPosition;    
};

} // namespace

#endif
