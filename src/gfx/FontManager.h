#ifndef GFX_FONTMANAGER_H
#define GFX_FONTMANAGER_H

#include "Character.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <memory>

namespace Gfx {

class FontManager
{
  public:
    FontManager(GLuint aScreenWidth, GLuint aScreenHeight);
    void LoadFont(const char* aFont);
    void Render(const std::string& aText, GLfloat aXPos, GLfloat aYPos, GLfloat aScale, glm::vec3 aColor);

  private:
    std::map<GLchar, Character> iCharacters;
    GLuint iScreenWidth, iScreenHeight;
    std::unique_ptr<Shader> iShader;
    GLuint iVAO, iVBO;
    glm::mat4 iProjection;
};

}


#endif

