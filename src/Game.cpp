#include "Game.h"
#include <fstream>
#include <iostream>

bool Game::saveProgress(const std::string& playerName, int level, int score) {
    std::ofstream file("save.txt");
    if (!file.is_open()) {
        std::cerr << "[SaveSystem] Nu s-a putut deschide save.txt pentru scriere.\n";
        return false;
    }
    file << "PlayerName " << playerName << "\n";
    file << "Level " << level << "\n";
    file << "Score " << score << "\n";
    file.close();
    std::cout << "[SaveSystem] Progres salvat cu succes.\n";
    return true;
}

bool Game::loadProgress(std::string& playerName, int& level, int& score) {
    std::ifstream file("save.txt");
    if (!file.is_open()) {
        std::cerr << "[SaveSystem] Nu s-a putut deschide save.txt pentru citire.\n";
        return false;
    }

    std::string key;
    while (file >> key) {
        if (key == "PlayerName")
            file >> playerName;
        else if (key == "Level")
            file >> level;
        else if (key == "Score")
            file >> score;
    }

    file.close();
    std::cout << "[SaveSystem] Progres incarcat cu succes.\n";
    return true;
}
