#include <iostream>
#include <string>
#include "Game.h"

int main() {
    std::cout << "=== PICNIC CUVANT ===" << std::endl;

    Game game;

    std::string playerName;
    std::cout << "Introdu numele jucatorului: ";
    std::getline(std::cin, playerName);

    // Încercăm să încărcăm progresul jucătorului
    std::string savedName;
    int savedLevel = 0, savedScore = 0;

    if (game.loadProgress(savedName, savedLevel, savedScore) && savedName == playerName) {
        std::cout << "Progres gasit pentru " << playerName
                  << ": nivel " << savedLevel
                  << ", scor " << savedScore << ".\n";
    } else {
        std::cout << "Nu exista progres salvat pentru " << playerName
                  << ". Se incepe un joc nou.\n";
        savedLevel = 1;
        savedScore = 0;
    }

    // Pornește jocul de la nivelul salvat
    std::cout << "Pornind nivelul " << savedLevel << "...\n";
    game.startLevel(savedLevel);

    // Când jocul se încheie, salvăm progresul
    std::cout << "\nSalvam progresul...\n";
    game.saveProgress(playerName, savedLevel, savedScore);

    std::cout << "Progres salvat cu succes. La revedere, " << playerName << "!\n";
    return 0;
}
