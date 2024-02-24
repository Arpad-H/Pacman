//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "TriangleBoxModel.h"


TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth)
{
    
    float x = Width / 2;
    float y = Height / 2;
    float z = Depth / 2;

    Vector A(x, -y, z);
    Vector B(-x, -y, z);
    Vector C(-x, -y, -z);
    Vector D(x, -y, -z);
    Vector E(x, y, z);
    Vector F(-x, y, z);
    Vector G(-x, y, -z);
    Vector H(x, y, -z);


    VB.begin();

    //Unterseite
    VB.addNormal(0, -1, 0);
    VB.addTexcoord0(0, 1);
    VB.addTexcoord0(1, 1);
    VB.addTexcoord0(1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(A);//0
    VB.addVertex(B);
    VB.addVertex(C);
    VB.addVertex(D);

    //Vorderseite
    VB.addNormal(0, 0, 1);
    VB.addTexcoord0(0, 1);
    VB.addTexcoord0(1, 1);
    VB.addTexcoord0(1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(A); //4
    VB.addVertex(B);
    VB.addVertex(F);
    VB.addVertex(E);
    //Rechte Seite
    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(1, 1);
    VB.addTexcoord0(0, 1);
    VB.addTexcoord0(1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(B); //8
    VB.addVertex(C);
    VB.addVertex(F);
    VB.addVertex(G);
    //Rückseite
    VB.addNormal(0, 0, 1);
    VB.addTexcoord0(1, 1);
    VB.addTexcoord0(0, 1);
    VB.addTexcoord0(1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(C);//12
    VB.addVertex(D);
    VB.addVertex(G);
    VB.addVertex(H);
    //Linke Seite
    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(0, 1);
    VB.addTexcoord0(1, 1);
    VB.addTexcoord0(1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(D);//16
    VB.addVertex(A);
    VB.addVertex(E);
    VB.addVertex(H);
    //Oben
    VB.addNormal(0, 1, 0);
    VB.addTexcoord0(0, 1);
    VB.addTexcoord0(1, 1);
    VB.addTexcoord0(1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(E);//20
    VB.addVertex(F);
    VB.addVertex(G);
    VB.addVertex(H);

    VB.end();

    /*
   G--------H
  /        /|
 /        / |
F--------E  |
|        |  |
|   C    |  D
|        | /
|        |/
B--------A


    */
    IB.begin();
    //Unterseite
    IB.addIndex(0); //A
    IB.addIndex(1); //B
    IB.addIndex(2); //C

    IB.addIndex(0); //A
    IB.addIndex(2); //D
    IB.addIndex(3); //C
    //Vorderseite a-b-f-e
    IB.addIndex(4); //A
    IB.addIndex(6); //B
    IB.addIndex(5); //F

    IB.addIndex(4); //A
    IB.addIndex(7); //E
    IB.addIndex(6); //F
    //RRechte Seite b-c-f-g
    IB.addIndex(8); //B
    IB.addIndex(11);
    IB.addIndex(9);

    IB.addIndex(8);
    IB.addIndex(10);
    IB.addIndex(11);
    //Rückseite  c-d-g-h
    IB.addIndex(12); //C
    IB.addIndex(14);
    IB.addIndex(15);

    IB.addIndex(12); //C
    IB.addIndex(15);
    IB.addIndex(13);

    /*
       G--------H
      /        /|
     /        / |
    F--------E  |
    |        |  |
    |   C    |  D
    |        | /
    |        |/
    B--------A*/

    //Linke Seite d-a-e-h
    IB.addIndex(16); //D
    IB.addIndex(19);
    IB.addIndex(18);

    IB.addIndex(16);
    IB.addIndex(18);
    IB.addIndex(17);
    //Oben e-f-g-h
    IB.addIndex(20); //E
    IB.addIndex(22);
    IB.addIndex(21);

    IB.addIndex(20);
    IB.addIndex(23);
    IB.addIndex(22);




    IB.end();
}

void TriangleBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    VB.activate();
    IB.activate();

    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

    IB.deactivate();
    VB.deactivate();
}

