#pragma once
#include "WordList.h"
#include "Level.h"
#include <vector>
#include <string>
#include <iostream>

class Game {
private:
    WordList wordList_;
    std::vector<Level> levels_;
    int currentLevelIndex_;

public:
    Game();
    explicit Game(const std::string &filename);

    Game(const Game &other);
    Game &operator=(const Game &other);

    Game(Game &&other) noexcept;
    Game &operator=(Game &&other) noexcept;

    ~Game();

    void buildLevels(int numberOfLevels);
    void displayCurrentLevel() const;
    int processGuess(const std::string &guess);
    bool advanceLevel(); //  modificat să returneze bool
    Level &getCurrentLevel();

    friend std::ostream &operator<<(std::ostream &os, const Game &g);
};
