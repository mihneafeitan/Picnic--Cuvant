#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <random>
#include <ctime>
#include <fstream>

Game::Game() : wordList_("big_romanian_list.txt"), currentLevelIndex_(0) {}
Game::Game(const std::string &filename) : wordList_(filename), currentLevelIndex_(0) {}

Game::Game(const Game &other)
    : wordList_(other.wordList_), levels_(other.levels_), currentLevelIndex_(other.currentLevelIndex_) {}

Game &Game::operator=(const Game &other) {
    if (this != &other) {
        wordList_ = other.wordList_;
        levels_ = other.levels_;
        currentLevelIndex_ = other.currentLevelIndex_;
    }
    return *this;
}

Game::Game(Game &&other) noexcept
    : wordList_(std::move(other.wordList_)),
      levels_(std::move(other.levels_)),
      currentLevelIndex_(other.currentLevelIndex_) {}

Game &Game::operator=(Game &&other) noexcept {
    if (this != &other) {
        wordList_ = std::move(other.wordList_);
        levels_ = std::move(other.levels_);
        currentLevelIndex_ = other.currentLevelIndex_;
    }
    return *this;
}

Game::~Game() = default;


void Game::buildLevels(int numberOfLevels) {
    levels_.clear();

    // încercăm să încărcăm fișierul principal de cuvinte
    if (!wordList_.loadFromFile("../big_romanian_list.txt") &&
        !wordList_.loadFromFile("big_romanian_list.txt")) {
        std::cerr << "Eroare: nu s-a putut încărca fisierul big_romanian_list.txt!\n";
        return;
    }

    // încărcăm toate cuvintele posibile
    std::vector<char> allLetters;
    for (char c = 'a'; c <= 'z'; ++c) allLetters.push_back(c);
    auto allWords = wordList_.getWordsWithLetters(allLetters, 50000);

    if (allWords.empty()) {
        std::cerr << "Eroare: lista de cuvinte este goală!\n";
        return;
    }

    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> wordDist(0, static_cast<int>(allWords.size()) - 1);

    // 🔹 deschidem fișierul cheats.txt (îl suprascriem)
    std::ofstream fout("cheats.txt");
    if (!fout.is_open()) {
        std::cerr << "Eroare: nu pot crea fisierul cheats.txt!\n";
    }

    for (int level = 1; level <= numberOfLevels; ++level) {
        std::string baseWord = allWords[wordDist(rng)];

        // selectăm literele (fără duplicate)
        std::vector<char> letters;
        for (char c : baseWord) {
            char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            if (std::isalpha(upper) && std::find(letters.begin(), letters.end(), upper) == letters.end())
                letters.push_back(upper);
        }

        if (letters.empty()) {
            letters = {'A', 'N', 'A'};
            baseWord = "ana";
        }

        // generăm lista de cuvinte pentru aceste litere
        auto candidates = wordList_.getWordsWithLetters(letters, 5);
        if (candidates.empty())
            candidates.push_back(baseWord);

        // adăugăm nivelul în joc
        levels_.emplace_back(level, letters, candidates);

        // 🔹 scriem în fișier cheats.txt
        if (fout.is_open()) {
            fout << "Nivel " << level << ":\n";
            fout << "Litere: ";
            for (char c : letters) fout << c << " ";
            fout << "\nCuvinte: ";
            for (size_t i = 0; i < candidates.size(); ++i) {
                fout << candidates[i];
                if (i + 1 < candidates.size()) fout << ", ";
            }
            fout << "\n\n";
        }
    }

    // 🔹 închidem fișierul
    if (fout.is_open()) {
        fout.close();
        std::cout << "[INFO] cheats.txt a fost generat cu succes!\n";
    }
}



void Game::displayCurrentLevel() const {
    if (currentLevelIndex_ >= levels_.size()) {
        std::cout << "Ai terminat toate nivelurile! Felicitari!\n";
        return;
    }
    levels_[currentLevelIndex_].display();
}

int Game::processGuess(const std::string &guess) {
    if (currentLevelIndex_ >= levels_.size())
        return 0;

    std::string g = guess;
    std::transform(g.begin(), g.end(), g.begin(), ::tolower);

    if (!wordList_.contains(g)) {
        std::cout << "Cuvantul nu se afla in lista.\n";
        return 0;
    }

    bool found = levels_[currentLevelIndex_].tryWord(g);
    if (!found) {
        std::cout << "Cuvantul nu este printre cele cautate!\n";
        return 0;
    }

    std::cout << "Corect!\n";
    return 1;
}

// ✅ modificat să returneze bool
bool Game::advanceLevel() {
    if (currentLevelIndex_ + 1 < levels_.size()) {
        ++currentLevelIndex_;
        return true;
    } else {
        std::cout << "Ai terminat toate nivelurile!\n";
        return false;
    }
}

std::ostream &operator<<(std::ostream &os, const Game &g) {
    os << "Joc PicnicCuvant - nivel curent: " << g.currentLevelIndex_ + 1 << "/" << g.levels_.size() << "\n";
    return os;
}
Level &Game::getCurrentLevel() {
    return levels_[currentLevelIndex_];
}
