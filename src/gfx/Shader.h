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

    void SetBool(const std::string& aVariable, bool aValue)
    {
      glUniform1i(glGetUniformLocation(iShaderProgram, aVariable.c_str()), aValue);
    }

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
      "layout (location = 2) in vec3 aNormal;\n"
      "uniform mat4 view;\n"
      "uniform mat4 projection;\n"
      "uniform mat4 lightspacematrix[10];\n"
      "uniform int lights;\n"
      "out vec2 TexCoord;\n"
      "out vec3 Normal;\n"
      "out vec3 FragPos;\n"
      "out vec4 FragPosLightspace[10];\n"
      "void main()\n"
      "{\n"
      "   gl_Position = projection*view*vec4(aPos, 1.0f);\n"
      "   FragPos = aPos;\n"
      "   TexCoord = aTexCoord;\n"
      "   Normal = aNormal;\n"
      "   for(int i=0; i < lights; ++i) {\n"
      "     FragPosLightspace[i] = lightspacematrix[i]*vec4(FragPos, 1.0);\n"
      "   }\n"
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
      "struct Light {\n"
      "  vec3 position;\n"
      "  vec3 color;\n"
      "  float intensity;\n"
      "  float attenuationConstant;\n"
      "  float attenuationLinear;\n"
      "  float attenuationQuadratic;\n"
      "  sampler2D shadowmap;\n"
      "};\n"
      "uniform Material material;\n"
      "uniform float dim;\n"
      "uniform Light PointLights[10];\n"
      "uniform int nrPointLights;\n"
      "out vec4 FragColor;\n"
      "in vec2 TexCoord;\n"
      "in vec3 Normal;\n"
      "in vec3 FragPos;\n"
      "in vec4 FragPosLightspace[10];\n"
      "float Shadow(int index)\n"
      "{\n"
      "  vec3 projCoords = FragPosLightspace[index].xyz;\n"
      "  projCoords = projCoords / FragPosLightspace[index].w;\n"
      "  projCoords = projCoords * 0.5 + 0.5;\n"
      "  float closestDepth; // OpenGL 330 and dynamic indexing into sampler array \n"
      "  if (index == 0) closestDepth = texture(PointLights[0].shadowmap, projCoords.xy).x;\n"
      "  if (index == 1) closestDepth = texture(PointLights[1].shadowmap, projCoords.xy).x;\n"
      "  if (index == 2) closestDepth = texture(PointLights[2].shadowmap, projCoords.xy).x;\n"
      "  if (index == 3) closestDepth = texture(PointLights[3].shadowmap, projCoords.xy).x;\n"
      "  if (index == 4) closestDepth = texture(PointLights[4].shadowmap, projCoords.xy).x;\n"
      "  if (index == 5) closestDepth = texture(PointLights[5].shadowmap, projCoords.xy).x;\n"
      "  if (index == 6) closestDepth = texture(PointLights[6].shadowmap, projCoords.xy).x;\n"
      "  if (index == 7) closestDepth = texture(PointLights[7].shadowmap, projCoords.xy).x;\n"
      "  if (index == 8) closestDepth = texture(PointLights[8].shadowmap, projCoords.xy).x;\n"
      "  if (index == 9) closestDepth = texture(PointLights[9].shadowmap, projCoords.xy).x;\n"
      "  float currentDepth = projCoords.z;\n"
      "  float shadow = currentDepth - 0.005 > closestDepth  ? 1.0 : 0.0;\n"
      "  return shadow;\n"
      "}\n"
      "void main()\n"
      "{\n"
      "  float ambient = 0.001f;\n"
      "  vec3 textureColor = texture(material.texture_diffuse1, TexCoord).xyz;\n"
      "  vec3 result = ambient*textureColor;\n"
      "  for(int i = 0; i < nrPointLights; i++){\n"
      "    vec3 lightDir = normalize(PointLights[i].position - FragPos);\n"
      "    float diff = max(dot(Normal, lightDir), 0.0);\n"
      "    float dist = length(PointLights[i].position - FragPos);\n"
      "    float att = 1.0 / (PointLights[i].attenuationConstant + PointLights[i].attenuationLinear*dist + PointLights[i].attenuationQuadratic*dist*dist);\n" 
      "    float shadow = Shadow(i);\n"
      "    result = result + (1.0-shadow)*att*diff*PointLights[i].intensity*PointLights[i].color*textureColor;\n"
      "  }\n"
      "  FragColor = vec4(dim*result,1.0f);\n"
      "}\n\0";

  private:
    int iShaderProgram;  
};

}

#endif
