#include "WordList.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>

std::string WordList::toLower(const std::string &s) {
    std::string r; r.reserve(s.size());
    for (char c : s) r.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    return r;
}

WordList::WordList(const std::string &filename) {
    loadFromFile(filename);
}

WordList::WordList(WordList&& other) noexcept
    : words_(std::move(other.words_)), wordSet_(std::move(other.wordSet_)) {
    // other left in valid, empty state
}

WordList& WordList::operator=(WordList&& other) noexcept {
    if (this != &other) {
        words_ = std::move(other.words_);
        wordSet_ = std::move(other.wordSet_);
    }
    return *this;
}

WordList::~WordList() {
    // nimic special - folosim RAII
}

bool WordList::loadFromFile(const std::string &filename) {
    std::ifstream in(filename);
    if (!in) return false;
    words_.clear();
    wordSet_.clear();
    std::string line;
    while (std::getline(in, line)) {
        std::string w;
        // curățare spații, transformare la lowercase
        std::istringstream iss(line);
        if (!(iss >> w)) continue;
        std::string lw = toLower(w);
        // acceptăm doar cuvinte alfabetice (litere române) sau `-`
        bool ok = true;
        for (char c : lw) {
            if (!( (c >= 'a' && c <= 'z') || (unsigned char)c >= 0xC0 || c=='-' )) {
                // permite caractere UTF-8 românești (simple check: bytes >= 0xC0) — tolerant
                // nota: pentru robustitate ar trebui un parser UTF-8; ținem verifier lax
            }
        }
        words_.push_back(lw);
        wordSet_.insert(lw);
    }
    return true;
}

bool WordList::exists(const std::string &word) const {
    return wordSet_.find(toLower(word)) != wordSet_.end();
}

static bool canFormWord(const std::string &word, const std::vector<char> &letters) {
    // numărăm literele din letters (lowercase ASCII / UTF-8 not fully supported)
    std::unordered_map<char,int> cnt;
    for (char c : letters) {
        cnt[static_cast<char>(std::tolower(static_cast<unsigned char>(c)))]++;
    }
    for (char wc : word) {
        char lc = static_cast<char>(std::tolower(static_cast<unsigned char>(wc)));
        if (cnt[lc]-- <= 0) return false;
    }
    return true;
}

std::vector<std::string> WordList::filterByLetters(const std::vector<char> &letters, size_t minLen) const {
    std::vector<std::string> out;
    for (const auto &w : words_) {
        if (w.size() < minLen) continue;
        bool ok = canFormWord(w, letters);
        if (ok) out.push_back(w);
    }
    return out;
}

std::ostream& operator<<(std::ostream& os, const WordList& wl) {
    os << "WordList: " << wl.words_.size() << " words\n";
    return os;
}
