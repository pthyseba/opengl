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

void Model::DumpNormals()
{
  Mesh::TFaceNormalMap faceNormals;	
  Mesh::TVertexNormalMap vertexNormals;
  //std::map<glm::vec3, std::vector<glm::vec3>, vec3Comparator> faceNormals;
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
      std::cout << "Vertex " << glm::to_string(it->first) << " : face normal " << glm::to_string(n) << std::endl;	    
      normal = normal + n;
    }
    //normal = normal / (float) (it->second).size();
    normal=glm::normalize(normal);
    std::cout << "Vertex " << glm::to_string(it->first) << " : vertex normal " << glm::to_string(normal) << std::endl;
    std::cout << std::endl;
    vertexNormals[it->first] = normal;
  }

  for (size_t i = 0; i < iMeshes.size(); i++)
  {
    iMeshes[i].LoadVertexNormals(vertexNormals);
  }
}

} // namespace
