#include "Camera.h"

namespace Gfx {

Camera::Camera()
{
  iCameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
  iCameraNormal = glm::vec3(0.0f, 0.0f, 1.0f);
  iCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Translate(float aStep)
{
  iCameraPos = iCameraPos + aStep*iCameraNormal;
}

void Camera::Rotate(float aDegrees)
{
  glm::mat4 rotationMatrix = glm::mat4(1.0f);	
  rotationMatrix = glm::rotate(rotationMatrix, glm::radians(aDegrees), iCameraUp);
  iCameraNormal = glm::vec3(rotationMatrix * glm::vec4(iCameraNormal, 1.0f));
}

void Camera::ConfigureShader(Gfx::Shader& aShader, unsigned int aScreenWidth, unsigned int aScreenHeight)
{
  glm::mat4 view =  glm::lookAt(iCameraPos, iCameraPos + iCameraNormal, iCameraUp);
  // note that we're translating the scene in the reverse direction of where we want to move
  // view = glm::translate(view, glm::vec3(0.0f, 0.0f, translate)); 
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), (float) aScreenWidth / (float) aScreenHeight, 0.05f, 100.0f);
  aShader.SetMatrix("projection", projection);
  aShader.SetMatrix("view", view);
}

glm::vec3 Camera::GetPosition() const
{
  return iCameraPos;
}

}
