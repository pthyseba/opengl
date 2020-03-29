#ifndef GFX_DEFAULTMODEL_H
#define GFX_DEFAULTMODEL_H

#include "Model.h"

namespace Gfx {

class DefaultModel : public Model
{
  public:
    void Load() override;
    bool InModel(glm::vec3 aPosition) override;
};

}
#endif
