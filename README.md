# Proiect3POO – Sistem de Documentare pe Internet

## Descriere generală

Acest proiect modelează un sistem simplificat de documentare pe internet. Aplicația gestionează domenii web, adrese web și vizite efectuate de un utilizator, apoi calculează relevanța resurselor și generează clasamente de recomandare pe baza istoricului de navigare.

Datele sunt citite din fișierul `sites.in`, apoi sunt validate, normalizate, deduplicate și folosite pentru generarea unor topuri.

Proiectul reprezintă continuarea proiectelor anterioare și include clase template și două design pattern-uri: **Template Method** și **Strategy**.

---

## Modificări față de proiectele anterioare

Față de Proiectul 2, Proiectul 3 adaugă următoarele elemente:

- citirea datelor din fișier;
- folosirea clasei template `Clasament<ElementType>`;
- implementarea pattern-ului **Template Method** în clasa `ResursaInternet`;
- implementarea pattern-ului **Strategy** pentru algoritmul de recomandare;

---

## Structura proiectului

Proiectul este împărțit în trei fișiere principale:

- `main.cpp` – controlează fluxul principal al programului;
- `classes.h` – conține declarațiile claselor;
- `classes.cpp` – conține implementările metodelor.

Fișierul de intrare:

- `sites.in` – conține datele despre vizite.

Formatul conceptual al datelor citite:
```
numar_vizite
domeniu extensie protocol sigur data clicuri
```

Exemplu:
3
google com https 1 12.05.2025-10:30 15
wikipedia org https 1 13.05.2025-11:00 20
google com https 1 14.05.2025-12:20 10

## Clase implementate:
1. InternetException
InternetException este clasa de bază pentru excepțiile proprii ale proiectului.

Clase derivate:
- DomeniuInvalidException;
- AdresaInvalidaException;
- VizitaInvalidaException.

Aceste clase sunt folosite pentru tratarea erorilor specifice aplicației, cum ar fi:
- domeniu invalid;
- extensie goală;
- protocol invalid;
- index invalid;
- vectori de dimensiuni diferite în clasa Vizita.

2. ResursaInternet
ResursaInternet este clasa de bază abstractă a proiectului.

Clase derivate:
- Domeniu;
- AdresaWeb;
- Vizita.

Atribute importante:
- std::string categorie;
- static int contor;

Metode importante:
- int Relevanta() const;
- void NormalizeazaResursa();
- void Afiseaza(std::ostream& out) const;
- virtual int CalculeazaRelevanta() const = 0;
- virtual void Normalizeaza() = 0;
- virtual ResursaInternet* clone() const = 0;
- virtual void AfisareVirtuala(std::ostream& out) const = 0;

Această clasă permite tratarea obiectelor derivate printr-o interfață comună. De asemenea, implementează pattern-ul Template Method, deoarece metodele publice definesc scheletul operațiilor, iar clasele derivate implementează pașii concreți.

3. Domeniu
Clasa Domeniu reprezintă un domeniu web.

Atribute:
- std::string nume;
- std::string extensie;

Exemple:
- google.com
- wikipedia.org
- harvard.edu

Funcționalități:
- normalizează numele domeniului la litere mici;
- adaugă automat punct în fața extensiei;
- validează extensia;
- calculează relevanța domeniului;
- implementează clone();
- suprascrie metodele virtuale din ResursaInternet.

Scoruri de relevanță:
- .edu -> 100
- .org -> 85
- .ro  -> 70
- .com -> 60
- .net -> 55
- altă extensie -> 40

4. AdresaWeb
Clasa AdresaWeb reprezintă o adresă web completă.

Atribute:
- Domeniu domeniu;
- std::string protocol;
- bool sigur;

Exemple:
- https://google.com
- http://example.net

Funcționalități:
- conține un obiect de tip Domeniu;
- validează protocolul;
- acceptă doar http și https;
- stabilește automat dacă adresa este sigură;
- calculează relevanța în funcție de domeniu, protocol și securitate;
- implementează clone().

Regulă:
- https -> sigur = true
- http  -> sigur = false

Dacă protocolul este diferit de http sau https, se aruncă AdresaInvalidaException.

5. Vizita
Clasa Vizita reprezintă istoricul de vizite al utilizatorului.

Atribute:
- std::vector<AdresaWeb> adrese;
- std::vector<std::string> date;
- std::vector<int> clicuri;

Funcționalități:
- gestionează mai multe vizite;
- validează dimensiunile vectorilor;
- normalizează adresele;
- verifică datele;
- corectează numărul de clicuri;
- elimină duplicatele;
- cumulează clicurile pentru același domeniu;
- calculează relevanța medie;
- aplică o strategie de recomandare.

Validări:
- data invalidă -> "Data necunoscuta"
- clicuri < 2   -> 2
- vectori de dimensiuni diferite -> excepție

Metoda de deduplicare -> Vizita CuratareVizite(const Vizita& vizitaCurenta);
Aceasta elimină duplicatele pe baza numelui și extensiei domeniului. Dacă același domeniu apare de mai multe ori, clicurile sunt adunate, iar datele sunt concatenate.

6. StrategieRecomandare
Aceasta este clasa de bază pentru pattern-ul Strategy.

class StrategieRecomandare
{
public:
    virtual ~StrategieRecomandare();

    virtual int CalculeazaScor(const AdresaWeb& adresa,
                               int clicuri,
                               int numarAccesari) const = 0;

    virtual std::string getNumeStrategie() const = 0;
};

Clase concrete:
- StrategieRecomandareStandard;
- StrategieRecomandareSiguranta.

În programul curent este folosită strategia: StrategieRecomandareSiguranta strategie;
Aceasta favorizează site-urile sigure, în special cele cu protocol https.

7. Clasament<ElementType>
Clasament<ElementType> este o clasă template folosită pentru a genera topuri.

Este utilizată pentru:
- Clasament<AdresaWeb>
- Clasament<Domeniu>

Atribute:
- std::vector<ElementType> elemente;
- std::vector<int> scoruri;

Funcționalități:
- adaugă elemente cu scor;
- returnează elemente și scoruri;
- sortează descrescător;
- afișează primele elemente din clasament.

Metode importante:
- void AdaugaElement(const ElementType& element, int scor);
- void SorteazaDescrescator();
- void Afiseaza(std::ostream& out, int limita) const;

Această clasă demonstrează folosirea template-urilor în C++.

8. ResursaSelectata
Clasa ResursaSelectata conține un pointer la clasa de bază: ResursaInternet* resursa;

Aceasta demonstrează:
- gestiunea unei resurse polimorfice;
- clonarea prin clone();
- destructor propriu;
- copy-and-swap;
- folosirea dynamic_cast.

Deși clasa nu este apelată în fluxul curent din main.cpp, ea rămâne implementată în proiect pentru a demonstra mecanismele cerute la Tema 2.

9. ColectieResurse
Clasa ColectieResurse folosește smart pointers:

std::vector<std::unique_ptr<ResursaInternet>> resurse;

Funcționalități:
- stochează resurse polimorfice;
- folosește unique_ptr;
- copiază resursele prin clone();
- normalizează toate resursele;
- sortează resursele după relevanță;
- calculează relevanța medie;
- numără câte resurse sunt de tip Vizita.

Această clasă demonstrează folosirea STL și a smart pointerilor în context polimorfic.

## Design patterns:

1. Template Method
Pattern-ul Template Method este implementat în clasa ResursaInternet.

Metodele publice:
- int Relevanta() const;
- void NormalizeazaResursa();
- void Afiseaza(std::ostream& out) const;

apelează metode virtuale pure:
- virtual int CalculeazaRelevanta() const = 0;
- virtual void Normalizeaza() = 0;
- virtual void AfisareVirtuala(std::ostream& out) const = 0;

Astfel, clasa de bază definește pașii generali ai algoritmului, iar clasele derivate oferă implementarea concretă.

Exemplu:
- Domeniu::CalculeazaRelevanta() calculează scorul după extensie;
- AdresaWeb::CalculeazaRelevanta() calculează scorul după domeniu, protocol și securitate;
- Vizita::CalculeazaRelevanta() calculează media scorurilor vizitelor.

Avantajul acestui pattern este că oferă o interfață uniformă pentru toate resursele internet, dar permite comportament diferit în clasele derivate.

2. Strategy
Pattern-ul Strategy este folosit pentru recomandarea site-urilor.

Clasa Vizita nu calculează direct scorul de recomandare printr-o formulă fixă, ci primește o strategie externă: void RecomandareSite(const StrategieRecomandare& strategie);

Această metodă apelează: strategie.CalculeazaScor(adresa, clicuri, numarAccesari);

Clasele concrete de strategie sunt:
- StrategieRecomandareStandard;
- StrategieRecomandareSiguranta.

În programul curent este folosită StrategieRecomandareSiguranta, care acordă scoruri mai mari adreselor sigure.
Avantajul acestui pattern este că algoritmul de recomandare poate fi schimbat fără modificarea clasei Vizita.

## Fluxul programului

Programul execută următoarele etape:
- Citește datele din sites.in.
- Construiește istoricul inițial de vizite.
- Afișează istoricul inițial.
- Elimină duplicatele.
- Afișează istoricul după deduplicare.
- Aplică o strategie de recomandare.
- Generează un clasament al adreselor web.
- Generează un clasament al domeniilor.
- Afișează contorii statici ai claselor.

Etapele afișate în consolă sunt:
1. ISTORIC VIZITE
2. DEDUPLICARE VIZITE
3. TOPURI VIZITE
4. CONTORI

## Compilare

Compilare recomandată:
g++ main.cpp classes.cpp -o main.exe

Rulare:
./main.exe

# REVIEW 
Proiectul „Sistem de Documentare pe Internet” urmărește modelarea unui scenariu realist în care un utilizator analizează resurse web folosite pentru informare și documentare. Un domeniu reprezintă baza unei resurse web, o adresă web extinde această informație prin protocol și stare de siguranță, iar o vizită reține interacțiunea utilizatorului cu aceste adrese. Prin această structură, proiectul nu rămâne la nivelul unor clase independente, ci construiește un model coerent și apropiat de un caz real.

Elementul central al proiectului este clasa abstractă `ResursaInternet`. Aceasta definește o interfață comună pentru toate resursele gestionate de aplicație. Clasele `Domeniu`, `AdresaWeb` și `Vizita` moștenesc această clasă și implementează propriile variante pentru calcularea relevanței, normalizare, afișare și clonare. Domeniul este evaluat în funcție de extensie, adresa web este evaluată în funcție de domeniu, protocol și siguranță, iar vizita este evaluată pe baza istoricului de adrese și a numărului de clicuri. Astfel, moștenirea este folosită pentru a evidenția relația conceptuală dintre obiecte și pentru a evita duplicarea codului.

Moștenirea și polimorfismul sunt folosite în mod corect și au un rol important în proiect. Metodele virtuale pure din `ResursaInternet` obligă clasele derivate să implementeze comportamentele esențiale. Compilatorul garantează astfel că orice clasă derivată oferă o implementare pentru calcularea relevanței, normalizare și afișare. Metoda `clone()` este importantă deoarece permite copierea polimorfică fără pierderea tipului real al obiectului. Aceasta este utilizată în clase precum `ResursaSelectata` și `ColectieResurse`, unde obiectele sunt manipulate prin pointeri sau smart pointeri la clasa de bază.

Primul design pattern folosit este Template Method. Acesta apare în clasa `ResursaInternet`, prin metodele publice `Relevanta()`, `NormalizeazaResursa()` și `Afiseaza()`. Aceste metode definesc pașii generali ai operațiilor, dar delegă implementarea concretă către metode virtuale precum `CalculeazaRelevanta()`, `Normalizeaza()` și `AfisareVirtuala()`. Alegerea este potrivită deoarece toate resursele trebuie să poată fi normalizate, evaluate și afișate, însă fiecare tip de resursă face aceste operații diferit. Pattern-ul oferă o interfață stabilă și ascunde detaliile particulare ale claselor derivate. De exemplu, un domeniu normalizează extensia și numele, o adresă web normalizează protocolul și verifică siguranța, iar o vizită normalizează toate datele interne și verifică validitatea vectorilor.

Al doilea design pattern folosit este Strategy. Acesta este aplicat în mecanismul de recomandare a site-urilor. Clasa `Vizita` nu conține direct o formulă fixă de recomandare, ci primește un obiect de tip `StrategieRecomandare`. Clasele `StrategieRecomandareStandard` și `StrategieRecomandareSiguranta` oferă algoritmi diferiți pentru calcularea scorului. În fluxul principal este folosită strategia bazată pe siguranță, care favorizează adresele cu protocol `https` și resursele considerate sigure. Avantajul pattern-ului este că algoritmul poate fi schimbat fără modificarea clasei `Vizita`. 

Clasa template `Clasament<ElementType>` reprezintă o completare utilă pentru proiect. Ea permite generarea de topuri pentru tipuri diferite fără duplicarea codului. În aplicație, aceeași clasă este folosită pentru `Clasament<AdresaWeb>` și `Clasament<Domeniu>`. Astfel, logica de adăugare a elementelor, memorare a scorurilor, sortare descrescătoare și afișare este reutilizată.

STL este folosit în mod constant și justificat. `std::string` apare pentru nume de domenii, extensii, protocoale, categorii și date. `std::vector` este folosit pentru listele de adrese, date, clicuri, elemente din clasamente și colecții de resurse. `std::unique_ptr` apare în `ColectieResurse`, unde rezolvă problema gestionării automate a memoriei pentru obiecte polimorfice. Această soluție este mai sigură decât folosirea exclusivă a pointerilor bruți, deoarece eliberarea memoriei se face automat atunci când obiectele ies din scope.

Clasa `ResursaSelectata` rămâne importantă pentru demonstrarea gestiunii manuale a unei resurse polimorfice. Ea conține un pointer de tip `ResursaInternet*`, folosește `clone()` pentru copiere profundă, definește destructor și implementează operatorul de atribuire prin copy-and-swap. Această tehnică oferă siguranță la atribuire, deoarece lucrează cu o copie temporară și apoi interschimbă resursele interne. Clasa folosește și `dynamic_cast` pentru a verifica dacă resursa selectată este de tip `Vizita`, ceea ce demonstrează un downcast cu sens în contextul ierarhiei. Chiar dacă această clasă nu mai este apelată în fluxul actual din `main`, ea rămâne utilă pentru demonstrarea cerințelor din proiectele anterioare și pentru explicarea conceptelor OOP la prezentare.

Validarea datelor este tratată prin excepții proprii. `InternetException` este baza, iar `DomeniuInvalidException`, `AdresaInvalidaException` și `VizitaInvalidaException` diferențiază tipurile de erori. De exemplu, protocolul invalid produce o excepție de adresă web, iar dimensiunile diferite ale vectorilor din `Vizita` produc o excepție specifică vizitelor. 

Funcționalitatea de deduplicare este relevantă pentru tema proiectului. Metoda `CuratareVizite()` identifică domeniile repetate, cumulează clicurile și concatenează datele asociate. Aceasta transformă istoricul brut într-o formă mai utilă pentru analiză și recomandare. După deduplicare, strategia de recomandare modifică scorurile, iar clasa `Clasament` permite afișarea celor mai relevante adrese și domenii.

Un alt aspect pozitiv este faptul că programul are un flux de execuție ușor de urmărit. În `main`, operațiile sunt separate în funcții cu rol clar: creare istoric, afișare, deduplicare, recomandare, generare de clasamente și afișare de contori. Această organizare face codul mai lizibil și ajută la prezentarea proiectului. Fiecare etapă corespunde unei funcționalități concrete, ceea ce permite explicarea rapidă a modului în care datele de intrare sunt transformate în rezultate utile.

Contorii statici ai claselor completează demonstrația, deoarece arată existența unor membri comuni tuturor instanțelor unei clase. Chiar dacă nu sunt esențiali pentru logica aplicației, ei demonstrează utilizarea membrilor statici în C++. Afișarea contorilor statici oferă o imagine asupra numărului de instanțe active pentru fiecare clasă și ajută la observarea modului în care constructorii, copiile, clonele și destructorii sunt apelați pe parcursul execuției.

Există și aspecte care pot fi îmbunătățite. Variabila globală `fin` ar putea fi înlocuită cu un flux local în `main`, transmis prin referință funcției de citire. Metoda `CuratareVizite(const Vizita&)` ar putea deveni `CuratareVizite() const`. De asemenea, sortarea manuală din `Clasament` ar putea fi înlocuită complet cu `std::sort`. O altă posibilă îmbunătățire ar fi introducerea unor structuri suplimentare pentru reprezentarea datelor calendaristice sau pentru serializarea informațiilor într-un fișier de ieșire, într-un mod clar și ușor de citit pentru utilizator.

În concluzie, proiectul realizează o implementare a unui sistem de contorizare si monitorizare a vizitelor web ale unui utilizator oarecare, care oferă informații importante în legătură cu obiceiurile sale de navigare, precum și recomandări relevante. De asemenea, proiectul include moștenire, polimorfism, clasă abstractă, funcții virtuale pure, clonare polimorfică, STL, smart pointers, excepții proprii, citirea datelor dintr-un fișier de intrare, clasă template și două design pattern-uri. 