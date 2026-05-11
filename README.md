# Proiect2POO – Sistem de Documentare pe Internet

## Descriere

Acest proiect C++ modelează un sistem simplificat de documentare pe internet. Aplicația gestionează domenii web, adrese web și vizite, apoi calculează relevanța resurselor și generează recomandări de site-uri pe baza istoricului de navigare.

Proiectul reprezintă continuarea proiectului anterior și extinde implementarea inițială prin concepte de Programare Orientată pe Obiecte cerute pentru Tema 2:

- moștenire și polimorfism;
- clasă de bază abstractă;
- funcții virtuale pure;
- clonare polimorfică;
- pointer la clasa de bază;
- `dynamic_cast`;
- copy-and-swap;
- STL: `string`, `vector`, `unique_ptr`;
- smart pointers;
- excepții proprii.

---

## Tema proiectului

**35. InternetDocumentare -> Domenii, AdreseWeb, Vizite**

Programul urmărește să creeze un mod prin care un utilizator poate analiza resurse web folosite în procesul de documentare. Sunt modelate:

- domenii web;
- adrese web complete;
- vizite efectuate pe site-uri;
- resurse selectate pentru analiză;
- colecții de resurse internet.

---

## Clase implementate
1. InternetException

Clasă de bază pentru excepțiile proprii ale proiectului.

Clase derivate:
- DomeniuInvalidException
- AdresaInvalidaException
- VizitaInvalidaException

Acestea sunt folosite pentru tratarea erorilor specifice proiectului, cum ar fi:
- domeniu invalid;
- protocol invalid;
- index invalid pentru vizite;
- vectori de dimensiuni diferite în clasa Vizita.

2. ResursaInternet

Clasă de bază abstractă pentru resursele din proiect.

Caracteristici:
- conține atributul categorie;
- are contor static de instanțe;
- definește interfețe publice non-virtuale care apelează funcții virtuale;
- conține funcții virtuale pure.

Funcții importante:
- int Relevanta() const;
- void NormalizeazaResursa();
- void Afiseaza(std::ostream& out) const;
- virtual int CalculeazaRelevanta() const = 0;
- virtual void Normalizeaza() = 0;
- virtual ResursaInternet* clone() const = 0;
- virtual void AfisareVirtuala(std::ostream& out) const = 0;

Această clasă permite tratarea polimorfică a obiectelor de tip Domeniu, AdresaWeb și Vizita.

3. Domeniu

Clasă derivată din ResursaInternet.

Reprezintă un domeniu web.

Atribute:
- std::string nume;
- std::string extensie;

Exemple:
- google.com
- wikipedia.org
- harvard.edu

Funcționalități:
- normalizează numele domeniului la litere mici;
- adaugă automat punctul în fața extensiei dacă lipsește;
- validează extensii cunoscute;
- calculează relevanța domeniului în funcție de extensie;
- implementează clone();
- suprascrie funcțiile virtuale din clasa de bază.

Scorul de relevanță pentru extensii:
- .edu -> 100
- .org -> 85
- .ro  -> 70
- .com -> 60
- .net -> 55
- altă extensie -> 40

4. AdresaWeb

Clasă derivată din ResursaInternet.

Reprezintă o adresă web completă.

Atribute:
- Domeniu domeniu;
- std::string protocol;
- bool sigur;

Exemple:
- https://google.com
- http://facebook.com

Funcționalități:
- compune clasa Domeniu;
- validează protocolul;
- acceptă doar http și https;
- setează automat atributul sigur în funcție de protocol;
- calculează relevanța adresei pe baza domeniului, protocolului și securității;
- implementează clone();
- permite afișare polimorfică.

Regulă de securitate:
- https -> sigur = true
- http  -> sigur = false

5. Vizita

Clasă derivată din ResursaInternet.

Reprezintă istoricul de vizite al utilizatorului.

Atribute
- std::vector<AdresaWeb> adrese;
- std::vector<std::string> date;
- std::vector<int> clicuri;

Funcționalități:
- gestionează mai multe vizite;
- validează datele;
- normalizează adresele web;
- corectează numărul de clicuri dacă este prea mic;
- elimină duplicatele;
- cumulează clicurile pentru același domeniu;
- concatenează datele vizitelor duplicate;
- calculează scoruri de recomandare;
- implementează clone().

Validări aplicate:
- data invalidă -> "Data necunoscuta"
- clicuri < 2   -> 2
- protocol invalid -> excepție
- dimensiuni diferite pentru vectori -> excepție

6. ResursaSelectata

Clasă care conține un pointer la clasa de bază:

ResursaInternet* resursa;

Scopul clasei este testarea gestiunii manuale a unei resurse polimorfice.

Funcționalități:
- copiere polimorfică prin clone();
- destructor care eliberează memoria;
- operator de atribuire implementat prin copy-and-swap;
- verificare prin dynamic_cast dacă resursa este de tip Vizita;
- calcularea relevanței resursei selectate.

Metode relevante:
- bool ContineVizita() const;
- int NumarViziteDacaEsteVizita() const;
- int CalculeazaRelevantaResursa() const;

7. ColectieResurse

Clasă care folosește STL și smart pointers.

Atribute:
- std::vector<std::unique_ptr<ResursaInternet>> resurse;

Funcționalități:
- stochează resurse internet polimorfice;
- folosește unique_ptr pentru gestiunea automată a memoriei;
- copiază resursele prin clone();
- normalizează toate resursele;
- sortează resursele descrescător după relevanță;
- calculează relevanța medie;
- numără câte resurse sunt de tip Vizita.
- Funcționalități principale

1. Crearea istoricului inițial

În main.cpp, istoricul inițial este creat prin funcții auxiliare:
- CreeazaAdreseInitiale();
- CreeazaDateInitiale();
- CreeazaClicuriInitiale();
- CreeazaVizitaInitiala();

Datele includ intenționat duplicate pentru a demonstra funcționalitatea de deduplicare.

Exemple de site-uri folosite:
- google.com
- youtube.com
- facebook.com

2. Validarea vizitelor

Metoda:
- ValidareVizite()

Aceasta verifică:
- dacă vectorii adrese, date și clicuri au aceeași dimensiune;
- dacă fiecare adresă este normalizată;
- dacă data are format minim valid;
- dacă numărul de clicuri este cel puțin 2.

3. Deduplicarea vizitelor

Metoda:
- CuratareVizite()

Aceasta elimină duplicatele pe baza:
- numelui domeniului;
- extensiei domeniului.

Pentru vizitele duplicate:
- clicurile sunt adunate;
- datele sunt concatenate cu separatorul /.

Exemplu conceptual:
- google.com, 12 clicuri
- google.com, 15 clicuri

devine:
- google.com, 27 clicuri

4. Recomandarea site-urilor

Metoda:
- RecomandareSite()

Calculează un scor pentru fiecare site folosind:
- numărul de clicuri;
- relevanța adresei web;
- numărul de vizite asociate.

Formula folosită în implementare:
- scor = 0.50 * clicuri + 0.30 * scorAdresa + 0.20 * numarDate
Scorul rezultat înlocuiește valoarea din vectorul clicuri, iar apoi site-urile sunt ordonate pentru generarea topului de recomandări.

5. Testarea moștenirii și polimorfismului

Programul creează un vector de pointeri la clasa de bază:

std::vector<ResursaInternet*> resursePolimorfice;

În acest vector sunt adăugate obiecte de tip:
- Domeniu
- AdresaWeb
- Vizita

Acestea sunt tratate prin interfața comună ResursaInternet.

6. Testarea clasei ResursaSelectata

Se demonstrează:
- folosirea unui atribut de tip ResursaInternet*;
- copierea polimorfică;
- apelarea metodei clone();
- folosirea dynamic_cast;
- verificarea dacă resursa selectată este de tip Vizita.

7. Testarea copy-and-swap

Operatorul de atribuire din ResursaSelectata este implementat prin tehnica copy-and-swap:

ResursaSelectata& operator=(ResursaSelectata R);

Această abordare oferă o metodă sigură de atribuire, deoarece lucrează cu o copie locală și apoi schimbă resursele interne.

8. Testarea smart pointers și STL

Clasa ColectieResurse demonstrează folosirea:

std::vector<std::unique_ptr<ResursaInternet>>

Aceasta permite stocarea polimorfică a resurselor fără gestionarea manuală directă a memoriei.

Funcționalități testate:
- adăugarea resurselor;
- normalizarea tuturor resurselor;
- sortarea după relevanță;
- calcularea relevanței medii;
- numărarea obiectelor de tip Vizita.

9. Testarea excepțiilor proprii

Programul creează intenționat o adresă web invalidă:

AdresaWeb adresaInvalida(Domeniu("site", ".com"), "ftp", true);

Protocolul ftp nu este acceptat, deci se aruncă o excepție de tip AdresaInvalidaException.

## Rularea
Programul execută următoarele etape:

1. ISTORIC VIZITE
2. DEDUPLICARE VIZITE
3. RECOMANDARE SITE-URI
4. TEST MOSTENIRE SI POLIMORFISM
5. TEST CLASA CU ATRIBUT POINTER LA BAZA
6. TEST COPY AND SWAP
7. TEST SMART POINTERS SI COLECTIE STL
8. TEST EXCEPTII PROPRII
9. CONTORI
Concepte OOP demonstrate

Proiectul demonstrează următoarele concepte:
- încapsulare;
- constructori și destructori;
- constructor de copiere;
- operator de atribuire;
- moștenire;
- polimorfism;
- funcții virtuale pure;
- clasă abstractă;
- suprascriere de metode;
- supraincărcarea operatorului <<;
- compoziție;
- clonare polimorfică;
- downcasting cu dynamic_cast;
- gestiunea resurselor cu unique_ptr;
- copy-and-swap;
- excepții proprii.