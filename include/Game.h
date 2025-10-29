#pragma once
#include <string>
#include <vector>
#include "Level.h"
#include "WordList.h"

class Game {
public:
    Game();

    // Inițializare și pornire joc
    void start();
    void startLevel(int level);

    // Funcționalități legate de joc
    void update();
    void render();

    // Sistem de salvare / încărcare progres
    bool saveProgress(const std::string& playerName, int level, int score);
    bool loadProgress(std::string& playerName, int& level, int& score);

private:
    int currentLevel;
    int score;
    WordList wordList;
    std::vector<Level> levels;

    void showIntro();
    void showGameOver();
};
