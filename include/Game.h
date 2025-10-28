#pragma once
#include "WordList.h"
#include "Level.h"
#include <vector>
#include <string>
#include <memory>
#include <ostream>

class Game {
private:
    WordList wordList_;                    // mare: mutări/copie gestionate mai jos
    std::vector<Level> levels_;
    size_t currentLevelIndex_;

    // helper privat: generează un set de litere random (pe baza unei liste de cuvinte posibile)
    std::vector<char> makeLettersFromWord(const std::string &w, size_t extra = 0) const;

public:
    // toate cele 5 funcții membre speciale cerute:
    Game(const std::string &wordFile);                 // ctor cu parametru
    Game();                                            // default ctor
    Game(const Game& other);                           // copy ctor
    Game& operator=(const Game& other);                // copy assign
    Game(Game&& other) noexcept;                       // move ctor
    Game& operator=(Game&& other) noexcept;            // move assign
    ~Game();

    // funcții publice netriviale:
    // construiește nivelele (1..nLevels) pe baza dictionarului
    void buildLevels(int nLevels = 50);

    // procesează un input (un guess). Returnează:
    //  0 -> cuvântul NU se afla în listă
    //  1 -> cuvânt găsit pentru nivelul curent
    //  2 -> nivel complet (după găsirea cuvântului)
    int processGuess(const std::string &guess);

    // avansează la nivelul următor (dacă există)
    bool advanceLevel();

    // afișare
    void displayCurrentLevel() const;

    friend std::ostream& operator<<(std::ostream& os, const Game& g);
};
