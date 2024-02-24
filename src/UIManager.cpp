#include "UIManager.h"
#include "imgui.h"






UIManager::UIManager()
{
    const Texture* pacman = Texture::LoadShared(ASSET_DIRECTORY "pacman.png");
    pacmanSpriteID = (void*)(intptr_t)pacman->getTextureID();
    const Texture* pacmanlogo = Texture::LoadShared(ASSET_DIRECTORY "pacmanlogo.png");
    pacmanLogoID = (void*)(intptr_t)pacmanlogo->getTextureID();
}
UIManager::~UIManager()
{

}
void UIManager::showMainMenu()
{
    ImGui::SetNextWindowPos(ImVec2(width/2, height*0.15), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(1140 * 0.5, 268 * 0.5), ImGuiCond_FirstUseEver);
    ImGui::Begin("Start Game",nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove);
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImGui::Image(pacmanLogoID, ImVec2(1140 * 0.5, 268 * 0.5));
   
    // Calculate position for the button to be centered
    ImVec2 textSize = ImGui::CalcTextSize("Start");
    int elementPos = windowSize.x*0.5 - textSize.x * 0.5f;
    ImGui::SetCursorPosX(elementPos);
    if (ImGui::Button("Start"))
    {
        changeState(GameState::Playing);
    }
    ImGui::End();
}
void UIManager::showHUD(float dtime)
{  
    ImGui::Begin("HUD", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);
    ImGui::Text("Score: %d", pacman->getScore());
    ImGui::Text("fps: %f", 1.0f / dtime);
    ImGui::End();
    
    ImGui::Begin("Lifes", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing); 
    int scaledWidth, scaledHeight;
    scaleImage(50, 50, scaledWidth, scaledHeight);

    for (int i = 0; i < pacman->getLives(); i++)
    {
        ImGui::Image(pacmanSpriteID, ImVec2(scaledWidth, scaledHeight));
        ImGui::SameLine();
    }
    ImGui::End();
}

void UIManager::showGameOverMenu()
{

}
void UIManager::showPauseMenu()
{
   /* ImGui::SetNextWindowPos(ImVec2(width / 2, height * 0.15));
    ImGui::SetNextWindowSize(ImVec2(1024 * 0.5, 268 * 0.5));
    ImGui::Begin("Pause", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);
    ImVec2 windowSize = ImGui::GetWindowSize();

    // Calculate position for the button to be centered
    ImVec2 textSize = ImGui::CalcTextSize("Continue");
    int elementPos = windowSize.x * 0.5 - textSize.x * 0.5f;
    ImGui::SetCursorPosX(elementPos);
    if (ImGui::Button("Continue"))
    {
        changeState(GameState::Paused);
    }
    ImGui::End();*/
}
void UIManager::update(float dtime)
{

}
void UIManager::registerPacman(Pacman* pacman)
{
    this->pacman = pacman;
}

void UIManager::setWindowSize(int w, int h)
{
    this->width = w;
	this->height = h;
}

void UIManager::scaleImage(float maxWidth, float maxHeight, int& scaledWidth, int& scaledHeight)
{
    float scaleWidth = maxWidth / width;
    float scaleHeight = maxHeight / height;

    // Choose the smaller scale factor to maintain aspect ratio
    float scaleFactor = std::min(scaleWidth, scaleHeight);

    // Calculate the scaled dimensions
     scaledWidth = (int)(width * scaleFactor);
     scaledHeight = (int)(height * scaleFactor);
}
void UIManager::changeState(GameState newState) {
    switch (newState) {
    case GameState::MainMenu:
        std::cout << "Switching to Main Menu state" << std::endl;
        // Code to handle transition to Main Menu state
        break;
    case GameState::Playing:
        std::cout << "Switching to Playing state" << std::endl;
        currentState = GameState::Playing;
        break;
    case GameState::Paused:
        std::cout << "Switching to Paused state" << std::endl;
        // Code to handle transition to Paused state
        break;
    case GameState::GameOver:
        std::cout << "Switching to Game Over state" << std::endl;
        // Code to handle transition to Game Over state
        break;
    }
}
