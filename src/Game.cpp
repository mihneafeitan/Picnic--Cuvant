#include "Game.h"
#include <iostream>
#include <fstream>

Game::Game()
    : currentLevel(1), score(0) {
    // Poți încărca lista de cuvinte sau aloca nivele
    std::cout << "[Game] Constructor apelat.\n";
}

void Game::start() {
    std::cout << "[Game] Joc pornit.\n";
    startLevel(currentLevel);
}

void Game::startLevel(int level) {
    currentLevel = level;
    std::cout << "[Game] Pornim nivelul " << currentLevel << "...\n";
    // aici poți adăuga cod pentru a inițializa obiectul Level
}

void Game::update() {
    // update logic
}

void Game::render() {
    // render logic
}

void Game::showIntro() {
    std::cout << "[Game] Bine ai venit la Picnic cu Cuvant!\n";
}

void Game::showGameOver() {
    std::cout << "[Game] Joc terminat. Scor final: " << score << "\n";
}

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
    std::cout << "[SaveSystem] Progres încărcat cu succes.\n";
    return true;
}
