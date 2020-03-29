#include "DefaultModel.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Gfx {

void DefaultModel::Load() 
{
  // Load Textures
  int width, height, nrChannels;
  unsigned char *wallData = stbi_load("../resources/textures/dark-grungy-wall-texture-background.jpg", &width, &height, &nrChannels, 0);
  unsigned int wallTexture, floorTexture, ceilTexture;
  glGenTextures(1, &wallTexture);
  glBindTexture(GL_TEXTURE_2D, wallTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(wallData);

  unsigned char *floorData = stbi_load("../resources/textures/brick-wall-dark.jpg", &width, &height, &nrChannels, 0);
  glGenTextures(1, &floorTexture);
  glBindTexture(GL_TEXTURE_2D, floorTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, floorData);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(floorData);

  unsigned char *ceilData = stbi_load("../resources/textures/night-sky.jpg", &width, &height, &nrChannels, 0);
  glGenTextures(1, &ceilTexture);
  glBindTexture(GL_TEXTURE_2D, ceilTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ceilData);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(ceilData);

  glm::vec3 g{0.5f,  0.5f, -5.0f};
  // Walls
  std::vector<Vertex> vertices { // coord, texCoord
    Vertex{{0.5f,  0.5f, -5.0f}, {/* texCoord */ 1.0f, 1.0f}},  // upright, top right
    Vertex{{0.5f, -0.5f, -5.0f}, {1.0f, 0.0f}}, // bottom right
    Vertex{{-0.5f, -0.5f, -5.0f}, {0.0f, 0.0f}}, // bottom left
    Vertex{{-0.5f,  0.5f, -5.0f}, {0.0f, 1.0f}}, // top left
    Vertex{{-0.5f, -0.5f,  5.0f}, {1.0f, 0.0f}}, // floor, front left
    Vertex{{0.5f, -0.5f,  5.0f}, {0.0f, 0.0f}},   // floor, front right
    Vertex{{-0.5f, 0.5f, 5.0f}, {1.0f, 1.0f}}, // front top left
    Vertex{{0.5f, 0.5f, 5.0f}, {0.0f, 1.0f}}// front top right
  };

  std::vector<unsigned int> wallIndices {  // note that we start from 0!
    0, 1, 3,  // back wall first Triangle
    1, 2, 3,  // back wall second Triangle
    4, 2, 3,  // left wall triangle 1
    3, 6, 4,  // left wall triangle 2
    5, 7, 0,  // right wall triangle 1
    0, 1, 5,   // right wall triangle 2
    4, 6, 7,// front wall triangle 1
    7, 5, 4// front wall triangle 2
  };

  iMeshes.push_back(Mesh(vertices, wallIndices, {Texture{wallTexture, "texture_diffuse"}}));

  // Ceiling
  std::vector<Vertex> ceilVertices {
    Vertex{{0.5f,  0.5f, -5.0f}, {/* texCoord */ 1.0f, 0.0f}},  // upright, top right
    Vertex{{-0.5f,  0.5f, -5.0f}, {0.0f, 0.0f}}, // top left
    Vertex{{-0.5f, 0.5f, 5.0f}, {0.0f, 1.0f}}, // front top left
    Vertex{{0.5f, 0.5f, 5.0f}, {1.0f, 1.0f}}// front top right
  };

  std::vector<unsigned int> ceilIndices {
    0, 3, 2,// ceiling first triangle
    2, 1, 0 // ceiling second triangle
  };
  
  iMeshes.push_back(Mesh(ceilVertices, ceilIndices, {Texture{ceilTexture, "texture_diffuse"}}));  
  
  // Floor
  std::vector<Vertex> floorVertices {
    Vertex{{0.5f, -0.5f, -5.0f}, {1.0f, 1.0f}}, // bottom right
    Vertex{{-0.5f, -0.5f, -5.0f}, {0.0f, 1.0f}}, // bottom left
    Vertex{{-0.5f, -0.5f,  5.0f}, {0.0f, 0.0f}}, // floor, front left
    Vertex{{0.5f, -0.5f,  5.0f}, {1.0f, 0.0f}}   // floor, front right
  };

  std::vector<unsigned int> floorIndices {  // note that we start from 0!
    0, 1, 2,  // floor first triangle
    2, 3, 0   // floor second triangle
  };

  iMeshes.push_back(Mesh(floorVertices, floorIndices, {Texture{floorTexture, "texture_diffuse"}}));
}

bool DefaultModel::InModel(glm::vec3 aPosition)
{
  if (abs(aPosition.z) > 4.9 || abs(aPosition.x) > 0.4)
    return false;

  return true;
}

}
