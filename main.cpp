#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "classes.h"

/// 35. InternetDocumentare -> Domenii, AdreseWeb, Vizite
///
/// Programul urmareste sa creeze un mod pentru un utilizator sa se documenteze despre internet.
/// Acesta contine clase precum Domenii, AdreseWeb si Vizite pentru a tine evidenta acestor lucruri.
///
/// Tema 2:
/// - folosire STL: string, vector, unique_ptr in clasele din proiect;
/// - ierarhie de mostenire: ResursaInternet -> Domeniu, AdresaWeb, Vizita;
/// - functii virtuale pure si apeluri virtuale;
/// - clone pentru copiere polimorfica;
/// - clasa cu atribut pointer la baza: ResursaSelectata;
/// - copy and swap in ResursaSelectata;
/// - dynamic_cast pentru downcast cu sens;
/// - smart pointers in ColectieResurse;
/// - exceptii proprii.

void AfiseazaTitluSectiune(const std::string &titlu)
{
    std::cout << std::endl;
    std::cout << "====================================================================" << std::endl;
    std::cout << titlu << std::endl;
    std::cout << "====================================================================" << std::endl;
}

/// Creeaza lista initiala de adrese web.
/// Datele contin intentionat duplicate, pentru a putea testa ulterior
/// functionalitatea de deduplicare.
std::vector<AdresaWeb> CreeazaAdreseInitiale()
{
    std::vector<AdresaWeb> adrese;

    adrese.push_back(AdresaWeb(Domeniu("google", ".com"), "https", true));
    adrese.push_back(AdresaWeb(Domeniu("youtube", ".com"), "https", true));
    adrese.push_back(AdresaWeb(Domeniu("google", ".com"), "https", true));
    adrese.push_back(AdresaWeb(Domeniu("facebook", ".com"), "http", true));
    adrese.push_back(AdresaWeb(Domeniu("youtube", ".com"), "https", true));

    return adrese;
}

/// Creeaza lista de date calendaristice asociate vizitelor.
std::vector<std::string> CreeazaDateInitiale()
{
    std::vector<std::string> date;

    date.push_back("19-03-2026_09:00");
    date.push_back("19-03-2026_09:05");
    date.push_back("19-03-2026_09:10");
    date.push_back("19-03-2026_09:15");
    date.push_back("19-03-2026_09:20");

    return date;
}

/// Creeaza lista de clicuri asociate fiecarei vizite.
/// Fiecare element din acest vector corespunde elementului cu acelasi index
/// din vectorul de adrese si vectorul de date.
std::vector<int> CreeazaClicuriInitiale()
{
    std::vector<int> clicuri;

    clicuri.push_back(12);
    clicuri.push_back(20);
    clicuri.push_back(15);
    clicuri.push_back(8);
    clicuri.push_back(18);

    return clicuri;
}

/// Creeaza obiectul initial de tip Vizita.
/// Aceasta functie grupeaza construirea vectorilor auxiliari si apelul
/// constructorului clasei Vizita.
Vizita CreeazaVizitaInitiala()
{
    std::vector<AdresaWeb> adrese = CreeazaAdreseInitiale();
    std::vector<std::string> date = CreeazaDateInitiale();
    std::vector<int> clicuri = CreeazaClicuriInitiale();

    Vizita vizita(adrese, date, clicuri);

    /// Normalizam datele pentru a elimina sau corecta informatiile invalide,
    vizita.ValidareVizite();

    return vizita;
}

/// Afiseaza istoricul initial al vizitelor.
void AfiseazaIstoricVizite(const Vizita &vizita)
{
    AfiseazaTitluSectiune("1. ISTORIC VIZITE");

    std::cout << "Istoric vizite:" << std::endl;
    std::cout << vizita << std::endl;
}

/// Curata istoricul de vizite prin eliminarea duplicatelor.
/// Returneaza un nou obiect Vizita, fara sa modifice explicit obiectul initial.
Vizita DeduplicaVizite(Vizita &vizita)
{
    AfiseazaTitluSectiune("2. DEDUPLICARE VIZITE");

    Vizita vizitaDeduplicata = vizita.CuratareVizite(vizita);

    std::cout << "Istoric vizite dupa deduplicare:" << std::endl;
    std::cout << vizitaDeduplicata << std::endl;

    return vizitaDeduplicata;
}

/// Calculeaza topul adreselor web in functie de numarul de clicuri.
/// Nu modifica obiectul Vizita primit ca parametru.
std::vector<AdresaWeb> CalculeazaTopAdrese(const Vizita &vizita, int limitaTop)
{
    std::vector<AdresaWeb> adreseTop;

    int numarVizite = vizita.getDimensiune();
    if (numarVizite < limitaTop)
    {
        limitaTop = numarVizite;
    }

    /// Vector auxiliar care retine ce pozitii au fost deja selectate.
    std::vector<bool> selectat(numarVizite, false);

    for (int i = 0; i < limitaTop; i++)
    {
        int maximScor = -1;
        int indexMax = -1;

        /// Cautam adresa neselectata cu cel mai mare numar de clicuri.
        for (int j = 0; j < numarVizite; j++)
        {
            if (!selectat[j] && vizita.getClicuri(j) > maximScor)
            {
                maximScor = vizita.getClicuri(j);
                indexMax = j;
            }
        }

        /// Daca s-a gasit o pozitie valida, adaugam adresa in top.
        if (indexMax != -1)
        {
            adreseTop.push_back(vizita.getAdresaWeb(indexMax));
            selectat[indexMax] = true;
        }
    }

    return adreseTop;
}

/// Testeaza mecanismul de recomandare si afiseaza topul adreselor web.
void TesteazaRecomandareSiteuri(Vizita &vizita)
{
    AfiseazaTitluSectiune("3. RECOMANDARE SITE-URI");

    vizita.RecomandareSite();

    const int limitaTop = 10;
    std::vector<AdresaWeb> adreseTop = CalculeazaTopAdrese(vizita, limitaTop);

    std::cout << std::endl;
    std::cout << "Top " << adreseTop.size()
              << " site-uri recomandate in functie de istoricul vizitelor:"
              << std::endl;

    for (int i = 0; i < static_cast<int>(adreseTop.size()); i++)
    {
        std::cout << i + 1 << ". " << adreseTop[i] << std::endl;
    }
}

/// Testeaza polimorfismul prin pointeri la clasa de baza ResursaInternet.
void TesteazaPolimorfism(const Vizita &vizita)
{
    AfiseazaTitluSectiune("4. TEST MOSTENIRE SI POLIMORFISM");

    std::cout << "Test polimorfism prin pointeri la clasa de baza:" << std::endl;

    std::vector<ResursaInternet *> resursePolimorfice;

    /// Fiecare obiect derivat este tratat prin pointer la clasa de baza.
    resursePolimorfice.push_back(new Domeniu("wikipedia", "org"));
    resursePolimorfice.push_back(new AdresaWeb(Domeniu("stackoverflow", ".com"), "https", true));
    resursePolimorfice.push_back(vizita.clone());

    for (int i = 0; i < static_cast<int>(resursePolimorfice.size()); i++)
    {
        resursePolimorfice[i]->NormalizeazaResursa();

        std::cout << i + 1 << ". "
                  << *resursePolimorfice[i]
                  << std::endl;
    }

    for (int i = 0; i < static_cast<int>(resursePolimorfice.size()); i++)
    {
        delete resursePolimorfice[i];
    }

    resursePolimorfice.clear();
}

/// Testeaza clasa ResursaSelectata, care contine un pointer la baza ResursaInternet.
/// Include si testul pentru dynamic_cast prin metodele ContineVizita si NumarViziteDacaEsteVizita.
void TesteazaResursaSelectata(const Vizita &vizita)
{
    AfiseazaTitluSectiune("5. TEST CLASA CU ATRIBUT POINTER LA BAZA");

    std::cout << "Test clasa ResursaSelectata, cu atribut ResursaInternet*:" << std::endl;

    ResursaSelectata resursaSelectata(
        vizita,
        "Istoric deduplicat selectat pentru analiza",
        1);

    std::cout << resursaSelectata << std::endl;

    std::cout << "Relevanta resursei selectate: "
              << resursaSelectata.CalculeazaRelevantaResursa()
              << std::endl;

    if (resursaSelectata.ContineVizita())
    {
        std::cout << "Downcast reusit: resursa selectata este de tip Vizita." << std::endl;

        std::cout << "Numar vizite in resursa selectata: "
                  << resursaSelectata.NumarViziteDacaEsteVizita()
                  << std::endl;
    }
    else
    {
        std::cout << "Resursa selectata nu este de tip Vizita." << std::endl;
    }
}

/// Testeaza operatorul de atribuire implementat prin copy-and-swap in clasa ResursaSelectata.
void TesteazaCopyAndSwap(const Vizita &vizita)
{
    AfiseazaTitluSectiune("6. TEST COPY AND SWAP");

    std::cout << "Test copy and swap pentru ResursaSelectata:" << std::endl;

    ResursaSelectata R1(
        vizita,
        "Istoric deduplicat selectat pentru analiza",
        1);

    ResursaSelectata R2(
        Domeniu("mit", ".edu"),
        "Domeniu educational",
        2);

    std::cout << "Inainte de atribuire:" << std::endl;
    std::cout << "R1 = " << R1 << std::endl;
    std::cout << "R2 = " << R2 << std::endl;

    /// Aici se testeaza operatorul=.
    /// Daca este implementat corect prin copy-and-swap,
    /// R2 va primi o copie independenta a resursei din R1.
    R2 = R1;

    std::cout << std::endl;
    std::cout << "Dupa atribuire R2 = R1:" << std::endl;
    std::cout << "R1 = " << R1 << std::endl;
    std::cout << "R2 = " << R2 << std::endl;
}

/// Testeaza clasa ColectieResurse.
void TesteazaColectieResurse(const Vizita &vizita)
{
    AfiseazaTitluSectiune("7. TEST SMART POINTERS SI COLECTIE STL");

    std::cout << "Test ColectieResurse, cu vector si unique_ptr:" << std::endl;

    ColectieResurse colectie;

    colectie.AdaugaResursa(Domeniu("harvard", "edu"));
    colectie.AdaugaResursa(AdresaWeb(Domeniu("github", ".com"), "https", true));
    colectie.AdaugaResursa(vizita);
    colectie.AdaugaResursa(Domeniu("example", "net"));

    colectie.NormalizeazaToate();

    std::cout << "Colectie inainte de sortare:" << std::endl;
    std::cout << colectie << std::endl;

    colectie.SorteazaDupaRelevanta();

    std::cout << std::endl;
    std::cout << "Colectie dupa sortare descrescatoare dupa relevanta:" << std::endl;
    std::cout << colectie << std::endl;
}

/// Testeaza exceptiile proprii.
/// Se construieste intentionat o adresa web cu protocol invalid.
void TesteazaExceptiiProprii()
{
    AfiseazaTitluSectiune("8. TEST EXCEPTII PROPRII");

    std::cout << "Test exceptii proprii:" << std::endl;

    try
    {
        AdresaWeb adresaInvalida(Domeniu("site", ".com"), "ftp", true);
        std::cout << adresaInvalida << std::endl;
    }
    catch (const InternetException &E)
    {
        std::cout << "Exceptie prinsa corect: " << E.what() << std::endl;
    }
}

/// Afiseaza contorii statici ai claselor.
void AfiseazaContoriClase()
{
    AfiseazaTitluSectiune("9. CONTORI");

    std::cout << "Contori clase:" << std::endl;
    std::cout << "ResursaInternet::contor = " << ResursaInternet::getContor() << std::endl;
    std::cout << "Domeniu::contor = " << Domeniu::getContor() << std::endl;
    std::cout << "AdresaWeb::contor = " << AdresaWeb::getContor() << std::endl;
    std::cout << "Vizita::contor = " << Vizita::getContor() << std::endl;
}

int main()
{
    try
    {
        /// 1. Construim istoricul initial de vizite.
        Vizita vizitaInitiala = CreeazaVizitaInitiala();

        /// 2. Afisam istoricul initial.
        AfiseazaIstoricVizite(vizitaInitiala);

        /// 3. Eliminam duplicatele si obtinem un nou istoric.
        Vizita vizitaDeduplicata = DeduplicaVizite(vizitaInitiala);

        /// 4. Testam recomandarea de site-uri pe istoricul deduplicat.
        TesteazaRecomandareSiteuri(vizitaDeduplicata);

        /// 5. Testam mostenirea si polimorfismul.
        TesteazaPolimorfism(vizitaDeduplicata);

        /// 6. Testam clasa cu atribut pointer la baza.
        TesteazaResursaSelectata(vizitaDeduplicata);

        /// 7. Testam operatorul de atribuire prin copy-and-swap.
        TesteazaCopyAndSwap(vizitaDeduplicata);

        /// 8. Testam colectia bazata pe vector si unique_ptr.
        TesteazaColectieResurse(vizitaDeduplicata);

        /// 9. Testam exceptiile proprii.
        TesteazaExceptiiProprii();

        /// 10. Afisam contorii statici ai claselor.
        AfiseazaContoriClase();
    }
    catch (const InternetException &E)
    {
        std::cout << "Eroare specifica proiectului: " << E.what() << std::endl;
    }
    catch (const std::exception &E)
    {
        std::cout << "Eroare standard: " << E.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Eroare necunoscuta." << std::endl;
    }

    return 0;
}