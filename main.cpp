#include "Game.h"
#include <iostream>

int main() {
    std::cout << "=== PICNIC CUVANT ===\n";

    Game game;
    std::string playerName;

    std::cout << "Introdu numele jucatorului: ";
    std::getline(std::cin, playerName);

    game.loadProgress(playerName);

    int optiune = 0;
    while (true) {
        std::cout << "\n--- MENIU PRINCIPAL ---\n";
        std::cout << "1. Joaca nivelul 1\n";
        std::cout << "2. Joaca nivelul 2\n";
        std::cout << "3. Salveaza progresul\n";
        std::cout << "4. Iesire\n";
        std::cout << "Alege optiunea: ";

        std::cin >> optiune;

        if (optiune == 1)
            game.startLevel(1);
        else if (optiune == 2)
            game.startLevel(2);
        else if (optiune == 3) {
            std::cout << "\nSalvam progresul...\n";
            game.saveProgress();
            std::cout << "[SaveSystem] Progres salvat cu succes.\n";
        }
        else if (optiune == 4) {
            std::cout << "La revedere, " << playerName << "!\n";
            break;
        }
        else {
            std::cout << "Optiune invalida. Incearca din nou.\n";
        }
    }

    return 0;
}
