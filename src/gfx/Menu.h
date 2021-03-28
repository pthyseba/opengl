#ifndef GFX_MENU_H
#define GFX_MENU_H

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include "Shader.h"

namespace Gfx {

class Menu
{
  public:
    enum class MenuAction
    {
	ACTION_RETURN_TO_GAME,
	ACTION_QUIT
    };

    struct MenuItem
    {
      std::string iText;
      MenuAction iAction;
    };	    

    Menu(const std::vector<MenuItem>& aItems);
    MenuAction Show();

  private:
    std::vector<MenuItem> iItems;
    GLuint iTexture;
    GLuint iVAO, iVBO;
    std::unique_ptr<Gfx::Shader> iSkullShader;
};

}

#endif

