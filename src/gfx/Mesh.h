#ifndef GFX_MESH_H
#define GFX_MESH_H

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

#include <vector>

namespace Gfx {
  class Mesh 
  {
    public:
	Mesh(const std::vector<Vertex>& aVertices, const std::vector<unsigned int> aIndices, const std::vector<Texture>& aTextures);
        ~Mesh();	
	void Draw(Shader& aShader) const;
    
    private:
	void Setup();

	std::vector<Vertex> iVertices;
        std::vector<unsigned int> iIndices;
	std::vector<Texture> iTextures;
	unsigned int iVAO, iVBO, iEBO;
  };

}

#endif
