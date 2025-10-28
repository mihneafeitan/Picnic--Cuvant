#pragma once
#include <vector>
#include <string>
#include <ostream>

class Level {
private:
    int id_;
    std::vector<char> letters_;             // litere disponibile
    std::vector<std::string> targets_;      // cuvintele care trebuie găsite (lowercase)
    std::vector<bool> found_;               // ce este găsit

    // helper privat
    static std::vector<char> toLowerLetters(const std::string &s);

public:
    // constructor de inițializare
    Level(int id, const std::vector<char>& letters, const std::vector<std::string>& targets);

    // copy semantics + destructor (cerință explicită)
    Level(const Level &other);
    Level& operator=(const Level &other);
    ~Level();

    // funcționalități publice netriviale:
    // afișează starea curentă (cu ** pentru negăsite)
    void display() const;

    // încearcă un guess: returnează true dacă s-a găsit și false altfel
    bool tryGuess(const std::string &guess);

    // verifică dacă nivelul e complet
    bool isComplete() const;

    // getters
    int id() const { return id_; }
    const std::vector<char>& letters() const { return letters_; }
    const std::vector<std::string>& targets() const { return targets_; }

    friend std::ostream& operator<<(std::ostream& os, const Level& lvl);
};
