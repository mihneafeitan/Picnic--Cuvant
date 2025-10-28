#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    // presupunem ca fisierul big_romanian_list.txt e în root
    const std::string dict = "big_romanian_list.txt";
    Game game(dict);
    game.buildLevels(50);

    // încercăm să citim "tastatura.txt" pentru input automat (doar exemple)
    std::ifstream fin("tastatura.txt");
    bool useFile = fin.good();
    std::string line;

    std::cout << "Joc: Picnic Cuvant (nivel 1..50)\n";

    while (true) {
        game.displayCurrentLevel();
        std::string guess;
        if (useFile) {
            if (!std::getline(fin, line)) break;
            guess = line;
            std::cout << "> " << guess << " (din tastatura.txt)\n";
        } else {
            std::cout << "Introduceti un cuvant: ";
            if (!std::getline(std::cin, guess)) break;
        }
        if (guess.empty()) continue;
        // procesare guess
        int res = game.processGuess(guess);
        if (res == 0) {
            std::cout << "Cuvantul nu se afla in lista\n";
        } else if (res == 1) {
            // reafișare nivel (parțial)
            game.displayCurrentLevel();
        } else if (res == 2) {
            // completat
            std::cout << "Felicitari, ai completat nivelul " 
                      << " " << /* id */ "" << "\n";
            // afisare nivel complet (pentru claritate)
            game.displayCurrentLevel();
            bool adv = game.advanceLevel();
            if (!adv) {
                std::cout << "Felicitari! Ai terminat toate nivelele.\n";
                break;
            } else {
                std::cout << "Se trece la nivelul urmator...\n";
            }
        } else {
            // eroare sau sfarsit
            break;
        }
    }

    std::cout << "Joc terminat.\n";
    return 0;
}
