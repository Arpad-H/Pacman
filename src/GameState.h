#pragma once
#ifndef GameState_hpp
#define GameState_hpp

enum class GameState {
    MainMenu,
    Playing,
    Paused,
    GameOver
};

extern GameState currentState;
#endif /* GameState_hpp */


