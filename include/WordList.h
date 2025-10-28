#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>

class WordList {
private:
    std::vector<std::string> words_;
    std::unordered_set<std::string> wordSet_; // pentru căutări rapide (lowercase)

    // utilitar privat
    static std::string toLower(const std::string &s);

public:
    // ctor cu parametru - initializează dintr-un fișier (numele fișierului)
    explicit WordList(const std::string &filename);

    // mutator/copy special: implementăm move ctor și move assign + destructor
    WordList(WordList&& other) noexcept;
    WordList& operator=(WordList&& other) noexcept;
    ~WordList();

    // (nu oferim copy ctor/assign intenționat pentru această clasă; doar move)

    // functionalități netriviale:
    // încarcă din fișier (public, poate fi apelat după ctor)
    bool loadFromFile(const std::string &filename);

    // returnează cuvinte care pot fi formate dintr-un set de litere (fără a folosi fiecare literă mai mult decât apare)
    std::vector<std::string> filterByLetters(const std::vector<char> &letters, size_t minLen = 2) const;

    // verifică existența unui cuvânt (case-insensitive)
    bool exists(const std::string &word) const;

    // getter simplu (const)
    const std::vector<std::string>& words() const { return words_; }

    friend std::ostream& operator<<(std::ostream& os, const WordList& wl);
};
