#include "Skybox.h"
#include <FreeImage.h>
#include "vertexbuffer.h"
#include "indexbuffer.h"
#define _USE_MATH_DEFINES
#include <math.h>
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
static float toRad(float deg) { return deg * M_PI / 180.0f; }
//angeleht an die  rescourcen von https://learnopengl.com/Advanced-OpenGL/Cubemaps
float skyboxVertices[] =
{
    //   Coordinates
    -1.0f, -1.0f,  1.0f,//        7--------6
     1.0f, -1.0f,  1.0f,//       /|       /|
     1.0f, -1.0f, -1.0f,//      4--------5 |
    -1.0f, -1.0f, -1.0f,//      | |      | |
    -1.0f,  1.0f,  1.0f,//      | 3------|-2
     1.0f,  1.0f,  1.0f,//      |/       |/
     1.0f,  1.0f, -1.0f,//      0--------1
    -1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
    // Right
    1, 5, 6,
    6, 2, 1,
    // Left
    0, 7, 4,
    7, 0, 3,
    // Top
    4, 7, 6,
    6, 5, 4,
    // Bottom
    0, 1, 2,
    2, 3, 0,
    // Back
    0, 4, 5,
    5, 1, 0,
    // Front
    3, 2, 6,
    6, 7, 3
};

Skybox::Skybox()
{
    setupSkybox();
    pSkyboxModel = new TriangleBoxModel(1, 1, 1);
}
void Skybox::setupSkybox() {
    pSkyboxShader = new BaseShader();
    pSkyboxShader->load(ASSET_DIRECTORY "/shaders/skybox.vert", ASSET_DIRECTORY "/shaders/skybox.frag");
    glUseProgram(pSkyboxShader->getProgramID());
    glUniform1i(glGetUniformLocation(pSkyboxShader->getProgramID(), "skybox"), 0);
    std::string facesCubemap[6] =
    {
        ASSET_DIRECTORY "/Skybox/px.png",
        ASSET_DIRECTORY "/Skybox/nx.png",
        ASSET_DIRECTORY "/Skybox/py.png",
        ASSET_DIRECTORY "/Skybox/nx.png",
        ASSET_DIRECTORY "/Skybox/pz.png",
        ASSET_DIRECTORY "/Skybox/nz.png",
       
    };

    
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //modifizirte texture load funktion basierend auf Texture.cpp für benutzung mit   GL_TEXTURE_CUBE_MAP
    for (unsigned int i = 0; i < 6; i++)
    {
       const char* Filename  = facesCubemap[i].c_str();
        FREE_IMAGE_FORMAT ImageFormat = FreeImage_GetFileType(Filename, 0);
        if (ImageFormat == FIF_UNKNOWN)
            ImageFormat = FreeImage_GetFIFFromFilename(Filename);

        if (ImageFormat == FIF_UNKNOWN)
        {
                    std::cout << "Warning: Unkown texture format: " << Filename << std::endl;
            return ;
        }

        FIBITMAP* pBitmap = FreeImage_Load(ImageFormat, Filename);

        if (pBitmap == NULL)
        {
                std::cout << "Warning: Unable to open texture image " << Filename << std::endl;
            return;
        }

        FREE_IMAGE_TYPE Type = FreeImage_GetImageType(pBitmap);
        assert(Type == FIT_BITMAP);

        unsigned int Width = FreeImage_GetWidth(pBitmap);
        unsigned int Height = FreeImage_GetHeight(pBitmap);
        unsigned int bpp = FreeImage_GetBPP(pBitmap);
        assert(bpp == 32 || bpp == 16 || bpp == 24);

        unsigned char* data = new unsigned char[Width * Height * 4];
        unsigned char* dataPtr = data - 1;

        if (data == NULL)
        {
            FreeImage_Unload(pBitmap);
            return ;
        }


        RGBQUAD c;
        for (unsigned int i = 0; i < Height; ++i)
            for (unsigned int j = 0; j < Width; ++j)
            {
                FreeImage_GetPixelColor(pBitmap, j, Height - i - 1, &c);
                *(++dataPtr) = c.rgbRed;
                *(++dataPtr) = c.rgbGreen;
                *(++dataPtr) = c.rgbBlue;
                if (bpp == 32)
                    *(++dataPtr) = c.rgbReserved;
                else
                    *(++dataPtr) = 255;
            }


        glTexImage2D
        (
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_RGBA, Width, Height, 0,GL_RGBA,GL_UNSIGNED_BYTE, data
        );

        FreeImage_Unload(pBitmap);
        delete[] data;
    }
}
void Skybox::draw(const BaseCamera& Cam)
{

    glDepthFunc(GL_LEQUAL);
   // glDisable(GL_CULL_FACE);
    //pSkyboxShader->activate(Cam);
    glUseProgram(pSkyboxShader->getProgramID());
    Matrix view, proj,m;
  //  m.rotationX(toRad(180)); 
    Matrix m_cam  = Cam.getViewMatrix();//remove translation
   // m_cam.invert();
     m_cam.m03 = 0;
     m_cam.m13 = 0;
     m_cam.m23 = 0;
    proj = Cam.getProjectionMatrix();
    glUniformMatrix4fv(glGetUniformLocation(pSkyboxShader->getProgramID(), "view"), 1, GL_FALSE, m_cam);
    glUniformMatrix4fv(glGetUniformLocation(pSkyboxShader->getProgramID(), "projection"), 1, GL_FALSE, proj);
   //VertexBuffer VB = pSkyboxModel->getVB();
   //IndexBuffer IB = pSkyboxModel->getIB();
   
   //VB.activate();
   //IB.activate();
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
   
  // IB.deactivate();
    //VB.deactivate();
     glBindVertexArray(0);
    
    glDepthFunc(GL_LESS);
}


