#ifdef WIN32
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#include "imgui_impl_opengl3_loader.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <stdio.h>
#include "Application.h"
#include "freeimage.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
#define EPSILON 1e-6

void PrintOpenGLVersion();
void configureImGuiStyle();

float rectangleVertices[] =
{
    // Coords    // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

int main() {
    FreeImage_Initialise();
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    const int WindowWidth = 800;
    const int WindowHeight = 600;

    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Computergrafik - Hochschule Osnabrück", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: can not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

#if WIN32
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    PrintOpenGLVersion();

    //############################
    //added the code here
    glViewport(0, 0, WindowWidth, WindowHeight);

    //generate base shader

    //frambuffer shader
    BaseShader* postProcessShader = new BaseShader();
    postProcessShader->load(ASSET_DIRECTORY "postProcess.vert", ASSET_DIRECTORY "postProcess.frag");
    glUseProgram(postProcessShader->getProgramID());
    //############################



    Application App(window);
    App.start(); //takes care of openGL setup

    //###########################
     // imgui Framebuffer
    GLuint imguiFBO, imguiTexture;
    glGenFramebuffers(1, &imguiFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, imguiFBO);

    // Create a texture to hold the ImGui rendering
    glGenTextures(1, &imguiTexture);
    glBindTexture(GL_TEXTURE_2D, imguiTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, imguiTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Error: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer to avoid rendering anything else to it accidentally
    //############################

    //frambuffer rectangle
    unsigned int rectVAO, rectVBO;

    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Create Frame Buffer Object
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create Framebuffer Texture
    unsigned int framebufferTexture;
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    // Create Render Buffer Object
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowWidth, WindowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    configureImGuiStyle();
   

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    while (!glfwWindowShouldClose(window)) {
        // once per frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Bind the custom framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Enable depth testing since it's disabled when drawing the framebuffer rectangle
        glEnable(GL_DEPTH_TEST);

        glfwPollEvents();
        App.update();
        App.draw();





        // Instead of rendering ImGui directly to the screen, render it to the framebuffer
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();

        // Bind the ImGui framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, imguiFBO);
        glViewport(0, 0, WindowWidth, WindowHeight);
        glClearColor(0, 0, 0, 0); // Clear with transparent color
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // Render ImGui draw data to the framebuffer
        ImGui_ImplOpenGL3_RenderDrawData(draw_data);

        // Unbind the ImGui framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //we have now rendered the scene to the framebuffer and imgui to another framebuffer
        // we can now render the framebuffers to the screen

        // Bind the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Draw the framebuffer rectangle
        glUseProgram(postProcessShader->getProgramID()); //activate framebuffer shader
        glBindVertexArray(rectVAO);
        glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        // Set the sampler uniform to use texture unit 0
        glUniform1i(glGetUniformLocation(postProcessShader->getProgramID(), "screenTexture"), 0);
        // Activate the second texture unit and bind the ImGui texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, imguiTexture);
        // Set the sampler uniform to use texture unit 1
        glUniform1i(glGetUniformLocation(postProcessShader->getProgramID(), "uiTexture"), 1);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
    }
    App.end();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteProgram(postProcessShader->getProgramID());
    glDeleteFramebuffers(1, &FBO);
    glfwTerminate();
    return 0;
}

void configureImGuiStyle()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    io.Fonts->AddFontFromFileTTF(ASSET_DIRECTORY "pacmanfont.ttf", 24.0f);
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(1, 1, 1, 0.7f);
    style.FrameBorderSize = 2.0f;
    style.FramePadding = ImVec2(50.0f, 25.0f);
}
void PrintOpenGLVersion()
{
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
}