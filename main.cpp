// main.cpp
// Include-uri necesare
#include <iostream>
#include <string>
#include <vector>   // Doar pentru a defini nivelele
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <cctype>   // Pentru std::toupper, std::tolower
#include <conio.h>  // Pentru _getch()
#include <limits>
#include <cstring>  // Pentru strlen, strcpy (management C-style string)

// --- Prototipe (Forward Declarations) ---
class CuvantSolutie;
class BancaDeLitere;
class Nivel;
class Joc;
class ManagerNivele; // Noua clasă "fabrică"

// =========================================================================
// CLASA 1: CuvantSolutie (Rule of 0)
// =========================================================================
class CuvantSolutie {
private:
    std::string cuvantAscuns; // Stocat ca MAJUSCULE
    bool gasit;

public:
    // Req 2: Constructor cu parametri
    CuvantSolutie(const std::string& cuvant = "")
        : cuvantAscuns(cuvant), gasit(false) {
        // Convertim cuvântul în majuscule o singură dată
        for (char& c : this->cuvantAscuns) {
            c = std::toupper(c);
        }
    }

    std::string getCuvant() const { return cuvantAscuns; }
    bool esteGasit() const { return gasit; }
    void setGasit(bool stare) { gasit = stare; }

    // Compara (case-insensitive)
    bool compara(const std::string& propunere) const {
        if (propunere.length() != cuvantAscuns.length()) {
            return false;
        }
        for (size_t i = 0; i < propunere.length(); ++i) {
            if (std::toupper(propunere[i]) != cuvantAscuns[i]) {
                return false;
            }
        }
        return true;
    }

    // Req 4: operator<<
    friend std::ostream& operator<<(std::ostream& os, const CuvantSolutie& s) {
        if (s.gasit) {
            os << s.cuvantAscuns;
        } else {
            os << std::string(s.cuvantAscuns.length(), '*');
        }
        return os;
    }
};

// =========================================================================
// CLASA 2: BancaDeLitere (Rule of 5)
// Req: "toate cele 5 funcții membru speciale"
// =========================================================================
class BancaDeLitere {
private:
    char* litere; // Resursă brută (C-style string)
    std::map<char, int> frecventaLitere;

    // Req 5: Funcție privată
    void calculeazaFrecventa() {
        frecventaLitere.clear();
        if (litere) {
            for (size_t i = 0; litere[i] != '\0'; ++i) {
                frecventaLitere[std::toupper(litere[i])]++;
            }
        }
    }

public:
    // 1. Constructor (cu parametri)
    BancaDeLitere(const std::string& l = "") : litere(nullptr) {
        if (!l.empty()) {
            litere = new char[l.length() + 1];
            strcpy_s(litere, l.length() + 1, l.c_str());
        }
        calculeazaFrecventa();
    }

    // 2. Destructor
    ~BancaDeLitere() {
        delete[] litere;
    }

    // 3. Constructor de Copiere
    BancaDeLitere(const BancaDeLitere& other) : litere(nullptr) {
        if (other.litere) {
            litere = new char[strlen(other.litere) + 1];
            strcpy_s(litere, strlen(other.litere) + 1, other.litere);
        }
        calculeazaFrecventa();
    }

    // 4. Constructor de Mutare
    BancaDeLitere(BancaDeLitere&& other) noexcept
        : litere(other.litere) { // "Furăm" pointerul
        other.litere = nullptr; // Resetăm sursa
        calculeazaFrecventa(); // Recalculăm frecvența (sau o mutăm și pe ea)
    }

    // 5. Operator= de Copiere
    BancaDeLitere& operator=(const BancaDeLitere& other) {
        if (this == &other) return *this;

        delete[] litere; // Eliberăm resursa veche
        litere = nullptr;

        if (other.litere) {
            litere = new char[strlen(other.litere) + 1];
            strcpy_s(litere, strlen(other.litere) + 1, other.litere);
        }
        calculeazaFrecventa();
        return *this;
    }

    // 6. Operator= de Mutare
    BancaDeLitere& operator=(BancaDeLitere&& other) noexcept {
        if (this == &other) return *this;
        delete[] litere;
        litere = other.litere; // "Furăm" resursa
        other.litere = nullptr;
        calculeazaFrecventa();
        return *this;
    }

    std::string getLitere() const {
        return (litere) ? std::string(litere) : "";
    }

    // Req 4: operator<<
    friend std::ostream& operator<<(std::ostream& os, const BancaDeLitere& b) {
        os << "Litere Disponibile: [ ";
        if (b.litere) {
            for (size_t i = 0; b.litere[i] != '\0'; ++i) {
                os << (char)std::toupper(b.litere[i])
                   << (b.litere[i+1] == '\0' ? " " : ", ");
            }
        }
        os << "]";
        return os;
    }
};

// =========================================================================
// CLASA 3: Nivel (Rule of 3 - Copy)
// Req: "constructor de copiere, operator= de copiere, destructor"
// =========================================================================
class Nivel {
private:
    int id;
    BancaDeLitere banca;            // Req 1: Compunere (Clasa cu Rule of 5)
    CuvantSolutie* solutii;         // Req 1: Compunere (Resursă brută)
    int nrSolutii;
    int nrGasite;

public:
    // Req 2: Constructor (primește un vector de string-uri)
    Nivel(int i, const BancaDeLitere& b, const std::vector<std::string>& cuvinteSolutii)
        : id(i), banca(b), nrSolutii(cuvinteSolutii.size()), nrGasite(0) {

        solutii = new CuvantSolutie[nrSolutii]; // Alocăm
        for (int j = 0; j < nrSolutii; ++j) {
            solutii[j] = CuvantSolutie(cuvinteSolutii[j]);
        }
    }

    // --- Rule of 3 (Copy) ---
    // 1. Destructor
    ~Nivel() {
        delete[] solutii;
    }

    // 2. Constructor de Copiere
    Nivel(const Nivel& other)
        : id(other.id), banca(other.banca), // Copierea Băncii apelează Ctor-ul ei de Copiere
          nrSolutii(other.nrSolutii), nrGasite(other.nrGasite) {

        solutii = new CuvantSolutie[nrSolutii]; // Alocare nouă
        for (int i = 0; i < nrSolutii; ++i) {
            solutii[i] = other.solutii[i]; // Copiere element cu element
        }
    }

    // 3. Operator= de Copiere
    Nivel& operator=(const Nivel& other) {
        if (this == &other) return *this;

        delete[] solutii; // Eliberăm resursa veche

        // Copiem datele simple
        id = other.id;
        banca = other.banca; // Atribuirea Băncii apelează Op= al ei de Copiere
        nrSolutii = other.nrSolutii;
        nrGasite = other.nrGasite;

        // Alocăm și copiem resursa nouă
        solutii = new CuvantSolutie[nrSolutii];
        for (int i = 0; i < nrSolutii; ++i) {
            solutii[i] = other.solutii[i];
        }
        return *this;
    }
    // NOTĂ: Nu definim Move Semantics, conform cerinței

    // --- Funcții membru ---
    int getId() const { return id; }
    int getNrSolutii() const { return nrSolutii; }
    BancaDeLitere getBanca() const { return banca; } // Returnează o copie
    CuvantSolutie& getSolutie(int index) { return solutii[index]; }
    const CuvantSolutie& getSolutie(int index) const { return solutii[index]; }
    void setNrGasite(int nr) { nrGasite = nr; }

    bool esteComplet() const { return nrGasite == nrSolutii; }

    // Req 6: Funcție non-trivială 1
    bool incearcaCuvant(const std::string& propunere) {
        for (int i = 0; i < nrSolutii; ++i) {
            if (solutii[i].compara(propunere)) {
                if (solutii[i].esteGasit()) {
                    std::cout << "Info: Cuvantul '" << propunere << "' a fost deja gasit!\n";
                    return false;
                }
                solutii[i].setGasit(true);
                nrGasite++;
                return true; // Succes!
            }
        }
        // Mesajul cerut de tine:
        std::cout << "Cuvantul nu se gaseste in lista.\n";
        return false;
    }

    // Req 4: operator<<
    friend std::ostream& operator<<(std::ostream& os, const Nivel& n) {
        os << "---- Nivelul " << n.id << " ----\n\n";
        // Req 4: Apel compus (BancaDeLitere)
        os << n.banca << "\n\n";
        os << "Cuvinte de gasit:\n";
        // Req 4: Apel compus (CuvantSolutie)
        for (int i = 0; i < n.nrSolutii; ++i) {
            os << " " << (i + 1) << ". " << n.solutii[i] << "\n";
        }
        os << "-------------------\n";
        return os;
    }
};

// =========================================================================
// CLASA 4: ManagerNivele (Static Factory)
// =========================================================================
class ManagerNivele {
public:
    // Req 6: Funcție complexă 2 (creează nivelele statice)
    static Nivel* creeazaNivel(int id) {
        BancaDeLitere banca;
        std::vector<std::string> solutii;

        switch (id) {
            // Exemplul tău:
            case 1:
                banca = BancaDeLitere("LMAA");
                solutii.push_back("MA");    // **
                solutii.push_back("MAL");   // ***
                solutii.push_back("AMA");   // ***
                return new Nivel(1, banca, solutii);

            case 2:
                banca = BancaDeLitere("ATRE");
                solutii.push_back("ARE");
                solutii.push_back("RATE");
                solutii.push_back("TE");
                solutii.push_back("ERA");
                return new Nivel(2, banca, solutii);

            case 3:
                banca = BancaDeLitere("CPNAAI");
                solutii.push_back("CAP");
                solutii.push_back("PANA");
                solutii.push_back("NAI");
                solutii.push_back("PIC"); // "PIC" nu e valid. "CAPAC"? nu. "CANAP"?
                solutii.push_back("PICI"); // Nu. "PACAN"?
                // Hai să simplificăm
                banca = BancaDeLitere("CPAA");
                solutii.push_back("CAP");
                solutii.push_back("PAC");
                solutii.push_back("APA");
                return new Nivel(3, banca, solutii);

            case 4:
                banca = BancaDeLitere("OARCEZ");
                solutii.push_back("ORCE");
                solutii.push_back("CARE");
                solutii.push_back("ROC");
                solutii.push_back("ZERO");
                solutii.push_back("RAC");
                return new Nivel(4, banca, solutii);

            case 5:
                banca = BancaDeLitere("IFLURA");
                solutii.push_back("FIR");
                solutii.push_back("FILA");
                solutii.push_back("LIRA");
                solutii.push_back("RUL");
                solutii.push_back("FUR");
                return new Nivel(5, banca, solutii);

            // TODO: Adaugă case 6...49

            case 50:
                banca = BancaDeLitere("FINAL");
                solutii.push_back("FIN");
                solutii.push_back("NAL");
                solutii.push_back("FIL");
                solutii.push_back("ANI");
                return new Nivel(50, banca, solutii);

            default:
                // S-a terminat jocul
                return nullptr;
        }
    }
};

// =========================================================================
// CLASA 5: Joc (Move-Only Semantics)
// Req: "constructor de mutare, operator= de mutare, destructor"
// =========================================================================
class Joc {
private:
    Nivel* nivelCurent;             // Req 1: Compunere (Resursă deținută)
    std::string numeJucator;
    std::string fisierSalvare;

    // Req 5: Funcție privată
    void curataNivelCurent() {
        delete nivelCurent;
        nivelCurent = nullptr;
    }

public:
    // Req 2: Constructor
    Joc(const std::string& nume)
        : nivelCurent(nullptr), numeJucator(nume), fisierSalvare("salvare.txt") {
    }

    // --- Move Semantics (Req 2) ---
    // 1. Destructor
    ~Joc() {
        salveazaProgres(); // Salvăm la ieșire
        curataNivelCurent();
    }

    // 2. Constructor de Mutare
    Joc(Joc&& other) noexcept
        : nivelCurent(other.nivelCurent),
          numeJucator(std::move(other.numeJucator)),
          fisierSalvare(std::move(other.fisierSalvare)) {

        other.nivelCurent = nullptr; // Am "furat" pointerul
    }

    // 3. Operator= de Mutare
    Joc& operator=(Joc&& other) noexcept {
        if (this == &other) return *this;

        curataNivelCurent(); // Eliberăm resursa proprie

        // "Furăm" resursele de la sursă
        nivelCurent = other.nivelCurent;
        numeJucator = std::move(other.numeJucator);
        fisierSalvare = std::move(other.fisierSalvare);

        other.nivelCurent = nullptr; // Resetăm sursa
        return *this;
    }

    // Interzicem Copierea (conform cerinței implicite de "Move-Only")
    Joc(const Joc&) = delete;
    Joc& operator=(const Joc&) = delete;

    // --- Funcții membru ---
    bool areSalvare() const {
        std::ifstream f(fisierSalvare);
        return f.is_open();
    }

    // Req 6: Funcție non-trivială 3 (Pornire/Trecere la nivel nou)
    void pornesteNivel(int id) {
        curataNivelCurent(); // Șterge nivelul vechi

        // Cere fabricii noul nivel
        nivelCurent = ManagerNivele::creeazaNivel(id);

        if (nivelCurent) {
            salveazaProgres(); // Salvăm starea inițială a noului nivel
        }
    }

    // Salvează progresul
    void salveazaProgres() const {
        if (!nivelCurent) {
            // Dacă nu e niciun nivel (joc terminat), ștergem salvarea
            std::remove(fisierSalvare.c_str());
            return;
        }

        std::ofstream f(fisierSalvare);
        if (!f.is_open()) return;

        // Salvăm ID-ul nivelului
        f << nivelCurent->getId() << "\n";

        // Salvăm starea soluțiilor (doar ce e găsit)
        f << nivelCurent->getNrSolutii() << "\n";
        for (int i = 0; i < nivelCurent->getNrSolutii(); ++i) {
            f << nivelCurent->getSolutie(i).esteGasit() << "\n";
        }
        f.close();
    }

    // Încarcă progresul
    bool incarcaProgres() {
        std::ifstream f(fisierSalvare);
        if (!f.is_open()) return false;

        curataNivelCurent();

        int id, nrSol;
        f >> id;
        f >> nrSol;

        // 1. Creăm nivelul de la zero folosind ID-ul
        nivelCurent = ManagerNivele::creeazaNivel(id);
        if (!nivelCurent) return false; // Nivelul salvat nu mai există

        // 2. Verificăm dacă salvarea corespunde
        if (nivelCurent->getNrSolutii() != nrSol) {
             std::cerr << "Eroare: Salvarea este corupta (nr. solutii diferit).\n";
             curataNivelCurent();
             return false;
        }

        // 3. Actualizăm starea "gasit"
        int nrGasiteSalvate = 0;
        bool gasit;
        for(int i = 0; i < nrSol; ++i) {
            f >> gasit;
            if (gasit) {
                nivelCurent->getSolutie(i).setGasit(true);
                nrGasiteSalvate++;
            }
        }
        nivelCurent->setNrGasite(nrGasiteSalvate);
        f.close();
        return true;
    }

    bool incearca(const std::string& propunere) {
        if (!nivelCurent) return false;
        bool succes = nivelCurent->incearcaCuvant(propunere);
        if (succes) salveazaProgres(); // Salvăm după fiecare ghicire corectă
        return succes;
    }

    bool nivelTerminat() const {
        if (!nivelCurent) return true; // Fără nivel = terminat
        return nivelCurent->esteComplet();
    }

    int getIdNivelCurent() const {
        if (!nivelCurent) return 0;
        return nivelCurent->getId();
    }

    // Req 4: operator<<
    friend std::ostream& operator<<(std::ostream& os, const Joc& j) {
        os << "Jucator: " << j.numeJucator << "\n";
        if (j.nivelCurent) {
            // Req 4: Apel compus (Nivel)
            os << *(j.nivelCurent);
        } else {
            os << "Niciun nivel incarcat.\n";
        }
        return os;
    }
};

// =========================================================================
// FUNCȚII UI (Helper)
// =========================================================================

void curataEcran() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void asteapta(int secunde) {
    std::this_thread::sleep_for(std::chrono::seconds(secunde));
}

void asteaptaTasta() {
    std::cout << "\nApasa orice tasta pentru a incepe." << std::endl;
    _getch(); // Așteaptă o tastă FĂRĂ Enter
}

int afiseazaMeniuPrincipal(bool areSalvare) {
    int alegere = 0;
    while (true) {
        curataEcran();
        std::cout << "Optiuni de joc:\n\n";

        if (areSalvare) {
            std::cout << "1. Reia Jocul\n";
            std::cout << "2. Joc Nou\n";
            std::cout << "3. Exit\n";
        } else {
            std::cout << "1. Joc Nou\n"; // Corect
            std::cout << "2. Exit\n"; // Corect
        }

        std::cout << "\nAlege o optiune: ";

        if (!(std::cin >> alegere)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (areSalvare) {
            if (alegere >= 1 && alegere <= 3) return alegere;
        } else {
            if (alegere == 1) return 2; // Mapăm "Joc Nou" la codul 2
            if (alegere == 2) return 3; // Mapăm "Exit" la codul 3
        }
        // Dacă input-ul e invalid (ex: 5), bucla se reia
    }
}

// =========================================================================
// Req 7: SCENARIU DE UTILIZARE (main)
// =========================================================================
int main() {
    // --- Inițializare ---
    // Ștergem fișierul cu soluții vechi, dacă există
    std::remove("solutii_secrete.txt");
    // Nu mai încărcăm dicționarul.

    Joc joc("Player1"); // Creăm jocul (obiectul nu poate fi copiat)

    // --- UI Flow ---
    curataEcran();
    std::cout << "---===== Bun venit ! ====---" << std::endl;
    asteapta(1);
    asteaptaTasta();

    int alegereMeniu = afiseazaMeniuPrincipal(joc.areSalvare());

    // --- Logica Meniu ---
    if (alegereMeniu == 3) { // Exit
        std::cout << "La revedere!\n";
        return 0; // Destructorul lui 'joc' *nu* se apelează (e ok)
                  // Corecție: se apelează, ceea ce e bine.
    }

    if (alegereMeniu == 1) { // Reia Jocul
        std::cout << "Se incarca progresul...\n";
        if (!joc.incarcaProgres()) {
            std::cout << "Salvarea este corupta! Se incepe un joc nou.\n";
            asteapta(2);
            joc.pornesteNivel(1); // Începe de la 1
        }
        asteapta(1);
    }
    else if (alegereMeniu == 2) { // Joc Nou
        std::cout << "Se incepe un joc nou...\n";
        joc.pornesteNivel(1); // Începe de la nivelul 1
        asteapta(1);
    }

    // --- Bucla Principală de Joc ---
    std::string propunere;
    while (true) {
        curataEcran();
        // Adăugăm linii goale PENTRU A FI SIGURI CĂ VEZI TOT
        std::cout << "\n\n\n";

        std::cout << joc; // Aici se afișează Nivelul (cu litere)

        if (joc.nivelTerminat()) {
            int idCurent = joc.getIdNivelCurent();
            if (idCurent == 0) { // Nu s-a putut încărca/crea nivelul
                 std::cerr << "Eroare la incarcarea nivelului.\n";
                 asteapta(2);
                 break;
            }
            if (idCurent >= 50) { // Am terminat ultimul nivel
                std::cout << "\n*** FELICITARI! Ai terminat toate cele 50 de nivele! ***\n";
                joc.pornesteNivel(0); // Șterge salvarea
                asteapta(3);
                break;
            }

            std::cout << "\n*** FELICITARI! Ai terminat Nivelul " << idCurent << "! ***\n";
            std::cout << "Se pregateste nivelul urmator...\n";
            asteapta(2);
            joc.pornesteNivel(idCurent + 1); // Pornește nivelul următor
            continue;
        }

        std::cout << "\n> Introdu un cuvant (sau 'exit' pt a iesi): ";
        std::cin >> propunere;

        if (propunere == "exit") {
            std::cout << "Progresul a fost salvat. La revedere!\n";
            asteapta(1);
            break; // Destructorul lui 'joc' va salva progresul
        }

        bool succes = joc.incearca(propunere);

        if (!succes) {
            // Mesajul de eroare a fost deja afișat
            asteapta(1); // Așteptăm 1 sec să vadă eroarea
        }
        // Dacă e succes, bucla se reia și afișează tabla actualizată
    }

    return 0;
}