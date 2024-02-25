//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "ghost.h"
#include "scene.h"
#include "pacman.h"
#include "GlowShader.h"
#include <chrono>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <imgui.h>
#include "rgbimage.h"



#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
#define EPSILON 1e-6

static float toRad(float deg) { return deg * M_PI / 180.0f; }

Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin)
{
    int width, height;
    glfwGetWindowSize(pWindow, &width, &height);
    BaseModel* pModel;
    ConstantShader* pConstShader;
    PhongShader* pPhongShader;
    
    skybox = new Skybox();
    
    level = Level(skybox->getCubemapTexture());
    level.loadLevel(levelDimX,levelDimY,levelSegments);

    //spawn the pacman
    pacman= new Pacman(ASSET_DIRECTORY "Pacman.dae",true,Vector(1,1,1));
    pacman->setLevel(&level);
    GameObjects.push_back(pacman);
    Models.push_back(pacman);
    pacman->registerCamera(&Cam);

    

    uiManager = new UIManager();
    uiManager->setWindowSize(width, height);
    uiManager->registerPacman(pacman);
   
}
void Application::start()
{
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    lastFrameTime= std::chrono::high_resolution_clock::now();
    time = 0;
   
}


void Application::update()
{
    if (currentState != GameState::Playing)
    {
        return;
    }
   
    // Calc Delta Time
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
    lastFrameTime = currentTime;
    time += deltaTime;
 
   if (!pacman->transitionState )//|| !pacman->hitState
    {
        if (glfwGetKey(pWindow, GLFW_KEY_W)) {
            dir = 0;
        }
        else if (glfwGetKey(pWindow, GLFW_KEY_S))
        {
            dir = 180;
        }
        else if (glfwGetKey(pWindow, GLFW_KEY_D)) {
            dir = 270;
        }
        else if (glfwGetKey(pWindow, GLFW_KEY_A)) {
            dir = 90;
        }


        double xpos, ypos;
        glfwGetCursorPos(pWindow, &xpos, &ypos);

        //Handle updating variables necessery for pacman
        pacman->steer(dir);
        //Update all game objects with the current delta time
        updateGameObjects(deltaTime);
        currentView = Cam.getViewMatrix();
    }
   
   Matrix view;
   Matrix pc = pacman->pacmanModel->transform();
  Vector pos = pc.translation() + level.forwardFacingFace->faceModel->transform().translation()*0.4 + pc.up().toUnitVector() * 16;
   //Vector pos = pc.translation() + level.forwardFacingFace->faceModel->transform().translation() * 0.4 + pc.up().toUnitVector() * 50; // Increased from 16 to 20

   view.lookAt(pc.translation(), pc.up(), pos);
   
   if (pacman->transitionState)
    {
        transitionTime += deltaTime;
        float  t = std::min(1.0f, std::max(0.0f, transitionTime));
        Matrix lerpedView = Matrix::lerp(currentView, view, t);
        //lerpedView.print();
        Cam.setViewMatrix(lerpedView);
        if (transitionTime >= 1.0f)
        {
            pacman->transitionState = false;
            transitionTime = 0;
        }
    }
    else
    {
        Cam.setViewMatrix(view);
    }
   if (pacman->hitState)
   {
       staggeredTime += deltaTime;
       if (staggeredTime >= 1.0f)
       {
           pacman->hitState = false;
           staggeredTime = 0;
       }
   }
   uiManager->showHUD(deltaTime);
}

void Application::updateGameObjects(float deltaTime) {
    for (GameObjectList::iterator it = GameObjects.begin(); it != GameObjects.end(); ++it) {
        (*it)->update(deltaTime);
    }
    level.update(deltaTime);
}
void Application::draw()
{
   
    switch (currentState)
    {
    case GameState::MainMenu:
        uiManager->showMainMenu();
        skybox->draw(Cam);
        return;
    case GameState::Playing:
        break;
    case GameState::Paused:
        uiManager->showPauseMenu();
        break;
    case GameState::GameOver:
        uiManager->showGameOverMenu();
        return;
    default:
        break;
    }
    // 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. setup shaders and draw models
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
    {
        (*it)->draw(Cam);
    }
    level.draw(Cam);
    
   skybox->draw(Cam);
    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}
void Application::end()
{
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
        delete *it;
    
    Models.clear();
}



