#include "Mesh.h"
#include <string>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
namespace Gfx {

Mesh::Mesh(const std::string& aName, const std::vector<Vertex>& aVertices, const std::vector<unsigned int> aIndices, const std::vector<Texture>& aTextures)
: iName(aName), iVertices(aVertices), iIndices(aIndices), iTextures(aTextures)
{
  glGenVertexArrays(1, &iVAO);
  glGenBuffers(1, &iVBO);
  glGenBuffers(1, &iEBO);  
  Load();
}

Mesh::~Mesh()
{
  //glDeleteVertexArrays(1, &iVAO);
  //glDeleteBuffers(1, &iVBO);
  //glDeleteBuffers(1, &iEBO);
}

void Mesh::Draw(Shader& aShader) const
{
  unsigned int diffuseTextureNr = 1;
  unsigned int specularTextureNr = 1;
  unsigned int i = 0;
  for(i = 0; i < iTextures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + 10+i); // activate proper texture unit before binding, 10 shadow maps reserved
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = iTextures[i].iType;
    if(name == "texture_diffuse")
      number = std::to_string(diffuseTextureNr++);
    else if(name == "texture_specular")
      number = std::to_string(specularTextureNr++);

    aShader.SetInt(("material." + name + number).c_str(), 10+i);
    glBindTexture(GL_TEXTURE_2D, iTextures[i].iId);
    //std::cout << "Texture unit " << 10+i << " bound to texture " << iTextures[i].iId << std::endl;
  }
  // draw mesh
  PureDraw();
}

void Mesh::PureDraw() const
{
  glBindVertexArray(iVAO);
  glDrawElements(GL_TRIANGLES, iIndices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::Load()
{
  glBindVertexArray(iVAO);
  glBindBuffer(GL_ARRAY_BUFFER, iVBO);

  glBufferData(GL_ARRAY_BUFFER, iVertices.size() * sizeof(Vertex), &iVertices[0], GL_STATIC_DRAW);  

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, iIndices.size() * sizeof(unsigned int), 
               &iIndices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);	
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex texture coords
  glEnableVertexAttribArray(1);	
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, iTexCoords));
  // vertex normals
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, iNormal));
  glEnableVertexAttribArray(2);
  
  glBindVertexArray(0);
}

void Mesh::RotateY(double aAngle)
{
  if (iVertices.size() == 0)
  {
    return;
  }
  // Calculate centroid
  // Translate centroid to origin
  // Rotate around Y axis
  // Translate back
  double centroidx = 0.0, centroidy = 0.0, centroidz = 0.0;
  for (const auto& v : iVertices)
  {
    centroidx += v.iPosition.x;
    centroidy += v.iPosition.y;
    centroidz += v.iPosition.z;
  }
  centroidx = centroidx / (double) iVertices.size();
  centroidy = centroidy / (double) iVertices.size();
  centroidz = centroidz / (double) iVertices.size();
 
  // Translation
  glm::vec4 trans = glm::vec4(glm::vec3(centroidx, centroidy, centroidz), 0.0);
  // Rotation matrix
  glm::mat4 rot = glm::mat4(1.0f);
  rot = glm::rotate(rot, (float) aAngle, glm::vec3(0.0f, 1.0f, 0.0f));

  for (auto& v : iVertices)
  { 
    glm::vec4 origPos = glm::vec4(v.iPosition, 1.0);
    glm::vec4 origNormal = glm::vec4(v.iNormal, 1.0);
    v.iPosition = glm::vec3(trans + rot*(origPos - trans));
    v.iNormal = glm::vec3(rot*origNormal);
  }

  Load();
}

void Mesh::LoadVertexNormals(const TVertexNormalMap& aMap) 
{
  for (auto& v : iVertices)
  {
    if (aMap.count(v.iPosition) > 0)
    {
      v.iNormal = aMap.at(v.iPosition);
    }
    else
    {
      std::cout << "Vertex " << glm::to_string(v.iPosition) << " does not have a normal??" << std::endl;
    }
  }
  Load();
}

}
