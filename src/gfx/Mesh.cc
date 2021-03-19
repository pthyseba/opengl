#include "Mesh.h"
#include <string>

namespace Gfx {

Mesh::Mesh(const std::string& aName, const std::vector<Vertex>& aVertices, const std::vector<unsigned int> aIndices, const std::vector<Texture>& aTextures)
: iName(aName), iVertices(aVertices), iIndices(aIndices), iTextures(aTextures)
{
  Setup();
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
  for(unsigned int i = 0; i < iTextures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = iTextures[i].iType;
    if(name == "texture_diffuse")
      number = std::to_string(diffuseTextureNr++);
    else if(name == "texture_specular")
      number = std::to_string(specularTextureNr++);

    aShader.SetInt(("material." + name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, iTextures[i].iId);
  }
  glActiveTexture(GL_TEXTURE0);
  // draw mesh
  glBindVertexArray(iVAO);
  glDrawElements(GL_TRIANGLES, iIndices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::Setup()
{
  glGenVertexArrays(1, &iVAO);
  glGenBuffers(1, &iVBO);
  glGenBuffers(1, &iEBO);
  
  glBindVertexArray(iVAO);
  glBindBuffer(GL_ARRAY_BUFFER, iVBO);

  glBufferData(GL_ARRAY_BUFFER, iVertices.size() * sizeof(Vertex), &iVertices[0], GL_STATIC_DRAW);  

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, iIndices.size() * sizeof(unsigned int), 
               &iIndices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);	
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex normals
  //glEnableVertexAttribArray(1);	
  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, iNormal));
  // vertex texture coords
  glEnableVertexAttribArray(1);	
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, iTexCoords));

  glBindVertexArray(0);
}

}
