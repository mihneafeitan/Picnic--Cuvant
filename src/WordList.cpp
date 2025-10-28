#include "WordList.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <unordered_map>

// 🔹 Constructor implicit
WordList::WordList() = default;

// 🔹 Constructor cu fișier
WordList::WordList(const std::string &filename) {
    loadFromFile(filename);
}

// 🔹 Constructor de copiere
WordList::WordList(const WordList &other) = default;

// 🔹 Operator= de copiere
WordList &WordList::operator=(const WordList &other) = default;

// 🔹 Constructor de mutare
WordList::WordList(WordList &&other) noexcept = default;

// 🔹 Operator= de mutare
WordList &WordList::operator=(WordList &&other) noexcept = default;

// 🔹 Destructor
WordList::~WordList() = default;

// 🔹 Conversie la litere mici
std::string WordList::toLower(const std::string &s) {
    std::string r;
    r.reserve(s.size());
    for (char c : s)
        r.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    return r;
}

// 🔹 Încarcă cuvinte din fișier
bool WordList::loadFromFile(const std::string &filename) {
    std::ifstream fin(filename);
    if (!fin.is_open())
        return false;

    words_.clear();
    std::string word;
    while (fin >> word) {
        words_.push_back(toLower(word));
    }
    return true;
}

// 🔹 Adaugă cuvânt în listă
void WordList::addWord(const std::string &word) {
    words_.push_back(toLower(word));
}

// 🔹 Verifică dacă un cuvânt există în listă
bool WordList::contains(const std::string &word) const {
    std::string w = toLower(word);
    return std::find(words_.begin(), words_.end(), w) != words_.end();
}

// 🔹 Returnează cuvinte care se pot forma din anumite litere
std::vector<std::string> WordList::getWordsWithLetters(const std::vector<char> &letters, int maxWords) const {
    std::vector<std::string> results;

    for (const auto &word : words_) {
        if (results.size() >= static_cast<size_t>(maxWords))
            break;

        std::unordered_map<char, int> available;
        for (char c : letters)
            available[std::tolower(static_cast<unsigned char>(c))]++;

        bool ok = true;
        for (char c : word) {
            char lc = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            if (available[lc]-- <= 0) {
                ok = false;
                break;
            }
        }
        if (ok)
            results.push_back(word);
    }
    return results;
}

// 🔹 Operator << pentru afișare
std::ostream &operator<<(std::ostream &os, const WordList &wl) {
    os << "WordList (" << wl.words_.size() << " cuvinte):\n";
    for (const auto &w : wl.words_)
        os << " - " << w << '\n';
    return os;
}
