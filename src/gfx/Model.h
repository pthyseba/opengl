#ifndef GFX_MODEL_H
#define GFX_MODEL_H

#include "Light.h"
#include "Mesh.h"
#include "Script.h"
#include "Shader.h"
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
namespace Gfx {

class Model
{
  public: 
    virtual ~Model()
    {
      delete [] iDepthMapFBO;
      delete [] iDepthMap;
    }
    Model()
    {
      iDim = 1.0f;
      iShader = std::make_unique<Gfx::Shader>();
     
      iDepthMapFBO = new GLuint[10];
      iDepthMap = new GLuint[10]; 
      static constexpr const char* KSimpleShadowVertexShader =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "uniform mat4 view;\n"
      "uniform mat4 projection;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = projection*view*vec4(aPos, 1.0f);\n"
      "}\0"; 
      static constexpr const char* KSimpleShadowFragmentShader =
      "#version 330 core\n"
      "void main()\n"
      "{\n"
      "}\0";
      
      iShadowShader = std::make_unique<Gfx::Shader>(KSimpleShadowVertexShader, KSimpleShadowFragmentShader);
      SetupShadowMap();
    }

    void SetProjection(glm::mat4& aProjection)
    {
      iShader->Activate();
      iShader->SetMatrix("projection", aProjection);
    }

    void SetView(glm::mat4& aView)
    {
      iShader->Activate();
      iShader->SetMatrix("view", aView);
    }

    virtual void Load() = 0;
    virtual bool InModel(glm::vec3 aPosition) = 0;
   
    void SetupShadowMap()
    {
      glGenFramebuffers(10, iDepthMapFBO);  
      const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
      glGenTextures(10, iDepthMap);
      for(int i = 0; i < 10; ++i)
      {
        glBindTexture(GL_TEXTURE_2D, iDepthMap[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                   SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindFramebuffer(GL_FRAMEBUFFER, iDepthMapFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, iDepthMap[i], 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
      }
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Draw() const
    {
      iShader->Activate();
      iShader->SetFloat("dim", iDim);

      SetupLights();

      // Render scene
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
      iShader->Activate();
      for (const auto& mesh : iMeshes)
      {
	mesh.Draw(*iShader);
      }
    }

    Mesh* GetMesh(const std::string& aName);
    Light* GetLight(const std::string& aName);

    const std::vector<Script>& GetScripts() const
    {
      return iScripts;
    }

    void DumpNormals();

    void SetDim(float aDim)
    {
      iDim = aDim;
    }

  private:
    void GenerateShadowMap(unsigned int aIndex, glm::vec3 aLightPos) const
    {
      const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
      GLint currentViewport[4];
      glGetIntegerv(GL_VIEWPORT, currentViewport);
      glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
      
      glBindFramebuffer(GL_FRAMEBUFFER, iDepthMapFBO[aIndex]);
      glClear(GL_DEPTH_BUFFER_BIT);

      iShadowShader->Activate();
      // Setup projection matrix and view matrix
      float near_plane = 0.01f, far_plane = 7.5f;
      glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
      iShadowShader->SetMatrix("projection", lightProjection);
      glm::mat4 lightView = glm::lookAt(aLightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));        
      iShadowShader->SetMatrix("view", lightView);
      for (const auto& mesh : iMeshes)
      {
        mesh.PureDraw();
      }
      
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(currentViewport[0], currentViewport[1], currentViewport[2], currentViewport[3]);  
      iShader->Activate();
      std::stringstream s;
      s << "lightspacematrix[" << aIndex << "]";
      iShader->SetMatrix(s.str().c_str(), lightProjection*lightView);
    }

    void SetupLights() const
    {
       iShader->Activate();
       int lights = 0;
       for(const auto& light : iLights)
       { 
         if(light.second.IsEnabled())
         {
           const auto pos = light.second.GetPosition();
           std::stringstream s;
           s << "PointLights[" << lights << "].position";
           iShader->SetVec3(s.str().c_str(), pos.x, pos.y, pos.z);
	   s.str("");
	   s.clear();
	   s << "PointLights[" << lights << "].color";
	   const auto color = light.second.GetColor();
	   iShader->SetVec3(s.str().c_str(), color.x, color.y, color.z);
	   s.str("");
	   s.clear();
	   s << "PointLights[" << lights << "].intensity";
	   const auto intensity = light.second.GetIntensity();
	   iShader->SetFloat(s.str().c_str(), intensity);
           s.str("");
           s.clear();
	   const auto attConstant = light.second.GetAttenuationConstant();
           s << "PointLights[" << lights << "].attenuationConstant";
	   iShader->SetFloat(s.str().c_str(), attConstant);
	   s.str("");
           s.clear();
           const auto attLinear = light.second.GetAttenuationLinear();
           s << "PointLights[" << lights << "].attenuationLinear";
           iShader->SetFloat(s.str().c_str(), attLinear);
           s.str("");
           s.clear();
	   const auto attQuadratic = light.second.GetAttenuationQuadratic();
           s << "PointLights[" << lights << "].attenuationQuadratic";
           iShader->SetFloat(s.str().c_str(), attQuadratic);

	   // Generate shadow map and configure shader
	   GenerateShadowMap(lights, pos);
	   s.str("");
           s.clear();
           s << "PointLights[" << lights << "].shadowmap";
           iShader->SetInt(s.str().c_str(), lights);

	   lights++;
           if (lights == 10) // Max 10 lights supported
	     break;
     	 }
       }
       for(int i = 0; i < lights; ++i) 
       {
         glActiveTexture(GL_TEXTURE0+i);
         glBindTexture(GL_TEXTURE_2D, iDepthMap[i]);
         //std::cout << "Texture unit " << lights << " bound to depth map " << lights << std::endl;
       }
       iShader->SetInt("nrPointLights", lights);
       iShader->SetInt("lights", lights);
    }

  protected:
    unsigned int CreateTexture(const std::string& aFilename);
    std::unique_ptr<Shader> iShader;
    std::unique_ptr<Shader> iShadowShader;
    std::vector<Mesh> iMeshes;
    std::vector<Script> iScripts;
    std::map<std::string, Light> iLights;
    float iDim;
    GLuint* iDepthMapFBO;
    GLuint* iDepthMap;
};

}

#endif
