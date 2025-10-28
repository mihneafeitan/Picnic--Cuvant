#include "Level.h"
#include <algorithm>
#include <iostream>

static std::string toLowerStr(const std::string &s) {
    std::string r; r.reserve(s.size());
    for (unsigned char c : s) r.push_back(static_cast<char>(std::tolower(c)));
    return r;
}

std::vector<char> Level::toLowerLetters(const std::string &s) {
    std::vector<char> v;
    for (unsigned char c : s) v.push_back(static_cast<char>(std::tolower(c)));
    return v;
}

Level::Level(int id, const std::vector<char>& letters, const std::vector<std::string>& targets)
    : id_(id), letters_(letters), targets_(targets), found_(targets.size(), false) {
}

Level::Level(const Level &other)
    : id_(other.id_), letters_(other.letters_), targets_(other.targets_), found_(other.found_) {
    // copy ctor explicit
}

Level& Level::operator=(const Level &other) {
    if (this != &other) {
        id_ = other.id_;
        letters_ = other.letters_;
        targets_ = other.targets_;
        found_ = other.found_;
    }
    return *this;
}

Level::~Level() {
    // nimic special
}

void Level::display() const {
    std::cout << "Nivelul " << id_ << ":\nLitere disponibile: ";
    for (size_t i = 0; i < letters_.size(); ++i) {
        std::cout << static_cast<char>(std::toupper(letters_[i]));
        if (i+1 < letters_.size()) std::cout << ", ";
    }
    std::cout << "\nCuvinte:\n";
    for (size_t i = 0; i < targets_.size(); ++i) {
        std::cout << i+1 << ". ";
        if (found_[i]) std::cout << static_cast<std::string>(targets_[i]) << "\n";
        else {
            for (size_t k=0;k<targets_[i].size();++k) std::cout << "*";
            std::cout << "\n";
        }
    }
}

bool Level::tryGuess(const std::string &guess) {
    std::string g = toLowerStr(guess);
    for (size_t i = 0; i < targets_.size(); ++i) {
        if (!found_[i] && targets_[i] == g) {
            found_[i] = true;
            return true;
        }
    }
    return false;
}

bool Level::isComplete() const {
    for (bool f : found_) if (!f) return false;
    return true;
}

std::ostream& operator<<(std::ostream& os, const Level& lvl) {
    os << "Level[id=" << lvl.id_ << ", letters=";
    for (char c : lvl.letters_) os << static_cast<char>(std::toupper(c));
    os << ", targets=" << lvl.targets_.size() << "]";
    return os;
}
