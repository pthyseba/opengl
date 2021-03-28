#include "Menu.h"

#define STB_IMAGE_STATIC
#include "stb_image.h"

namespace Gfx {

Menu::Menu(const std::vector<MenuItem>& aItems)
: iItems(aItems)
{
  // Menu selection icon
  int width, height;
  int nrChannels;
  unsigned char *skullData = stbi_load("../resources/textures/skull.png", &width, &height, &nrChannels, 0);
  iTexture;
  glGenTextures(1, &iTexture);
  glBindTexture(GL_TEXTURE_2D, iTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, skullData);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(skullData);

  // Skull shader
  

}

Menu::MenuAction Menu::Show()
{
  if (iItems.empty())
  {
    return MenuAction::ACTION_RETURN_TO_GAME;
  }
	  
  for(const auto& item : iItems)
  {

  }
}



}
