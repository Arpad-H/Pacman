//
//  LineBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "LineBoxModel.h"

LineBoxModel::LineBoxModel(float Width, float Height, float Depth)
{
    VB.begin();

    VB.addVertex(Width / 2, Height, -Depth / 2);
    VB.addVertex(-Width / 2, Height, -Depth / 2);
    VB.addVertex(-Width / 2, Height, -Depth / 2);
    VB.addVertex(-Width / 2, Height, Depth / 2);
    VB.addVertex(-Width / 2, Height, Depth / 2);
    VB.addVertex(Width / 2, Height, Depth / 2);
    VB.addVertex(Width / 2, Height, Depth / 2);
    VB.addVertex(Width / 2, Height, -Depth / 2);


    VB.addVertex(Width / 2, 0, -Depth / 2);
    VB.addVertex(-Width / 2, 0, -Depth / 2);
    VB.addVertex(-Width / 2, 0, -Depth / 2);
    VB.addVertex(-Width / 2, 0, Depth / 2);
    VB.addVertex(-Width / 2, 0, Depth / 2);
    VB.addVertex(Width / 2, 0, Depth / 2);
    VB.addVertex(Width / 2, 0, Depth / 2);
    VB.addVertex(Width / 2, 0, -Depth / 2);

    //connecting lines
    VB.addVertex(Width / 2, Height, -Depth / 2);
    VB.addVertex(Width / 2, 0, -Depth / 2);

    VB.addVertex(-Width / 2, Height, -Depth / 2);
    VB.addVertex(-Width / 2, 0, -Depth / 2);

    VB.addVertex(Width / 2, Height, Depth / 2);
    VB.addVertex(Width / 2, 0, Depth / 2);

    VB.addVertex(-Width / 2, Height, Depth / 2);
    VB.addVertex(-Width / 2, 0, Depth / 2);


    VB.end();
}

void LineBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    VB.activate();

    glDrawArrays(GL_LINES, 0, VB.vertexCount());

    VB.deactivate();
}
