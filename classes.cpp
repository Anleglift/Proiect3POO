#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <algorithm>
#include <memory>
#include <utility>
#include "classes.h"

/// FUNCTII AUXILIARE

// Transforma manual literele mari ASCII in litere mici.
static std::string transformaInLitereMici(const std::string &text)
{
    std::string rezultat = text;

    for (char &caracter : rezultat)
    {
        if (caracter >= 'A' && caracter <= 'Z')
        {
            caracter = char(caracter - 'A' + 'a');
        }
    }

    return rezultat;
}

// Adauga punct in fata extensiei, daca lipseste, si transforma totul in litere mici.
static std::string normalizeazaExtensie(const std::string &extensie)
{
    if (extensie.empty())
    {
        throw DomeniuInvalidException("extensia domeniului nu poate fi goala");
    }

    std::string rezultat = transformaInLitereMici(extensie);

    if (rezultat[0] != '.')
    {
        rezultat = "." + rezultat;
    }

    return rezultat;
}

// Valideaza indexul pentru vectorii clasei Vizita.
static void valideazaIndexVizita(int index, int dimensiune, const std::string &operatie)
{
    if (index < 0 || index >= dimensiune)
    {
        throw VizitaInvalidaException("index invalid pentru " + operatie);
    }
}

// EXCEPTII

InternetException::InternetException(const std::string &mesaj)
    : mesaj(mesaj)
{
}

const char *InternetException::what() const noexcept
{
    return mesaj.c_str();
}

DomeniuInvalidException::DomeniuInvalidException(const std::string &mesaj)
    : InternetException("Eroare domeniu: " + mesaj)
{
}

AdresaInvalidaException::AdresaInvalidaException(const std::string &mesaj)
    : InternetException("Eroare adresa web: " + mesaj)
{
}

VizitaInvalidaException::VizitaInvalidaException(const std::string &mesaj)
    : InternetException("Eroare vizita: " + mesaj)
{
}

// CLASA DE BAZA: ResursaInternet

int ResursaInternet::contor = 0;

ResursaInternet::ResursaInternet()
    : categorie("necunoscuta")
{
    contor++;
}

ResursaInternet::ResursaInternet(const std::string &categorie)
    : categorie(categorie.empty() ? "necunoscuta" : categorie)
{
    contor++;
}

ResursaInternet::ResursaInternet(const ResursaInternet &R)
    : categorie(R.categorie)
{
    contor++;
}

ResursaInternet &ResursaInternet::operator=(const ResursaInternet &R)
{
    if (this != &R)
    {
        categorie = R.categorie;
    }

    return *this;
}

ResursaInternet::~ResursaInternet()
{
    contor--;
}

void ResursaInternet::setCategorie(const std::string &categorie)
{
    this->categorie = categorie.empty() ? "necunoscuta" : categorie;
}

const std::string &ResursaInternet::getCategorie() const
{
    return categorie;
}

int ResursaInternet::getContor()
{
    return contor;
}

// Interfata publica non-virtuala. Implementarea reala este lasata claselor derivate.
int ResursaInternet::Relevanta() const
{
    return CalculeazaRelevanta();
}

// Interfata publica non-virtuala pentru normalizare.
void ResursaInternet::NormalizeazaResursa()
{
    Normalizeaza();
}

// Interfata publica non-virtuala pentru afisare polimorfica.
void ResursaInternet::Afiseaza(std::ostream &out) const
{
    AfisareVirtuala(out);
}

std::ostream &operator<<(std::ostream &out, const ResursaInternet &R)
{
    R.Afiseaza(out);
    return out;
}

// CLASA DERIVATA: Domeniu

int Domeniu::contor = 0;

Domeniu::Domeniu()
    : ResursaInternet("domeniu"),
      nume("necunoscut"),
      extensie(".com")
{
    contor++;
}

Domeniu::Domeniu(const std::string &nume, const std::string &extensie)
    : ResursaInternet("domeniu"),
      nume(nume),
      extensie(extensie)
{
    Normalizeaza();
    contor++;
}

Domeniu::Domeniu(const Domeniu &D)
    : ResursaInternet(D),
      nume(D.nume),
      extensie(D.extensie)
{
    contor++;
}

Domeniu &Domeniu::operator=(const Domeniu &D)
{
    if (this != &D)
    {
        ResursaInternet::operator=(D);
        nume = D.nume;
        extensie = D.extensie;
    }

    return *this;
}

Domeniu::~Domeniu()
{
    contor--;
}

void Domeniu::setNume(const std::string &nume)
{
    if (nume.empty())
    {
        throw DomeniuInvalidException("numele domeniului nu poate fi gol");
    }

    this->nume = transformaInLitereMici(nume);
}

void Domeniu::setExtensie(const std::string &extensie)
{
    this->extensie = normalizeazaExtensie(extensie);
}

const std::string &Domeniu::getNume() const
{
    return nume;
}

const std::string &Domeniu::getExtensie() const
{
    return extensie;
}

int Domeniu::getContor()
{
    return contor;
}

bool Domeniu::EsteExtensieCunoscuta(const std::string &extensie)
{
    const std::string extensieNormalizata = normalizeazaExtensie(extensie);

    return extensieNormalizata == ".com" ||
           extensieNormalizata == ".ro" ||
           extensieNormalizata == ".org" ||
           extensieNormalizata == ".edu" ||
           extensieNormalizata == ".net";
}

void Domeniu::ModificareDomeniu(const std::string &nume, const std::string &extensie)
{
    setNume(nume);
    setExtensie(extensie);
}

int Domeniu::CalculeazaRelevanta() const
{
    // Scorul depinde strict de extensia domeniului.
    if (extensie == ".edu")
        return 100;
    if (extensie == ".org")
        return 85;
    if (extensie == ".ro")
        return 70;
    if (extensie == ".com")
        return 60;
    if (extensie == ".net")
        return 55;

    return 40;
}

void Domeniu::Normalizeaza()
{
    if (nume.empty())
    {
        throw DomeniuInvalidException("numele domeniului nu poate fi gol");
    }

    nume = transformaInLitereMici(nume);
    extensie = normalizeazaExtensie(extensie);
}

ResursaInternet *Domeniu::clone() const
{
    return new Domeniu(*this);
}

void Domeniu::AfisareVirtuala(std::ostream &out) const
{
    out << "[Domeniu] "
        << nume << extensie
        << " | categorie: " << getCategorie()
        << " | relevanta: " << CalculeazaRelevanta();
}

std::ostream &operator<<(std::ostream &out, const Domeniu &D)
{
    out << D.getNume() << D.getExtensie();
    return out;
}

// CLASA DERIVATA: AdresaWeb

int AdresaWeb::contor = 0;

AdresaWeb::AdresaWeb()
    : ResursaInternet("adresa web"),
      domeniu(),
      protocol("https"),
      sigur(true)
{
    contor++;
}

AdresaWeb::AdresaWeb(const Domeniu &domeniu, const std::string &protocol, bool sigur)
    : ResursaInternet("adresa web"),
      domeniu(domeniu),
      protocol(protocol),
      sigur(sigur)
{
    Normalizeaza();
    contor++;
}

AdresaWeb::AdresaWeb(const AdresaWeb &A)
    : ResursaInternet(A),
      domeniu(A.domeniu),
      protocol(A.protocol),
      sigur(A.sigur)
{
    contor++;
}

AdresaWeb &AdresaWeb::operator=(const AdresaWeb &A)
{
    if (this != &A)
    {
        ResursaInternet::operator=(A);
        domeniu = A.domeniu;
        protocol = A.protocol;
        sigur = A.sigur;
    }

    return *this;
}

AdresaWeb::~AdresaWeb()
{
    contor--;
}

void AdresaWeb::setDomeniu(const Domeniu &domeniu)
{
    this->domeniu = domeniu;
    this->domeniu.Normalizeaza();
}

void AdresaWeb::setProtocol(const std::string &protocol)
{
    this->protocol = protocol;
    Normalizeaza();
}

void AdresaWeb::setSigur(bool sigur)
{
    this->sigur = sigur;
}

const Domeniu &AdresaWeb::getDomeniu() const
{
    return domeniu;
}

const std::string &AdresaWeb::getProtocol() const
{
    return protocol;
}

bool AdresaWeb::getSigur() const
{
    return sigur;
}

int AdresaWeb::getContor()
{
    return contor;
}

bool AdresaWeb::EsteProtocolValid(const std::string &protocol)
{
    const std::string protocolNormalizat = transformaInLitereMici(protocol);
    return protocolNormalizat == "http" || protocolNormalizat == "https";
}

void AdresaWeb::ModificareAdresaWeb(const Domeniu &domeniu,
                                    const std::string &protocol,
                                    bool sigur)
{
    this->domeniu = domeniu;
    this->protocol = protocol;
    this->sigur = sigur;

    Normalizeaza();
}

int AdresaWeb::CalculeazaRelevanta() const
{
    int scor = domeniu.CalculeazaRelevanta();

    // Protocolul HTTPS creste increderea, HTTP o scade.
    if (protocol == "https")
    {
        scor += 25;
    }
    else
    {
        scor -= 15;
    }

    // Flag-ul sigur influenteaza scorul final.
    if (sigur)
    {
        scor += 15;
    }
    else
    {
        scor -= 20;
    }

    return std::max(0, scor);
}

void AdresaWeb::Normalizeaza()
{
    protocol = transformaInLitereMici(protocol);

    if (!EsteProtocolValid(protocol))
    {
        throw AdresaInvalidaException("protocolul trebuie sa fie http sau https");
    }

    domeniu.Normalizeaza();

    // In aceasta implementare, protocolul decide automat daca adresa este sigura.
    sigur = (protocol == "https");
}

ResursaInternet *AdresaWeb::clone() const
{
    return new AdresaWeb(*this);
}

void AdresaWeb::AfisareVirtuala(std::ostream &out) const
{
    out << "[AdresaWeb] "
        << protocol << "://" << domeniu
        << " | sigur: " << (sigur ? "da" : "nu")
        << " | categorie: " << getCategorie()
        << " | relevanta: " << CalculeazaRelevanta();
}

std::ostream &operator<<(std::ostream &out, const AdresaWeb &A)
{
    out << A.getProtocol()
        << "://"
        << A.getDomeniu()
        << " ( sigur: " << (A.getSigur() ? "da" : "nu") << " )";

    return out;
}

// CLASA DERIVATA: Vizita

int Vizita::contor = 0;

Vizita::Vizita()
    : ResursaInternet("vizita"),
      adrese(),
      date(),
      clicuri()
{
    contor++;
}

Vizita::Vizita(const std::vector<AdresaWeb> &adrese,
               const std::vector<std::string> &date,
               const std::vector<int> &clicuri)
    : ResursaInternet("vizita"),
      adrese(adrese),
      date(date),
      clicuri(clicuri)
{
    ValidareVizite();
    contor++;
}

Vizita::Vizita(const Vizita &V)
    : ResursaInternet(V),
      adrese(V.adrese),
      date(V.date),
      clicuri(V.clicuri)
{
    contor++;
}

Vizita &Vizita::operator=(const Vizita &V)
{
    if (this != &V)
    {
        ResursaInternet::operator=(V);
        adrese = V.adrese;
        date = V.date;
        clicuri = V.clicuri;
    }

    return *this;
}

Vizita::~Vizita()
{
    contor--;
}

void Vizita::setAdresa(const AdresaWeb &adresa, int index)
{
    valideazaIndexVizita(index, getDimensiune(), "setAdresa");
    adrese[index] = adresa;
    adrese[index].Normalizeaza();
}

void Vizita::setData(const std::string &data, int index)
{
    valideazaIndexVizita(index, getDimensiune(), "setData");
    date[index] = EsteDataValida(data) ? data : "Data necunoscuta";
}

void Vizita::setClicuri(int clicuri, int index)
{
    valideazaIndexVizita(index, getDimensiune(), "setClicuri");
    this->clicuri[index] = std::max(2, clicuri);
}

const AdresaWeb &Vizita::getAdresaWeb(int index) const
{
    valideazaIndexVizita(index, getDimensiune(), "getAdresaWeb");
    return adrese[index];
}

const std::string &Vizita::getData(int index) const
{
    valideazaIndexVizita(index, getDimensiune(), "getData");
    return date[index];
}

int Vizita::getClicuri(int index) const
{
    valideazaIndexVizita(index, getDimensiune(), "getClicuri");
    return clicuri[index];
}

int Vizita::getDimensiune() const
{
    return static_cast<int>(adrese.size());
}

int Vizita::getContor()
{
    return contor;
}

bool Vizita::EsteDataValida(const std::string &data)
{
    // Validare minima: pastram comportamentul original.
    return data.size() >= 16;
}

void Vizita::AdaugaVizita(const AdresaWeb &adresa, const std::string &data, int clicuri)
{
    adrese.push_back(adresa);
    date.push_back(data);
    this->clicuri.push_back(clicuri);

    ValidareVizite();
}

void Vizita::ModificareVizita(const AdresaWeb &adresa,
                              const std::string &data,
                              int clicuri,
                              int index)
{
    valideazaIndexVizita(index, getDimensiune(), "ModificareVizita");

    adrese[index] = adresa;
    date[index] = data;
    this->clicuri[index] = clicuri;

    ValidareVizite();
}

Vizita Vizita::CuratareVizite(const Vizita &vizitaCurenta)
{
    std::vector<AdresaWeb> adreseUnice;
    std::vector<std::string> dateUnice;
    std::vector<int> clicuriUnice;

    for (int i = 0; i < vizitaCurenta.getDimensiune(); i++)
    {
        const AdresaWeb &adresaCurenta = vizitaCurenta.getAdresaWeb(i);
        const Domeniu &domeniuCurent = adresaCurenta.getDomeniu();

        int pozitieGasita = -1;

        // Cautam daca domeniul exista deja in lista de domenii unice.
        for (int j = 0; j < static_cast<int>(adreseUnice.size()); j++)
        {
            const Domeniu &domeniuUnic = adreseUnice[j].getDomeniu();

            const bool acelasiNume = domeniuCurent.getNume() == domeniuUnic.getNume();
            const bool aceeasiExtensie = domeniuCurent.getExtensie() == domeniuUnic.getExtensie();

            if (acelasiNume && aceeasiExtensie)
            {
                pozitieGasita = j;
                break;
            }
        }

        if (pozitieGasita == -1)
        {
            // Prima aparitie a domeniului.
            adreseUnice.push_back(adresaCurenta);
            dateUnice.push_back(vizitaCurenta.getData(i));
            clicuriUnice.push_back(vizitaCurenta.getClicuri(i));
        }
        else
        {
            // Domeniu repetat: cumulam clicurile si pastram toate datele.
            clicuriUnice[pozitieGasita] += vizitaCurenta.getClicuri(i);
            dateUnice[pozitieGasita] += " / " + vizitaCurenta.getData(i);
        }
    }

    return Vizita(adreseUnice, dateUnice, clicuriUnice);
}

void Vizita::RecomandareSite()
{
    for (int i = 0; i < getDimensiune(); i++)
    {
        const int scorAdresa = adrese[i].CalculeazaRelevanta();
        int numarDate = 1;

        // Datele combinate sunt separate prin '/'. Pastram logica originala.
        for (char caracter : date[i])
        {
            if (caracter == '/')
            {
                numarDate++;
            }
        }

        const int scor = static_cast<int>(
            0.50 * clicuri[i] +
            0.30 * scorAdresa +
            0.20 * numarDate);

        clicuri[i] = scor;
    }
}

void Vizita::ValidareVizite()
{
    if (adrese.size() != date.size() || adrese.size() != clicuri.size())
    {
        throw VizitaInvalidaException("vectorii adrese, date si clicuri trebuie sa aiba aceeasi dimensiune");
    }

    for (int i = 0; i < getDimensiune(); i++)
    {
        adrese[i].Normalizeaza();

        if (!EsteDataValida(date[i]))
        {
            date[i] = "Data necunoscuta";
        }

        if (clicuri[i] < 2)
        {
            clicuri[i] = 2;
        }
    }
}

int Vizita::CalculeazaRelevanta() const
{
    if (getDimensiune() == 0)
    {
        return 0;
    }

    int scorTotal = 0;

    for (int i = 0; i < getDimensiune(); i++)
    {
        scorTotal += adrese[i].CalculeazaRelevanta();
        scorTotal += clicuri[i];
    }

    return scorTotal / getDimensiune();
}

void Vizita::Normalizeaza()
{
    ValidareVizite();
}

ResursaInternet *Vizita::clone() const
{
    return new Vizita(*this);
}

void Vizita::AfisareVirtuala(std::ostream &out) const
{
    out << "[Vizita] "
        << "numar vizite: " << getDimensiune()
        << " | categorie: " << getCategorie()
        << " | relevanta medie: " << CalculeazaRelevanta();
}

std::ostream &operator<<(std::ostream &out, const Vizita &V)
{
    for (int i = 0; i < V.getDimensiune(); i++)
    {
        out << "Pe data/datele de: "
            << V.getData(i)
            << " site-ul: "
            << V.getAdresaWeb(i)
            << " a avut un engagement de "
            << V.getClicuri(i)
            << " clicuri"
            << std::endl;
    }

    return out;
}

// CLASA CU POINTER LA CLASA DE BAZA: ResursaSelectata

ResursaSelectata::ResursaSelectata()
    : resursa(NULL),
      nota("fara nota"),
      prioritate(0)
{
}

ResursaSelectata::ResursaSelectata(const ResursaInternet &resursa,
                                   const std::string &nota,
                                   int prioritate)
    : resursa(resursa.clone()),
      nota(nota),
      prioritate(prioritate)
{
}

ResursaSelectata::ResursaSelectata(const ResursaSelectata &R)
    : resursa(R.resursa != NULL ? R.resursa->clone() : NULL),
      nota(R.nota),
      prioritate(R.prioritate)
{
}

ResursaSelectata::~ResursaSelectata()
{
    delete resursa;
    resursa = NULL;
}

void ResursaSelectata::swap(ResursaSelectata &R) noexcept
{
    std::swap(resursa, R.resursa);
    std::swap(nota, R.nota);
    std::swap(prioritate, R.prioritate);
}

// Copy-and-swap: copia este primita prin valoare, apoi se face schimbul intern.
ResursaSelectata &ResursaSelectata::operator=(ResursaSelectata R)
{
    swap(R);
    return *this;
}

void ResursaSelectata::setResursa(const ResursaInternet &resursa)
{
    ResursaInternet *copie = resursa.clone();

    delete this->resursa;
    this->resursa = copie;
}

void ResursaSelectata::setNota(const std::string &nota)
{
    this->nota = nota;
}

void ResursaSelectata::setPrioritate(int prioritate)
{
    this->prioritate = prioritate;
}

const ResursaInternet *ResursaSelectata::getResursa() const
{
    return resursa;
}

const std::string &ResursaSelectata::getNota() const
{
    return nota;
}

int ResursaSelectata::getPrioritate() const
{
    return prioritate;
}

int ResursaSelectata::CalculeazaRelevantaResursa() const
{
    if (resursa == NULL)
    {
        throw InternetException("nu exista resursa selectata pentru calculul relevantei");
    }

    return resursa->Relevanta();
}

void ResursaSelectata::NormalizeazaResursaSelectata()
{
    if (resursa == NULL)
    {
        throw InternetException("nu exista resursa selectata pentru normalizare");
    }

    resursa->NormalizeazaResursa();
}

bool ResursaSelectata::ContineVizita() const
{
    return dynamic_cast<const Vizita *>(resursa) != NULL;
}

int ResursaSelectata::NumarViziteDacaEsteVizita() const
{
    const Vizita *vizita = dynamic_cast<const Vizita *>(resursa);

    if (vizita == NULL)
    {
        return 0;
    }

    return vizita->getDimensiune();
}

void ResursaSelectata::Afisare(std::ostream &out) const
{
    out << "Resursa selectata: ";

    if (resursa != NULL)
    {
        out << *resursa;
    }
    else
    {
        out << "NULL";
    }

    out << " | nota: " << nota
        << " | prioritate: " << prioritate;

    if (ContineVizita())
    {
        out << " | contine vizite: " << NumarViziteDacaEsteVizita();
    }
}

std::ostream &operator<<(std::ostream &out, const ResursaSelectata &R)
{
    R.Afisare(out);
    return out;
}

// CLASA CARE FOLOSESTE SMART POINTERS SI STL: ColectieResurse

ColectieResurse::ColectieResurse()
    : resurse()
{
}

ColectieResurse::ColectieResurse(const ColectieResurse &C)
    : resurse()
{
    for (const std::unique_ptr<ResursaInternet> &resursa : C.resurse)
    {
        if (resursa)
        {
            resurse.push_back(std::unique_ptr<ResursaInternet>(resursa->clone()));
        }
    }
}

ColectieResurse &ColectieResurse::operator=(const ColectieResurse &C)
{
    if (this != &C)
    {
        resurse.clear();

        for (const std::unique_ptr<ResursaInternet> &resursa : C.resurse)
        {
            if (resursa)
            {
                resurse.push_back(std::unique_ptr<ResursaInternet>(resursa->clone()));
            }
        }
    }

    return *this;
}

void ColectieResurse::AdaugaResursa(const ResursaInternet &resursa)
{
    resurse.push_back(std::unique_ptr<ResursaInternet>(resursa.clone()));
}

int ColectieResurse::getDimensiune() const
{
    return static_cast<int>(resurse.size());
}

const ResursaInternet &ColectieResurse::getResursa(int index) const
{
    if (index < 0 || index >= getDimensiune())
    {
        throw InternetException("index invalid pentru ColectieResurse::getResursa");
    }

    return *resurse[index];
}

void ColectieResurse::NormalizeazaToate()
{
    for (std::unique_ptr<ResursaInternet> &resursa : resurse)
    {
        if (resursa)
        {
            resursa->NormalizeazaResursa();
        }
    }
}

void ColectieResurse::SorteazaDupaRelevanta()
{
    std::sort(
        resurse.begin(),
        resurse.end(),
        [](const std::unique_ptr<ResursaInternet> &A,
           const std::unique_ptr<ResursaInternet> &B)
        {
            return A->Relevanta() > B->Relevanta();
        });
}

int ColectieResurse::NumaraVizite() const
{
    int numarVizite = 0;

    for (const std::unique_ptr<ResursaInternet> &resursa : resurse)
    {
        if (dynamic_cast<const Vizita *>(resursa.get()) != NULL)
        {
            numarVizite++;
        }
    }

    return numarVizite;
}

int ColectieResurse::RelevantaMedie() const
{
    if (resurse.empty())
    {
        return 0;
    }

    int suma = 0;

    for (const std::unique_ptr<ResursaInternet> &resursa : resurse)
    {
        suma += resursa->Relevanta();
    }

    return suma / static_cast<int>(resurse.size());
}

void ColectieResurse::Afisare(std::ostream &out) const
{
    out << "Colectie resurse internet:" << std::endl;

    for (size_t i = 0; i < resurse.size(); i++)
    {
        out << i + 1 << ". " << *resurse[i] << std::endl;
    }

    out << "Relevanta medie colectie: " << RelevantaMedie() << std::endl;
    out << "Numar obiecte de tip Vizita in colectie: " << NumaraVizite();
}

std::ostream &operator<<(std::ostream &out, const ColectieResurse &C)
{
    C.Afisare(out);
    return out;
}
