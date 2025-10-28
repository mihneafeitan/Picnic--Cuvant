#pragma once
#include <vector>
#include <string>
#include <iostream>

class Level {
private:
    int number_;                             // numărul nivelului
    std::vector<char> letters_;              // literele disponibile
    std::vector<std::string> targetWords_;   // cuvintele corecte din acest nivel
    std::vector<bool> found_;                // ce cuvinte au fost deja găsite

public:
    // 🔹 Constructori
    Level(int number, const std::vector<char> &letters, const std::vector<std::string> &targetWords);
    Level();

    // 🔹 Rule of Five (cerință POO)
    Level(const Level &other);
    Level &operator=(const Level &other);
    Level(Level &&other) noexcept;
    Level &operator=(Level &&other) noexcept;
    ~Level();

    // 🔹 Funcții utile
    bool tryWord(const std::string &word); // încearcă un cuvânt
    bool isComplete() const;                // toate cuvintele au fost găsite?
    void display() const;                   // afișează starea nivelului

    // 🔹 Operator <<
    friend std::ostream &operator<<(std::ostream &os, const Level &lvl);
};
