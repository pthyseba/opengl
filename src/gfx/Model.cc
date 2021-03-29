#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <map>
#include <vector>
#include "glm/gtx/string_cast.hpp"

namespace Gfx {

unsigned int Model::CreateTexture(const std::string& aFilename)
{
  int width, height, nrChannels;
  unsigned int texture;
  unsigned char *data = stbi_load(aFilename.c_str(), &width, &height, &nrChannels, 0);
  if (stbi_failure_reason())
    std::cout << stbi_failure_reason() << std::endl;
  
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  return texture;
}

void Model::SetProjection(glm::mat4& aProjection)
{
  iShader->Activate();
  iShader->SetMatrix("projection", aProjection);
}

void Model::SetView(glm::mat4& aView)
{
  iShader->Activate();
  iShader->SetMatrix("view", aView);
}

void Model::SetDim(float aDim)
{
  iDim = aDim;
}

void Model::Draw() const
{
  iShader->Activate();
  iShader->SetFloat("dim", iDim);

  SetupLights();

  // Render scene
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  iShader->Activate();
  for (const auto& mesh : iMeshes)
  {
    mesh.Draw(*iShader);
  }
}

void Model::SetupShadowMap()
{
  glGenFramebuffers(10, iDepthMapFBO);
  const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
  glGenTextures(10, iDepthMap);
  for(int i = 0; i < 10; ++i)
  {
    glBindTexture(GL_TEXTURE_2D, iDepthMap[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	   SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, iDepthMapFBO[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, iDepthMap[i], 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Model::GenerateShadowMap(unsigned int aIndex, glm::vec3 aLightPos) const
{
  const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
  GLint currentViewport[4];
  glGetIntegerv(GL_VIEWPORT, currentViewport);
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, iDepthMapFBO[aIndex]);
  glClear(GL_DEPTH_BUFFER_BIT);

  iShadowShader->Activate();
  // Setup projection matrix and view matrix
  float near_plane = 0.01f, far_plane = 7.5f;
  glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
  iShadowShader->SetMatrix("projection", lightProjection);
  glm::mat4 lightView = glm::lookAt(aLightPos, glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  iShadowShader->SetMatrix("view", lightView);
  for (const auto& mesh : iMeshes)
  {
    mesh.PureDraw();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(currentViewport[0], currentViewport[1], currentViewport[2], currentViewport[3]);
  iShader->Activate();
  std::stringstream s;
  s << "lightspacematrix[" << aIndex << "]";
  iShader->SetMatrix(s.str().c_str(), lightProjection*lightView);
}

void Model::SetupLights() const
{
   iShader->Activate();
   int lights = 0;
   for(const auto& light : iLights)
   {
     if(light.second.IsEnabled())
     {
       const auto pos = light.second.GetPosition();
       std::stringstream s;
       s << "PointLights[" << lights << "].position";
       iShader->SetVec3(s.str().c_str(), pos.x, pos.y, pos.z);
       s.str("");
       s.clear();
       s << "PointLights[" << lights << "].color";
       const auto color = light.second.GetColor();
       iShader->SetVec3(s.str().c_str(), color.x, color.y, color.z);
       s.str("");
       s.clear();
       s << "PointLights[" << lights << "].intensity";
       const auto intensity = light.second.GetIntensity();
       iShader->SetFloat(s.str().c_str(), intensity);
       s.str("");
       s.clear();
       const auto attConstant = light.second.GetAttenuationConstant();
       s << "PointLights[" << lights << "].attenuationConstant";
       iShader->SetFloat(s.str().c_str(), attConstant);
       s.str("");
       s.clear();
       const auto attLinear = light.second.GetAttenuationLinear();
       s << "PointLights[" << lights << "].attenuationLinear";
       iShader->SetFloat(s.str().c_str(), attLinear);
       s.str("");
       s.clear();
       const auto attQuadratic = light.second.GetAttenuationQuadratic();
       s << "PointLights[" << lights << "].attenuationQuadratic";
       iShader->SetFloat(s.str().c_str(), attQuadratic);
       // Generate shadow map and configure shader
       GenerateShadowMap(lights, pos);
       s.str("");
       s.clear();
       s << "PointLights[" << lights << "].shadowmap";
       iShader->SetInt(s.str().c_str(), lights);

       lights++;
       if (lights == 10) // Max 10 lights supported
         break;
     }
   }
   for(int i = 0; i < lights; ++i)
   {
     glActiveTexture(GL_TEXTURE0+i);
     glBindTexture(GL_TEXTURE_2D, iDepthMap[i]);
     //std::cout << "Texture unit " << lights << " bound to depth map " << lights << std::endl;
   }
   iShader->SetInt("nrPointLights", lights);
   iShader->SetInt("lights", lights);
}


Mesh* Model::GetMesh(const std::string& aName)
{
  Mesh* result = nullptr;
  for (size_t i = 0; i < iMeshes.size(); i++)
  {
    if (iMeshes[i].GetName() == aName)
    {
      result = &iMeshes[i];
      break;
    }	    
  }
  return result;
}

Light* Model::GetLight(const std::string& aName)
{
  Light* result = nullptr;
  auto it = iLights.find(aName);
  if (it != iLights.end())
  {
    result = &(it->second);
  }
  return result;
}

const std::vector<Script>& Model::GetScripts() const
{
  return iScripts;
}

void Model::DumpNormals()
{
  Mesh::TFaceNormalMap faceNormals;	
  Mesh::TVertexNormalMap vertexNormals;
  for(size_t i = 0; i < iMeshes.size(); i++)
  {
    const auto& v = iMeshes[i].GetVertices();
    const auto& ind = iMeshes[i].GetIndices();
    // iterate over triples of indices
    for (int s = 0; s < ind.size(); s+=3)
    {
      const auto i1 = ind[s];
      const auto i2 = ind[s+1];
      const auto i3 = ind[s+2];
      const auto& v1 = v[i1];
      const auto& v2 = v[i2];
      const auto& v3 = v[i3];
      const auto r1 = v2.iPosition - v1.iPosition;
      const auto r2 = v3.iPosition - v2.iPosition;
      const auto n = glm::normalize(glm::cross(r1, r2));
      faceNormals[v1.iPosition].insert(n);
      faceNormals[v2.iPosition].insert(n);
      faceNormals[v3.iPosition].insert(n);
    }    
  }
  for(auto it = faceNormals.begin(); it!= faceNormals.end(); it++)
  {
    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
    for (const auto& n : it->second)
    {
      //std::cout << "Vertex " << glm::to_string(it->first) << " : face normal " << glm::to_string(n) << std::endl;	    
      normal = normal + n;
    }
    normal=glm::normalize(normal);
    //std::cout << "Vertex " << glm::to_string(it->first) << " : vertex normal " << glm::to_string(normal) << std::endl;
    vertexNormals[it->first] = normal;
  }

  for (size_t i = 0; i < iMeshes.size(); i++)
  {
    iMeshes[i].LoadVertexNormals(vertexNormals);
  }
}

} // namespace
