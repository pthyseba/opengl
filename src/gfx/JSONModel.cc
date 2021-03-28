#include "clipper.hpp"
#include <cmath>
#include <fstream>
#include "JSONModel.h"
#include "json.hpp"
#include <sstream>
#include <vector>

namespace Gfx {

using json = nlohmann::json;
  
void JSONModel::Load()
{
  try
  {
    std::ifstream inputFile(iFilename.c_str());
    std::stringstream text;
    text << inputFile.rdbuf();  
    json fullModel = json::parse(text.str().c_str());
    const auto inclusion = fullModel.at("inclusionzone");
    if (!inclusion.is_null())
    {
      const auto& x = inclusion.at("x");
      const auto& z = inclusion.at("z");
      if (x.is_null() || x.is_null())
      {
        std::cout << "Invalid polygon" << std::endl;
      } 
      if (x.size() != z.size())
      {
        std::cout << "Invalid coordinate array" << std::endl;
      }
      for (size_t i = 0; i < x.size(); i++)
      {
        iInclusionZone.x.push_back(x[i]);
	iInclusionZone.z.push_back(z[i]);
      }
    }
    const auto exclusion = fullModel.at("exclusionzones");
    if (!exclusion.is_null())
    {
      for(const auto& e : exclusion)
      {
        const auto& x = e.at("x");
	const auto& z = e.at("z");
	if (x.is_null() || x.is_null())
	{
	  std::cout << "Invalid polygon" << std::endl;
	}
	if (x.size() != z.size())
	{
	  std::cout << "Invalid coordinate array" << std::endl;
	}
	Polygon p;
	for (size_t i = 0; i < x.size(); i++)
	{
	  p.x.push_back(x[i]);
	  p.z.push_back(z[i]);
	}
	iExclusionZones.push_back(p);
      }
    }
    for(const auto& light : fullModel.at("lights"))
    {
      const std::string name = light.at("name");
      const auto pos = light.at("position").get<std::vector<float>>();
      const auto color = light.at("color").get<std::vector<float>>();
      const auto att = light.at("attenuation").get<std::vector<float>>();
      const auto intensity = light.at("intensity").get<float>();      
      
      iLights.insert({name, Light(pos[0], pos[1], pos[2], color[0], color[1], color[2], intensity, att[0], att[1], att[2])});
    } 
    for(const auto& script : fullModel.at("scripts"))
    {
      const std::string name = script.at("name");
      const std::string source = script.at("source");
      iScripts.push_back(Script(name, source));    
    }
    for(const auto& mesh : fullModel.at("meshes"))
    {
      const auto name = mesh.at("name");
      std::vector<Vertex> vertices;
      for (const auto& v : mesh.at("vertices"))
      {
	const auto coordinates = v.at("position").get<std::vector<float>>();
        const auto texcoordinates = v.at("texcoords").get<std::vector<float>>();
	vertices.push_back(Vertex{{coordinates[0], coordinates[1], coordinates[2]}, {texcoordinates[0], texcoordinates[1]}});
      }
      const std::vector<unsigned int> indices = mesh.at("indices").get<std::vector<unsigned int>>();
      std::vector<Texture> textures;
      for (const auto& t : mesh.at("textures"))
      {
        const unsigned int textureId = CreateTexture(t.at("path"));
	textures.push_back(Texture{textureId, t.at("type")});
      }
      iMeshes.push_back(Mesh{name,vertices, indices, textures});
    }
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
  catch(...)
    {
    }
}

bool JSONModel::InModel(glm::vec3 aPosition)
{
  const double scale = 100.0;
  // Use 100.0 scale factor
  // Check whether XZ coordinates of position are 
  // a. NOT in an exclusion zone
  // b. in the inclusion zone if it is specified
  ClipperLib::IntPoint p{round(scale*aPosition.x), round(scale*aPosition.z)};
  
  for (const auto& e : iExclusionZones)
  {
    ClipperLib::Path poly;
    for (size_t i = 0; i < std::min(e.x.size(), e.z.size()); i++)
    {
      poly << ClipperLib::IntPoint(round(scale*e.x[i]), round(scale*e.z[i]));
    }
    if (ClipperLib::PointInPolygon(p, poly))
    {
      return false;
    }
  }
  
  if (iInclusionZone.x.size() > 0)
  {
    ClipperLib::Path poly;
    for (size_t i = 0; i < std::min(iInclusionZone.x.size(), iInclusionZone.z.size()); i++)
    {
      poly << ClipperLib::IntPoint(round(scale*iInclusionZone.x[i]), round(scale*iInclusionZone.z[i]));
    }
    return ClipperLib::PointInPolygon(p, poly);
  }
  
  return true;
}



} // namespace
