#include "include/Game.h"
#include <iostream>
#include <string>

int main() {
    Game game("big_romanian_list.txt");
    game.buildLevels(10); // poți schimba în 50 dacă vrei toate nivelurile

    std::cout << "Bun venit la jocul Picnic Cuvant!\n";

    std::string input;
    bool running = true;

    while (running) {
        game.displayCurrentLevel();

        std::cout << "Introduceti un cuvant: ";
        std::cin >> input;

        game.processGuess(input);

        // 🔹 verificăm dacă nivelul curent este complet
        if (game.getCurrentLevel().isComplete()) {
            // afișăm felicitare și trecem la următorul nivel
            std::cout << "\nFelicitari, ai completat nivelul!\n";

            if (!game.advanceLevel()) {
                // nu mai sunt niveluri
                running = false;
                std::cout << "Ai terminat toate nivelurile!\n";
            }
        }
    }

    std::cout << "\nFelicitari! Ai terminat jocul!\n";
    return 0;
}
