//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "GameObject.h"
#include <chrono>
#include <algorithm>

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    typedef std::list<GameObject*> GameObjectList;
    Application(GLFWwindow* pWin);
    void start();
    void update();
    void updateGameObjects(float deltaTime);
    void draw();
    void end();
protected:
    Camera Cam;
    ModelList Models;
    GameObject* temp;
    GameObjectList GameObjects;
    GLFWwindow* pWindow;
    ;
   
    float time;
    std::chrono::high_resolution_clock::time_point lastFrameTime;

};

#endif /* Application_hpp */
