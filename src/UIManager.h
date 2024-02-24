#pragma once
#include "Pacman.h"
#include "imgui.h"
#include "GameState.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

class UIManager
{
	
public:
	UIManager();
	~UIManager();
 void showMainMenu();
 void showHUD(float dtime);
 void showPauseMenu();
 void showGameOverMenu();
 void update(float dtime);
 void registerPacman(Pacman* pacman);
 void setWindowSize(int w, int h);
 void changeState(GameState newState);
protected:
	int width;
	int height;
	Pacman* pacman;
	ImTextureID pacmanSpriteID;
	ImTextureID pacmanLogoID;
	void scaleImage(float maxWidth, float maxHeight, int& scaledWidth, int& scaledHeight);
};

