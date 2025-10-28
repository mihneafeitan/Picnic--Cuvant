#include "Game.h"
#include <random>
#include <algorithm>
#include <iostream>

static std::string toLowerStr(const std::string &s) {
    std::string r; r.reserve(s.size());
    for (unsigned char c : s) r.push_back(static_cast<char>(std::tolower(c)));
    return r;
}

Game::Game(const std::string &wordFile)
    : wordList_(wordFile), currentLevelIndex_(0) {
    // ctor parametric
}

Game::Game() : wordList_(), currentLevelIndex_(0) {}

Game::Game(const Game& other)
    : wordList_(other.wordList_), levels_(other.levels_), currentLevelIndex_(other.currentLevelIndex_) {
    // copy ctor
}

Game& Game::operator=(const Game& other) {
    if (this != &other) {
        wordList_ = other.wordList_;
        levels_ = other.levels_;
        currentLevelIndex_ = other.currentLevelIndex_;
    }
    return *this;
}

Game::Game(Game&& other) noexcept
    : wordList_(std::move(other.wordList_)), levels_(std::move(other.levels_)), currentLevelIndex_(other.currentLevelIndex_) {
    other.currentLevelIndex_ = 0;
}

Game& Game::operator=(Game&& other) noexcept {
    if (this != &other) {
        wordList_ = std::move(other.wordList_);
        levels_ = std::move(other.levels_);
        currentLevelIndex_ = other.currentLevelIndex_;
        other.currentLevelIndex_ = 0;
    }
    return *this;
}

Game::~Game() {
    // destructor
}

std::vector<char> Game::makeLettersFromWord(const std::string &w, size_t extra) const {
    std::vector<char> letters;
    for (char c : w) letters.push_back(static_cast<char>(std::tolower(c)));
    // adăugăm extra litere random din alfabet (a..z)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist('a', 'z');
    for (size_t i = 0; i < extra; ++i) letters.push_back(static_cast<char>(dist(gen)));
    // amestecăm
    std::shuffle(letters.begin(), letters.end(), gen);
    return letters;
}

void Game::buildLevels(int nLevels) {
    levels_.clear();
    // Strategie simplă: pentru fiecare nivel, selectăm un cuvânt destul de scurt din listă
    // și construim un set de litere pe baza lui + câteva extra astfel încât să existe mai multe cuvinte
    auto &words = wordList_.words();
    if (words.empty()) return;
    std::mt19937 gen((unsigned)time(nullptr));
    std::uniform_int_distribution<size_t> idxDist(0, words.size() - 1);

    int created = 0;
    int attempts = 0;
    while (created < nLevels && attempts < nLevels * 20) {
        ++attempts;
        const std::string &seed = words[idxDist(gen)];
        if (seed.size() < 3 || seed.size() > 8) continue;
        size_t extra = std::min<size_t>(3, 8 - seed.size());
        auto letters = makeLettersFromWord(seed, extra);
        // generăm lista de candidate
        auto candidates = wordList_.filterByLetters(letters, 2);
        // păstrăm doar 2-4 cuvinte relevante
        if (candidates.size() >= 2 && candidates.size() <= 8) {
            // alegem 2..4 ținte unice (cele mai scurte pentru claritate)
            std::sort(candidates.begin(), candidates.end(), [](const std::string&a, const std::string&b){
                if (a.size() != b.size()) return a.size() < b.size();
                return a < b;
            });
            size_t pick = std::min<size_t>(4, std::max<size_t>(2, candidates.size()/2));
            std::vector<std::string> targets;
            for (size_t i = 0; i < pick; ++i) {
                targets.push_back(candidates[i]);
            }
            Level lvl(created+1, letters, targets);
            levels_.push_back(std::move(lvl));
            ++created;
        }
    }

    // Daca n-am creat destule nivele, generăm nivele triviale (folosind prima literă)
    for (int i = created; i < nLevels; ++i) {
        std::vector<char> letters = {'a','n','i'}; // fallback
        std::vector<std::string> targets = {"an","in"};
        levels_.emplace_back(i+1, letters, targets);
    }
}

int Game::processGuess(const std::string &guess) {
    if (levels_.empty() || currentLevelIndex_ >= levels_.size()) return -1;
    Level &lvl = levels_[currentLevelIndex_];
    // verificăm dacă cuvântul există în lista globală și e compatibil cu literele nivelului
    if (!wordList_.exists(guess)) {
        return 0; // "Cuvantul nu se afla in lista"
    }
    // dacă există ca termen general, verificăm dacă e target la acest nivel - folosim lvl.tryGuess
    bool found = lvl.tryGuess(guess);
    if (!found) {
        // cuvântul există în dicționar, dar nu e unul din target-urile curente
        return 0;
    } else {
        if (lvl.isComplete()) return 2;
        else return 1;
    }
}

bool Game::advanceLevel() {
    if (currentLevelIndex_ + 1 < levels_.size()) {
        ++currentLevelIndex_;
        return true;
    }
    return false;
}

void Game::displayCurrentLevel() const {
    if (levels_.empty() || currentLevelIndex_ >= levels_.size()) {
        std::cout << "Nu există nivel curent.\n";
        return;
    }
    levels_[currentLevelIndex_].display();
}

std::ostream& operator<<(std::ostream& os, const Game& g) {
    os << "Game[curLevel=" << g.currentLevelIndex_+1 << "/" << g.levels_.size() << "]";
    return os;
}
