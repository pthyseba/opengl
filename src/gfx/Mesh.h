#ifndef GFX_MESH_H
#define GFX_MESH_H

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

#include <string>
#include <vector>

namespace Gfx {
  class Mesh 
  {
    public:
	Mesh(const std::string& aName,const std::vector<Vertex>& aVertices, const std::vector<unsigned int> aIndices, const std::vector<Texture>& aTextures);
        ~Mesh();	
	void Draw(Shader& aShader) const;
        std::string GetName() const {return iName;};
        void RotateY(double aAngle);

    private:
        void Load();

	std::string iName;
	std::vector<Vertex> iVertices;
        std::vector<unsigned int> iIndices;
	std::vector<Texture> iTextures;
	unsigned int iVAO, iVBO, iEBO;
  };

}

#endif
