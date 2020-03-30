#include "FontManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdexcept>

namespace Gfx {

FontManager::FontManager(GLuint aScreenWidth, GLuint aScreenHeight)
: iScreenWidth(aScreenWidth), iScreenHeight(aScreenHeight)
{
  // Create text shader
  const char vs[] = {
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex;\n" // <vec2 pos, vec2 tex>
    "out vec2 TexCoords;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    "TexCoords = vertex.zw;\n"
    "}\n"  
  };

  const char fs[] = {
    "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D text;\n"
    "uniform vec3 textColor;\n"
    "void main()\n"
    "{\n"    
    "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
    "color = vec4(textColor, 1.0) * sampled;\n"
    "}\n"
  };

  iShader = std::make_unique<Shader>(vs, fs);
}

void FontManager::LoadFont(const char* aFont)
{
  for (const auto& character : iCharacters)
  {
    glDeleteTextures(1, &character.second.iTextureId);
  }

  FT_Library ft;
  if (FT_Init_FreeType(&ft))
    throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

  FT_Face face;
  if (FT_New_Face(ft, aFont, 0, &face))
    throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

  FT_Set_Pixel_Sizes(face, 0, 48);  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
  for (GLubyte c = 0; c < 128; c++)
  {
    // Load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
        throw std::runtime_error("ERROR::FREETYTPE: Failed to load Glyph");
    }
    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Now store character for later use
    Character character = {
      texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      face->glyph->advance.x
    };
    iCharacters.insert(std::pair<GLchar, Character>(c, character));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  iProjection = glm::ortho(0.0f, (float)iScreenWidth, 0.0f, (float)iScreenHeight);

  // Reserve buffer memory: 1 rectangle (screen) == 2 triangles == 6 vertices.
  // 1 vertex: 2 position floats, 2 texture coord floats
  glGenVertexArrays(1, &iVAO);
  glGenBuffers(1, &iVBO);
  glBindVertexArray(iVAO);
  glBindBuffer(GL_ARRAY_BUFFER, iVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void FontManager::Render(const std::string& aText, GLfloat aXPos, GLfloat aYPos, GLfloat aScale, glm::vec3 aColor)
{
  // Enable blending with background	
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // Activate corresponding render state	
  iShader->Activate();
  iShader->SetMatrix("projection", iProjection);
  iShader->SetVec3("textColor", aColor.x, aColor.y, aColor.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(iVAO);

  // Iterate through all characters
  std::string::const_iterator c;
  for (c = aText.begin(); c != aText.end(); c++)
  {
    Character ch = iCharacters[*c];

    GLfloat xpos = aXPos + ch.iBearing.x * aScale;
    GLfloat ypos = aYPos - (ch.iSize.y - ch.iBearing.y) * aScale;

    GLfloat w = ch.iSize.x * aScale;
    GLfloat h = ch.iSize.y * aScale;
    // Update VBO for each character
    GLfloat vertices[6][4] = {
      { xpos,     ypos + h,   0.0, 0.0 },            
      { xpos,     ypos,       0.0, 1.0 },
      { xpos + w, ypos,       1.0, 1.0 },

      { xpos,     ypos + h,   0.0, 0.0 },
      { xpos + w, ypos,       1.0, 1.0 },
      { xpos + w, ypos + h,   1.0, 0.0 }           
    };
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.iTextureId);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, iVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    aXPos += (ch.iAdvance >> 6) * aScale; // Bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  // Reset blending
  glDisable(GL_BLEND);
}

}
