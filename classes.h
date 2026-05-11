#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <memory>

/// CLASE DE EXCEPTII

class InternetException : public std::exception
{
private:
    std::string mesaj;

public:
    InternetException(const std::string &mesaj);
    const char *what() const noexcept override;
};

class DomeniuInvalidException : public InternetException
{
public:
    DomeniuInvalidException(const std::string &mesaj);
};

class AdresaInvalidaException : public InternetException
{
public:
    AdresaInvalidaException(const std::string &mesaj);
};

class VizitaInvalidaException : public InternetException
{
public:
    VizitaInvalidaException(const std::string &mesaj);
};

/// CLASA DE BAZA: ResursaInternet

class ResursaInternet
{
protected:
    std::string categorie;

public:
    static int contor;

    ResursaInternet();
    ResursaInternet(const std::string &categorie);
    ResursaInternet(const ResursaInternet &R);

    ResursaInternet &operator=(const ResursaInternet &R);

    virtual ~ResursaInternet();

    void setCategorie(const std::string &categorie);
    const std::string &getCategorie() const;

    static int getContor();

    /// Interfete non-virtuale care apeleaza functii virtuale.
    int Relevanta() const;
    void NormalizeazaResursa();
    void Afiseaza(std::ostream &out) const;

    virtual int CalculeazaRelevanta() const = 0;
    virtual void Normalizeaza() = 0;
    virtual ResursaInternet *clone() const = 0;
    virtual void AfisareVirtuala(std::ostream &out) const = 0;
};

std::ostream &operator<<(std::ostream &out, const ResursaInternet &R);

/// CLASA DERIVATA: Domeniu

class Domeniu : public ResursaInternet
{
private:
    std::string nume;
    std::string extensie;

public:
    static int contor;

    Domeniu();
    Domeniu(const std::string &nume, const std::string &extensie);
    Domeniu(const Domeniu &D);

    Domeniu &operator=(const Domeniu &D);

    ~Domeniu() override;

    void setNume(const std::string &nume);
    void setExtensie(const std::string &extensie);

    const std::string &getNume() const;
    const std::string &getExtensie() const;

    static int getContor();
    static bool EsteExtensieCunoscuta(const std::string &extensie);

    void ModificareDomeniu(const std::string &nume, const std::string &extensie);

    int CalculeazaRelevanta() const override;
    void Normalizeaza() override;
    ResursaInternet *clone() const override;
    void AfisareVirtuala(std::ostream &out) const override;
};

std::ostream &operator<<(std::ostream &out, const Domeniu &D);

/// CLASA DERIVATA: AdresaWeb

class AdresaWeb : public ResursaInternet
{
private:
    Domeniu domeniu;
    std::string protocol;
    bool sigur;

public:
    static int contor;

    AdresaWeb();
    AdresaWeb(const Domeniu &domeniu, const std::string &protocol, bool sigur);
    AdresaWeb(const AdresaWeb &A);

    AdresaWeb &operator=(const AdresaWeb &A);

    ~AdresaWeb() override;

    void setDomeniu(const Domeniu &domeniu);
    void setProtocol(const std::string &protocol);
    void setSigur(bool sigur);

    const Domeniu &getDomeniu() const;
    const std::string &getProtocol() const;
    bool getSigur() const;

    static int getContor();
    static bool EsteProtocolValid(const std::string &protocol);

    void ModificareAdresaWeb(const Domeniu &domeniu, const std::string &protocol, bool sigur);

    int CalculeazaRelevanta() const override;
    void Normalizeaza() override;
    ResursaInternet *clone() const override;
    void AfisareVirtuala(std::ostream &out) const override;
};

std::ostream &operator<<(std::ostream &out, const AdresaWeb &A);

/// CLASA DERIVATA: Vizita

class Vizita : public ResursaInternet
{
private:
    std::vector<AdresaWeb> adrese;
    std::vector<std::string> date;
    std::vector<int> clicuri;

public:
    static int contor;

    Vizita();
    Vizita(const std::vector<AdresaWeb> &adrese,
           const std::vector<std::string> &date,
           const std::vector<int> &clicuri);
    Vizita(const Vizita &V);

    Vizita &operator=(const Vizita &V);

    ~Vizita() override;

    void setAdresa(const AdresaWeb &adresa, int index);
    void setData(const std::string &data, int index);
    void setClicuri(int clicuri, int index);

    const AdresaWeb &getAdresaWeb(int index) const;
    const std::string &getData(int index) const;
    int getClicuri(int index) const;
    int getDimensiune() const;

    static int getContor();
    static bool EsteDataValida(const std::string &data);

    void AdaugaVizita(const AdresaWeb &adresa, const std::string &data, int clicuri);
    void ModificareVizita(const AdresaWeb &adresa, const std::string &data, int clicuri, int index);

    Vizita CuratareVizite(const Vizita &vizitaCurenta);
    void RecomandareSite();
    void ValidareVizite();

    int CalculeazaRelevanta() const override;
    void Normalizeaza() override;
    ResursaInternet *clone() const override;
    void AfisareVirtuala(std::ostream &out) const override;
};

std::ostream &operator<<(std::ostream &out, const Vizita &V);

/// CLASA CU ATRIBUT POINTER LA CLASA DE BAZA: ResursaSelectata

class ResursaSelectata
{
private:
    ResursaInternet *resursa;
    std::string nota;
    int prioritate;

public:
    ResursaSelectata();
    ResursaSelectata(const ResursaInternet &resursa,
                     const std::string &nota,
                     int prioritate);
    ResursaSelectata(const ResursaSelectata &R);

    ~ResursaSelectata();

    void swap(ResursaSelectata &R) noexcept;

    /// Copy and swap.
    ResursaSelectata &operator=(ResursaSelectata R);

    void setResursa(const ResursaInternet &resursa);
    void setNota(const std::string &nota);
    void setPrioritate(int prioritate);

    const ResursaInternet *getResursa() const;
    const std::string &getNota() const;
    int getPrioritate() const;

    int CalculeazaRelevantaResursa() const;
    void NormalizeazaResursaSelectata();

    /// Downcast cu sens prin dynamic_cast.
    bool ContineVizita() const;
    int NumarViziteDacaEsteVizita() const;

    void Afisare(std::ostream &out) const;
};

std::ostream &operator<<(std::ostream &out, const ResursaSelectata &R);

/// CLASA CARE FOLOSESTE SMART POINTERS SI STL: ColectieResurse

class ColectieResurse
{
private:
    std::vector<std::unique_ptr<ResursaInternet>> resurse;

public:
    ColectieResurse();
    ColectieResurse(const ColectieResurse &C);

    ColectieResurse &operator=(const ColectieResurse &C);

    void AdaugaResursa(const ResursaInternet &resursa);

    int getDimensiune() const;
    const ResursaInternet &getResursa(int index) const;

    void NormalizeazaToate();
    void SorteazaDupaRelevanta();

    int NumaraVizite() const;
    int RelevantaMedie() const;

    void Afisare(std::ostream &out) const;
};

std::ostream &operator<<(std::ostream &out, const ColectieResurse &C);