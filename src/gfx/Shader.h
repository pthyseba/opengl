#ifndef GFX_SHADER_H
#define GFX_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace Gfx {

class Shader
{
  public: 
    Shader();	  
    Shader(const char* aVertexShader, const char* aFragmentShader);
    void Activate() const;

    void SetInt(const std::string& aVariable, unsigned int aValue)
    {
      glUniform1i(glGetUniformLocation(iShaderProgram, aVariable.c_str()), aValue); 
    }

    void SetFloat(const std::string& aVariable, float aValue)
    {
      glUniform1f(glGetUniformLocation(iShaderProgram, aVariable.c_str()), aValue);
    }

    template<typename taType>
    void SetMatrix(const std::string& aMatrixName, const taType& aMatrix)
    {
      int projectionLoc = glGetUniformLocation(iShaderProgram, aMatrixName.c_str());
      glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(aMatrix));
    }

    static constexpr const char* KDefaultVertexShader = 
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "layout (location = 1) in vec2 aTexCoord;\n"
      "uniform mat4 view;\n"
      "uniform mat4 projection;\n"
      "out vec4 color;\n"
      "out vec2 TexCoord;\n"
      "void main()\n"
      "{\n"
      "   color = vec4(aPos, 1.0f);\n"
      "   gl_Position = projection*view*vec4(aPos, 1.0f);\n"
      "   TexCoord = aTexCoord;\n"
      "}\0";

    static constexpr const char* KDefaultFragmentShader =
      "#version 330 core\n"
      "struct Material {\n"
      "  vec3 ambient;\n"
      "  vec3 diffuse;\n"
      "  vec3 specular;\n"
      "  float shininess;\n"
      "  sampler2D texture_diffuse1;\n"
      "};\n"
      "uniform Material material;\n"
      "out vec4 FragColor;\n"
      "in vec4 color;\n"
      "in vec2 TexCoord;\n"
      "uniform sampler2D ourTexture\n;"
      "void main()\n"
      "{\n"
      "  FragColor = texture(material.texture_diffuse1, TexCoord);\n"
      "  /* FragColor = vec4(1.0f, abs(gl_FragCoord.z), 0.2f, 1.0f);\n*/"
      "  /* FragColor = color;\n*/"
      "}\n\0";

  private:
    int iShaderProgram;  
};

}

#endif
