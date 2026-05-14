#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <fstream>
#include "classes.h"
std::ifstream fin("sites.in");

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
///
/// Tema 3:
/// - Design Pattern 1: Template Method in ResursaInternet.
///   Metodele Relevanta(), NormalizeazaResursa() si Afiseaza()
///   definesc scheletul operatiilor, iar clasele derivate implementeaza pasii concreti.
///
/// - Design Pattern 2: Strategy pentru recomandarea site-urilor.
///   Clasa Vizita foloseste o StrategieRecomandare pentru a calcula scorul de recomandare.
///
/// - Clasa template: Clasament<T>.
///   Este folosita pentru Clasament<AdresaWeb> si Clasament<Domeniu>.

void AfiseazaTitluSectiune(const std::string &titlu)
{
    std::cout << std::endl;
    std::cout << "====================================================================" << std::endl;
    std::cout << titlu << std::endl;
    std::cout << "====================================================================" << std::endl;
}

/// Creeaza obiectul initial de tip Vizita.
/// Aceasta functie grupeaza construirea vectorilor auxiliari si apelul
/// constructorului clasei Vizita.
Vizita CreeazaVizitaInitiala()
{
    std::vector<AdresaWeb> adrese;
    std::vector<std::string> date;
    std::vector<int> clicuri;

    int x;
    fin >> x;
    for (int i = 0; i < x; i++)
    {

        std::string dom, ext, prot, data;
        int sigur, clic;

        fin >> dom >> ext >> prot >> sigur >> data >> clic;

        adrese.push_back(AdresaWeb(Domeniu(dom, ext), prot, (sigur) ? true : false));
        clicuri.push_back(clic);
        date.push_back(data);
    }

    Vizita vizita(adrese, date, clicuri);

    return vizita;
}

/// Afiseaza istoricul initial al vizitelor.
void AfiseazaIstoricVizite(const Vizita &vizita)
{
    std::cout << "Istoric vizite:" << std::endl;
    std::cout << vizita << std::endl;
}

/// Curata istoricul de vizite prin eliminarea duplicatelor.
/// Returneaza un nou obiect Vizita, fara sa modifice explicit obiectul initial.
Vizita DeduplicaVizite(Vizita &vizita)
{
    Vizita vizitaDeduplicata = vizita.CuratareVizite(vizita);

    std::cout << "Istoric vizite dupa deduplicare:" << std::endl;
    std::cout << vizitaDeduplicata << std::endl;

    return vizitaDeduplicata;
}

Clasament<AdresaWeb> CalculeazaClasamentAdrese(const Vizita &vizita)
{
    Clasament<AdresaWeb> clasament;

    for (int i = 0; i < vizita.getDimensiune(); i++)
    {
        clasament.AdaugaElement(
            vizita.getAdresaWeb(i),
            vizita.getClicuri(i));
    }

    clasament.SorteazaDescrescator();

    return clasament;
}

/// Testeaza mecanismul de recomandare si afiseaza topul adreselor web.
void TesteazaRecomandareSiteuri(Vizita &vizita)
{
    StrategieRecomandareSiguranta strategie;

    std::cout << "Strategie folosita: "
              << strategie.getNumeStrategie()
              << std::endl;

    vizita.RecomandareSite(strategie);

    Clasament<AdresaWeb> clasamentAdrese = CalculeazaClasamentAdrese(vizita);

    std::cout << std::endl;
    std::cout << "Top site-uri recomandate in functie de istoricul vizitelor:"
              << std::endl;

    clasamentAdrese.Afiseaza(std::cout, 10);
}

/// Calculeaza topul adreselor web in functie de numarul de clicuri.
/// Nu modifica obiectul Vizita primit ca parametru.

Clasament<Domeniu> CalculeazaClasamentDomenii(const Vizita &vizita)
{
    Clasament<Domeniu> clasament;

    for (int i = 0; i < vizita.getDimensiune(); i++)
    {
        const Domeniu &domeniu = vizita.getAdresaWeb(i).getDomeniu();

        int scor = domeniu.CalculeazaRelevanta() + vizita.getClicuri(i);

        clasament.AdaugaElement(domeniu, scor);
    }

    clasament.SorteazaDescrescator();

    return clasament;
}

void TesteazaClasamentDomenii(const Vizita &vizita)
{
    Clasament<Domeniu> clasamentDomenii =
        CalculeazaClasamentDomenii(vizita);

    std::cout << std::endl
              << "Domenii ordonate dupa relevanta si engagement:"
              << std::endl;

    clasamentDomenii.Afiseaza(std::cout, 10);
}

/// Afiseaza contorii statici ai claselor.
void AfiseazaContoriClase()
{
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
        /// 1. Construim istoricul inițial de vizite și îl afișăm.
        AfiseazaTitluSectiune("1. ISTORIC VIZITE");
        Vizita vizitaInitiala = CreeazaVizitaInitiala();
        AfiseazaIstoricVizite(vizitaInitiala);

        /// 2. Eliminăm duplicatele și obținem un nou istoric. Afișăm topul de site-uri.
        AfiseazaTitluSectiune("2. DEDUPLICARE VIZITE");
        Vizita vizitaDeduplicata = DeduplicaVizite(vizitaInitiala);

        /// 3. Calculăm top-urile in funcție de o strategie aleasă și le afișăm
        AfiseazaTitluSectiune("3. TOPURI VIZITE");
        TesteazaRecomandareSiteuri(vizitaDeduplicata);
        TesteazaClasamentDomenii(vizitaDeduplicata);

        /// 4. Afisam contorii statici ai claselor.
        AfiseazaTitluSectiune("4. CONTORI");
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