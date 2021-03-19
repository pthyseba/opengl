#ifndef JSONMODEL_H
#define JSONMODEL_H

#include "Model.h"
#include <string>
#include <vector>

namespace Gfx {

struct Polygon
{
  std::vector<double> x, z;
};

class JSONModel : public Model
{
  public:
    JSONModel(const std::string& aFilename) : iFilename(aFilename) {}
    void Load() override;
    bool InModel(glm::vec3 aPosition) override;    
  private:
    std::string iFilename;
    Polygon iInclusionZone;
    std::vector<Polygon> iExclusionZones;  
};

} // namespace


#endif // JSONMODEL_H
