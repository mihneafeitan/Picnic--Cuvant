#include "Level.h"
#include <algorithm>
#include <cctype>

// 🔹 Constructor implicit
Level::Level() : number_(0) {}

// 🔹 Constructor cu parametri
Level::Level(int number, const std::vector<char> &letters, const std::vector<std::string> &targetWords)
    : number_(number), letters_(letters), targetWords_(targetWords), found_(targetWords.size(), false) {}

// 🔹 Constructor de copiere
Level::Level(const Level &other) = default;

// 🔹 Operator= de copiere
Level &Level::operator=(const Level &other) = default;

// 🔹 Constructor de mutare
Level::Level(Level &&other) noexcept = default;

// 🔹 Operator= de mutare
Level &Level::operator=(Level &&other) noexcept = default;

// 🔹 Destructor
Level::~Level() = default;

// 🔹 Încearcă să potrivească un cuvânt
bool Level::tryWord(const std::string &word) {
    std::string lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    for (size_t i = 0; i < targetWords_.size(); ++i) {
        std::string t = targetWords_[i];
        std::transform(t.begin(), t.end(), t.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (!found_[i] && t == lower) {
            found_[i] = true;
            return true;
        }
    }
    return false;
}

// 🔹 Verifică dacă toate cuvintele au fost găsite
bool Level::isComplete() const {
    return std::all_of(found_.begin(), found_.end(), [](bool f) { return f; });
}

// 🔹 Afișează nivelul curent
void Level::display() const {
    std::cout << "\nNivelul " << number_ << ":\n";
    std::cout << "Litere disponibile: ";
    for (char c : letters_) std::cout << (char)std::toupper(c) << " ";
    std::cout << "\nCuvinte:\n";

    for (size_t i = 0; i < targetWords_.size(); ++i) {
        if (found_[i])
            std::cout << i + 1 << ". " << targetWords_[i] << "\n";
        else
            std::cout << i + 1 << ". " << std::string(targetWords_[i].size(), '*') << "\n";
    }
    if (isComplete())
        std::cout << "Felicitari, ai completat nivelul " << number_ << "!\n";
}

// 🔹 Operator << pentru afișare simplificată
std::ostream &operator<<(std::ostream &os, const Level &lvl) {
    os << "Nivel " << lvl.number_ << " cu " << lvl.letters_.size() << " litere și "
       << lvl.targetWords_.size() << " cuvinte.";
    return os;
}
