//
//  TriangleBoxModel.hpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef TriangleBoxModel_hpp
#define TriangleBoxModel_hpp

#include <stdio.h>

#include <stdio.h>
#include "basemodel.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

struct BoxInstanceData {
    Vector Position;
    Matrix Rotation;
};
class TriangleBoxModel : public BaseModel
{
public:
    TriangleBoxModel(float Width=1, float Height=1, float Depth=1 );
    virtual ~TriangleBoxModel() {}
    virtual void draw(const BaseCamera& Cam);
    VertexBuffer getVB();
    IndexBuffer getIB();
    void setInstanceData(const std::vector<BoxInstanceData>& data);
    void pupulateBuffers();
    std::vector<BoxInstanceData> InstanceData;
protected:
    VertexBuffer VB;
    IndexBuffer IB;
    
    float Width;
    float Height;
        float Depth;
};


#endif /* TriangleBoxModel_hpp */
