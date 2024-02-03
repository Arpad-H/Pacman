//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "phongshader.h"
#include <list>

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{

}
Model::Model(const char* ModelFile, bool FitSize, Vector initScale) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
    bool ret = load(ModelFile,FitSize, initScale);
    if (!ret)
        throw std::exception();
}
Model::~Model()
{
    // TODO: Add your code (Exercise 3)
    deleteNodes(&RootNode);
}

void Model::deleteNodes(Node* pNode)
{
    if (!pNode)
        return;
    for (unsigned int i = 0; i < pNode->ChildCount; ++i)
        deleteNodes(&(pNode->Children[i]));
    if (pNode->ChildCount > 0)
        delete[] pNode->Children;
    if (pNode->MeshCount > 0)
        delete[] pNode->Meshes;
}

bool Model::load(const char* ModelFile, bool FitSize, Vector initScale)
{
    //if (FitSize == true) std::cout << "true" << std::endl;
    const aiScene* pScene = aiImportFile(ModelFile, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords);

    if (pScene == NULL || pScene->mNumMeshes <= 0)
        return false;

    Filepath = ModelFile;
    Path = Filepath;
    size_t pos = Filepath.rfind('/');
    if (pos == std::string::npos)  pos = Filepath.rfind('\\');
    if (pos != std::string::npos)   Path.resize(pos + 1);

    loadMeshes(pScene, FitSize,initScale);
    loadMaterials(pScene);
    loadNodes(pScene);

    return true;
}

void Model::loadMeshes(const aiScene* pScene, bool FitSize, Vector initScale)
{
    calcBoundingBox(pScene, BoundingBox);
    MeshCount = pScene->mNumMeshes;
    pMeshes = new Mesh[MeshCount];
    std::cout << MeshCount << std::endl;

    //Scene Besteht aus mehreren meshes und jedes muss asu der datei geladen werden
    for (size_t i = 0; i < MeshCount; i++) {


        aiMesh* aiMesh = pScene->mMeshes[i];
        Mesh& mesh = this->pMeshes[i];
        mesh.MaterialIdx = aiMesh->mMaterialIndex;


        //Für das i-te modell in der scene jeweils Vetrtex und indexbuffer füllen
        // Vertexbuffer befuellen
        mesh.VB.begin();
        int vertexCount = aiMesh->mNumVertices;

        for (size_t j = 0; j < vertexCount; j++) {

            if (aiMesh->HasNormals()) {
                aiVector3D normal = aiMesh->mNormals[j];
                mesh.VB.addNormal(normal.x, normal.y, normal.z);
            }

            //Unser Vertex Buffer kann bis zu 4 sets an Texturkoordinaten haben. Falls diese im model Vorhanden sind werden sie vollständigkeitshalber auch geladen.
            // Texcoord0 reicht aber um die Texturen für den Phong shader zu laden
            if (aiMesh->HasTextureCoords(0)) {
               // std::cout << "texture setzen" << std::endl;
                aiVector3D tcoor = aiMesh->mTextureCoords[0][j];
                // -tcoor.y weil die Texturcoordinate nicht oben links den Ursprung hat sondern in manchen anderen Conventionen ist der Ursprung unten Links
                //Daher muss die y coordinate negiert werden
                mesh.VB.addTexcoord0(tcoor.x, -tcoor.y);
            }
            if (aiMesh->HasTextureCoords(1)) {
                aiVector3D tcoor = aiMesh->mTextureCoords[1][j];
                // -tcoor.y weil die Texturcoordinate nicht oben links den Ursprung hat sondern in manchen anderen Conventionen ist der Ursprung unten Links
                //Daher muss die y coordinate negiert werden
                mesh.VB.addTexcoord1(tcoor.x, -tcoor.y);
            }
            if (aiMesh->HasTextureCoords(2)) {
                aiVector3D tcoor = aiMesh->mTextureCoords[2][j];
                // -tcoor.y weil die Texturcoordinate nicht oben links den Ursprung hat sondern in manchen anderen Conventionen ist der Ursprung unten Links
                //Daher muss die y coordinate negiert werden
                mesh.VB.addTexcoord2(tcoor.x, -tcoor.y);
            }
            if (aiMesh->HasTextureCoords(3)) {
                aiVector3D tcoor = aiMesh->mTextureCoords[3][j];
                // -tcoor.y weil die Texturcoordinate nicht oben links den Ursprung hat sondern in manchen anderen Conventionen ist der Ursprung unten Links
                //Daher muss die y coordinate negiert werden
                mesh.VB.addTexcoord3(tcoor.x, -tcoor.y);
            }
            //Vertexe müssen als letztes dem Buffer hinzugefügt werden
            if (aiMesh->HasPositions()) {
                aiVector3D vector = aiMesh->mVertices[j];
                mesh.VB.addVertex(vector.x, vector.y, vector.z);
            }
        }
        mesh.VB.end();

        // Indexbuffer befuellen
        mesh.IB.begin();
        int faceCount = aiMesh->mNumFaces;
        for (size_t j = 0; j < faceCount; j++) {
            aiFace face = aiMesh->mFaces[j];
            int indicesOfFace = face.mNumIndices;
            if (indicesOfFace < 3) continue;

            //Triangulate faces with more than 3 Vertexes
            //https://en.wikipedia.org/wiki/Polygon_triangulation
            //Very Basic Ear clipping Algorithm
            //Fails with more complex Polygons, especially ones with holes
            for (size_t k = 0; k < indicesOfFace - 2; k++) {
                mesh.IB.addIndex(face.mIndices[0]);
                mesh.IB.addIndex(face.mIndices[k + 1]);
                mesh.IB.addIndex(face.mIndices[k + 2]);
            }
        }
        mesh.IB.end();

        pMeshes[i] = mesh;

        //Skaliere einheitlich so, dass das Modell in eine Bounding Box 5x5x5 passt
        if (FitSize == true) {
            // Dimmensionen der Bounding Box
            float boundingBoxWidth = BoundingBox.Max.X - BoundingBox.Min.X;
            float boundingBoxHeight = BoundingBox.Max.Y - BoundingBox.Min.Y;
            float boundingBoxDepth = BoundingBox.Max.Z - BoundingBox.Min.Z;

            //SkalierungsFaktor auf jeder Achse berechnen
            float scaleX = initScale.X / boundingBoxWidth;
            float scaleY = initScale.Y / boundingBoxHeight;
            float scaleZ = initScale.Z / boundingBoxDepth;

            // Um eine einheitliche skalierung zu gewährleisten nehme den kleinsten von den Werten. 
            // so wird die längste seite genau in die box passen
            float minScale = std::min(std::min(scaleX, scaleY), scaleZ);

            Matrix m;
            m.scale(minScale);
            this->transform(m);
        }
    }
}

void Model::loadMaterials(const aiScene* pScene)
{
    if (pScene->HasMaterials()) {
        MaterialCount = pScene->mNumMaterials;
        pMaterials = new Material[MaterialCount];

        for (size_t i = 0; i < MaterialCount; i++) {
            aiMaterial* mtrl = pScene->mMaterials[i];
            Material material;


            aiColor3D color;
            mtrl->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            material.DiffColor = Color(color.r, color.g, color.b);

            mtrl->Get(AI_MATKEY_COLOR_SPECULAR, color);
            material.SpecColor = Color(color.r, color.g, color.b);

            mtrl->Get(AI_MATKEY_COLOR_AMBIENT, color);
            material.AmbColor = Color(color.r, color.g, color.b);


            aiString temp;
            mtrl->GetTexture(aiTextureType_DIFFUSE, 0, &temp);
            std::string p = Path + temp.data;
            material.DiffTex = Texture().LoadShared(p.c_str());

            pMaterials[i] = material;
        }
    }
}
void Model::calcBoundingBox(const aiScene* pScene, AABB& Box)
{
    //Hold min and max  bound then iterate over every vertex to which points "stick out the furthest in 3d space"
    aiVector3D minBound(FLT_MAX, FLT_MAX, FLT_MAX);
    aiVector3D maxBound(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (size_t i = 0; i < pScene->mNumMeshes; i++)
    {
        const aiMesh* mesh = pScene->mMeshes[i];

        for (size_t j = 0; j < mesh->mNumVertices; j++)
        {
            const aiVector3D& vertex = mesh->mVertices[j];

            minBound.x = std::min(minBound.x, vertex.x);
            minBound.y = std::min(minBound.y, vertex.y);
            minBound.z = std::min(minBound.z, vertex.z);

            maxBound.x = std::max(maxBound.x, vertex.x);
            maxBound.y = std::max(maxBound.y, vertex.y);
            maxBound.z = std::max(maxBound.z, vertex.z);
        }
    }

    // Build Bounding Box
    Box = AABB(minBound.x, minBound.y, minBound.z, maxBound.x, maxBound.y, maxBound.z);
}

void Model::loadNodes(const aiScene* pScene)
{
    deleteNodes(&RootNode);
    copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
    pNode->Name = paiNode->mName.C_Str();
    pNode->Trans = convert(paiNode->mTransformation);

    if (paiNode->mNumMeshes > 0)
    {
        pNode->MeshCount = paiNode->mNumMeshes;
        pNode->Meshes = new int[pNode->MeshCount];
        for (unsigned int i = 0; i < pNode->MeshCount; ++i)
            pNode->Meshes[i] = (int)paiNode->mMeshes[i];
    }

    if (paiNode->mNumChildren <= 0)
        return;

    pNode->ChildCount = paiNode->mNumChildren;
    pNode->Children = new Node[pNode->ChildCount];
    for (unsigned int i = 0; i < paiNode->mNumChildren; ++i)
    {
        copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
        pNode->Children[i].Parent = pNode;
    }
}

void Model::applyMaterial(unsigned int index)
{
    if (index >= MaterialCount)
        return;

    PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
    if (!pPhong) {
        std::cout << "Model::applyMaterial(): WARNING Invalid shader-type. Please apply PhongShader for rendering models.\n";
        return;
    }

    Material* pMat = &pMaterials[index];
    pPhong->ambientColor(pMat->AmbColor);
    pPhong->diffuseColor(pMat->DiffColor);
    pPhong->specularExp(pMat->SpecExp);
    pPhong->specularColor(pMat->SpecColor);
    pPhong->diffuseTexture(pMat->DiffTex);
}

void Model::draw(const BaseCamera& Cam)
{
    if (!pShader) {
        std::cout << "BaseModel::draw() no shader found" << std::endl;
        return;
    }
    pShader->modelTransform(transform());

    std::list<Node*> DrawNodes;
    DrawNodes.push_back(&RootNode);

    while (!DrawNodes.empty())
    {
        Node* pNode = DrawNodes.front();
        Matrix GlobalTransform;

        if (pNode->Parent != NULL)
            pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
        else
            pNode->GlobalTrans = transform() * pNode->Trans;

        pShader->modelTransform(pNode->GlobalTrans);

        for (unsigned int i = 0; i < pNode->MeshCount; ++i)
        {
            Mesh& mesh = pMeshes[pNode->Meshes[i]];
            mesh.VB.activate();
            mesh.IB.activate();
            applyMaterial(mesh.MaterialIdx);
            pShader->activate(Cam);
            glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
            mesh.IB.deactivate();
            mesh.VB.deactivate();
        }
        for (unsigned int i = 0; i < pNode->ChildCount; ++i)
            DrawNodes.push_back(&(pNode->Children[i]));

        DrawNodes.pop_front();
    }
}

Matrix Model::convert(const aiMatrix4x4& m)
{
    return Matrix(m.a1, m.a2, m.a3, m.a4,
        m.b1, m.b2, m.b3, m.b4,
        m.c1, m.c2, m.c3, m.c4,
        m.d1, m.d2, m.d3, m.d4);
}



