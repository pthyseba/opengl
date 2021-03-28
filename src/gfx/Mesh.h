#ifndef GFX_MESH_H
#define GFX_MESH_H

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include <map>
#include <set>
#include <string>
#include <vector>

namespace Gfx {
  struct vec3Comparator
  {
    bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const
    {
      return lhs.x < rhs.x || lhs.x == rhs.x && (lhs.y < rhs.y || lhs.y == rhs.y && lhs.z < rhs.z);
    }
  };

  class Mesh 
  {
    public:
	Mesh(const std::string& aName,const std::vector<Vertex>& aVertices, const std::vector<unsigned int> aIndices, const std::vector<Texture>& aTextures);
        ~Mesh();	
	void Draw(Shader& aShader) const;
	void PureDraw() const;
        std::string GetName() const {return iName;};
        void RotateY(double aAngle);
        const std::vector<Vertex>& GetVertices() const
	{
	  return iVertices;
	}
	const std::vector<unsigned int>& GetIndices() const
	{
          return iIndices;
	}

	typedef std::set<glm::vec3, vec3Comparator> TVec3Set;
	typedef std::map<glm::vec3, TVec3Set, vec3Comparator> TFaceNormalMap;
	typedef std::map<glm::vec3, glm::vec3, vec3Comparator> TVertexNormalMap;
	void LoadVertexNormals(const TVertexNormalMap& aMap);
        
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
