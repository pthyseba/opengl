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

    void SetVec3(const std::string& aVariable, float aX, float aY, float aZ)
    {
      glUniform3f(glGetUniformLocation(iShaderProgram, aVariable.c_str()), aX, aY, aZ);
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
      "out vec2 TexCoord;\n"
      "void main()\n"
      "{\n"
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
      "uniform float dim;\n"
      "out vec4 FragColor;\n"
      "in vec2 TexCoord;\n"
      "void main()\n"
      "{\n"
      "  FragColor = dim*texture(material.texture_diffuse1, TexCoord);\n"
      "}\n\0";

  private:
    int iShaderProgram;  
};

}

#endif
