#ifndef GFX_SCRIPT_H
#define GFX_SCRIPT_H

namespace Gfx {

class Script
{
public:
  Script(const std::string& aName, const std::string& aSource) : iName(aName), iSource(aSource) {}
  
  std::string GetName() const 
  {
    return iName;
  }

  std::string GetSource() const
  {
    return iSource;
  }

private:
  std::string iName;
  std::string iSource;
};

} // namespace

#endif

