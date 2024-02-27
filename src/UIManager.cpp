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
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::Begin("Start Game", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoMove);

    ImGui::SetCursorPosX((ImGui::GetWindowWidth()*0.5 - 1140 * 0.5));
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.2);
    ImGui::Image(pacmanLogoID, ImVec2(1140 , 268 ));

    // Center the button below
    ImVec2 textSize = ImGui::CalcTextSize("Start");
    ImGui::SetCursorPosX((ImGui::GetWindowWidth()*0.5-textSize.x*0.5) );
    ImGui::SetCursorPosY(ImGui::GetWindowHeight()*0.5 );

    if (ImGui::Button("Start"))
    {
        changeState(GameState::Playing);
    }

    ImGui::End();
}
void UIManager::showHUD(float dtime)
{  
    
    time += dtime;
    ticks++;
    if (time > 1.0f)
	{
        avgFPS = 1/(time / ticks);
		time = 0;
		ticks = 0;
	}
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::Begin("HUD", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);
    
    ImVec2 textSize = ImGui::CalcTextSize("Score: 10000");
    ImGui::SetCursorPosX(ImGui::GetWindowWidth()- textSize.x*1.5);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.1);

    ImGui::Text("Score: %d", pacman->getScore());
    
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - textSize.x * 1.5);
    ImGui::Text("fps: %d",(int)avgFPS);
    ImGui::End();
    
    ImGui::Begin("Lifes", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing); 
   
    int scaledWidth, scaledHeight;
    scaleImage(100, 100, scaledWidth, scaledHeight);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5 + 300);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.85);
    for (int i = 0; i < pacman->getLives(); i++)
    {
       
        ImGui::Image(pacmanSpriteID, ImVec2(scaledWidth, scaledHeight));
        ImGui::SameLine();
    }
    ImGui::End();
}

void UIManager::showGameOverMenu()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::Begin("GameOver", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);
    
    ImVec2 textSize = ImGui::CalcTextSize("GAME OVER");
    ImGui::SetCursorPosX(ImGui::GetWindowWidth()*0.5 - textSize.x * 0.5);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.5);
    ImGui::Text("GAME OVER");
    textSize = ImGui::CalcTextSize("FINAL SCORE: 10000");
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5 - textSize.x * 0.5);
    ImGui::Text("FINAL SCORE: %d", pacman->getScore());
    ImGui::End();
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

     scaledWidth = (int)(width * scaleFactor);
     scaledHeight = (int)(height * scaleFactor);
}
void UIManager::changeState(GameState newState) {
    switch (newState) {
    case GameState::MainMenu:
        std::cout << "Switching to Main Menu state" << std::endl;
       
        break;
    case GameState::Playing:
        std::cout << "Switching to Playing state" << std::endl;
        currentState = GameState::Playing;
        break;
    case GameState::Paused:
        std::cout << "Switching to Paused state" << std::endl;
     
        break;
    case GameState::GameOver:
        std::cout << "Switching to Game Over state" << std::endl;
      
        break;
    }
}
