#pragma once
#include <string>
#include <vector>
#include <iostream>

class WordList {
private:
    std::vector<std::string> words_;   // lista tuturor cuvintelor încărcate din fișier

public:
    // 🔹 Constructori
    explicit WordList(const std::string &filename);
    WordList();

    // 🔹 Constructor de copiere
    WordList(const WordList &other);

    // 🔹 Operator= de copiere
    WordList &operator=(const WordList &other);

    // 🔹 Constructor de mutare
    WordList(WordList &&other) noexcept;

    // 🔹 Operator= de mutare
    WordList &operator=(WordList &&other) noexcept;

    // 🔹 Destructor
    ~WordList();

    // 🔹 Funcții utile
    bool loadFromFile(const std::string &filename);
    std::vector<std::string> getWordsWithLetters(const std::vector<char> &letters, int maxWords = 10) const;
    void addWord(const std::string &word);
    bool contains(const std::string &word) const;
    static std::string toLower(const std::string &s);

    // 🔹 Operator <<
    friend std::ostream &operator<<(std::ostream &os, const WordList &wl);
};
