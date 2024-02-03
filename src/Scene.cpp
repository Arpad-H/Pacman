//
//  Scene.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 06.11.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "Scene.h"
#include <string.h>
#include "model.h"
#include <assert.h>

SceneNode::SceneNode() : m_pParent(NULL), m_pModel(NULL), m_Scaling(1,1,1)
{
    // TODO: Add your code
}

SceneNode::SceneNode( const std::string& Name, const Vector& Translation, const Vector& RotationAxis, const float RotationAngle, const Vector& Scale, SceneNode* pParent, Model* pModel)
{
    setName(Name);
    setLocalTransform( Translation, RotationAxis, RotationAngle);
    setScaling( Scale);
    setParent(pParent);
    setModel(pModel);
}

const Matrix& SceneNode::getLocalTransform() const
{
    return m_LocalTransform;
}

void SceneNode::setLocalTransform( const Vector& Translation, const Vector& RotationAxis, const float RotationAngle)
{
    // TODO: Add your code
}

void SceneNode::setLocalTransform( const Matrix& LocalTransform)
{
    m_LocalTransform = LocalTransform;
}

Matrix SceneNode::getGlobalTransform() const
{
    // TODO: Add your code
    return Matrix(); // dummy return
}

const SceneNode* SceneNode::getParent() const
{
    return m_pParent;
}

void SceneNode::setParent( SceneNode* pNode)
{
    // TODO: Add your code
}

const std::set<SceneNode*>& SceneNode::getChildren() const
{
    return m_Children;
}

void SceneNode::addChild(SceneNode* pChild)
{
    m_Children.insert(pChild);
}

void SceneNode::removeChild(SceneNode* pChild)
{
    m_Children.erase(pChild);
}

void SceneNode::setModel( Model* pModel)
{
    m_pModel = pModel;
}

const Model* SceneNode::getModel() const
{
    return m_pModel;
}

void SceneNode::setName( const std::string& Name)
{
    m_Name = Name;
}

const std::string& SceneNode::getName() const
{
    return m_Name;
}

const Vector& SceneNode::getScaling() const
{
    return m_Scaling;
}
void SceneNode::setScaling( const Vector& Scaling)
{
    m_Scaling = Scaling;
}

void SceneNode::draw(const BaseCamera& Cam)
{
    // TODO: Add your code
}


Scene::Scene()
{
    // TODO: Add your code    
}

Scene::~Scene()
{
    // TODO: Add your code
}

bool Scene::addSceneFile( const char* Scenefile)
{
    // TODO: Add your code
    return true;
}
void Scene::draw( const BaseCamera& Cam)
{
    // TODO: Add your code
}






































