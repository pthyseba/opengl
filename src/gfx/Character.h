#ifndef GFX_CHARACTER_H
#define GFX_CHARACTER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Gfx {

struct Character
{
  GLuint     iTextureId;  // ID handle of the glyph texture
  glm::ivec2 iSize;       // Size of glyph
  glm::ivec2 iBearing;    // Offset from baseline to left/top of glyph
  GLuint     iAdvance;
};

}

#endif

