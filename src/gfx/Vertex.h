#ifndef GFX_VERTEX_H
#define GFX_VERTEX_H

#include <glm/glm.hpp>

namespace Gfx {
  struct Vertex
  {
    glm::vec3 iPosition;
    glm::vec2 iTexCoords;  
    glm::vec3 iNormal;
  };
}

#endif
