**Postelnicu Cristina-Marina**
**313CA**

## Segregated Free Lists TEMA 1 ##

### Descriere:

* Tema este alcatuita din functii care au rolul de a gestiona memoria pusa la
dispozitie in vectorul de liste. Programul foloseste ,in primul rand, functiile
de creare a unei liste dublu inlantuite, de adaugare a unui nod pe pozitia n,
de stergere a unui nod de pe pozitia n si de eliberare a memoriei folosite 
pentru o lista o lista dublu inlantuita.
* Structura sfl contine vectorul de liste pe care l am denumit heap, adresa
de start, numarul de bytes per lista, numarul de liste din vector, numarul
de apelari ale functiei malloc_function, functiei free_function si numarul de
fragmentari efectuate si tipul de reconstituire. In structura corespunzatoare
a unei liste dublu inlantuite am adaugat nr_bytes pentru a stii numarul de
bytes al nodurilor din fiecare lista.
* Functia init_heap este folosita pentru alocarea memoriei pentru vectorul de
liste si pentru listele prezente in acesta si pentru adaugarea in fiecare nod a
dimensiunii si adresei acestuia. Fiecare lista are un total de bytes egal cu
nr_bytes_lists introdus de la tastatura, astfel fiecare lista contine
noduri cu dimensiuni incepand de la 8, avand numarul necesar de noduri
pentru a ajunge la nr_bytes_lists. Astfel se folosesc variabilele in care
se actualizeaza dimensiunea si adresa pe masura ce se adauga alte liste pana
se atinge nr_lists introdus de la tastatura.
* Functiile add_in_order si add_new_list au rolul de a ajuta la implemantarea
functiilor malloc_function si free_function. Add_in_order adauga noduri in
lista in ordinea crescatoare a adreselor, iar add_new_list adauga o noua lista
in vector in ordinea crescatoare a dimensiunilor.
* Functia malloc_function are rolul de a sterge un nod cu un numar de bytes
citit de la tastatura din lista corespunzatoare din vectorul de liste si de a
adauga acest nod intr o lista cu noduri alocate, pe care am numit o allocated.
Pentru a vedea daca avem suficienta memorie se calculeaza maximul de bytes
dintr o lista din vector, iar daca numarul este mai mic decat nr de bytes citit
se afiseaza mesajul "Out of memory". Exista inca doua cazuri ce sunt tratate in
functie, cazul in care nr_bytes este egal cu dimenisunea unui bloc si cel in
care nr_bytes este mai mic decat orice bloc disponibil. In cazul de egalitate
nodul gasit (care va avea adresa minima datorita adaugarii in ordinea 
crescatoare a adresei) este sters din lista acestuia din vectorul de liste
si apoi se adauga in lista allocated, folosindu se dimensiunea si adresa sa
prin stocarea intr un nod auxiliar returnat prin functia de stergere a unui
nod. In cel de al doilea caz, mai intai se adauga in allocated un nod cu
numarul de bytes citit si cu adresa blocului care va fi fragmentat si sters
din vectorul de liste. Se calculeaza cati bytes raman liberi din acest bloc
si adresa acestuia, care este in continuarea blocului alocat, dupa care se
adauga in vectorul de liste, insa exista doua cazuri pentru aceasta adaugare.
Se verifica daca exista deja o lista cu noduri care au aceeasi dimensiune ca
blocul ramas, iar daca raspunsul este pozitiv, blocul se adauga in aceasta
lista, in caz contrar se realoca vectorul de liste si se adauga o noua lista
pentru acesta, fiind adaugat, de asemenea in ordine crescatoare.
* Functia free_function are rolul de a sterge un nod cu o anumita adresa din
lista allocated. Daca la acea adresa nu s a alocat niciun nod, se va afisa
mesajul "Invalid free". Daca se gaseste adresa, se sterge nodul din allocated
si acesta trebuie adaugat in lista corespunzatoare din vectorul de liste, in
acelasi fel in care s a adaugat in functia malloc_function prezentata mai sus.
* Functia write are rolul de a scrie un string in nodurile alocate. La inceput
se citeste adresa la care se va scrie, dupa care se citeste restul liniei cu
fgets, pentru a se folosi in continuare strtok pentru a obtine pe rand sirul
de caractere si numarul de bytes care trebuie scris. Dupa ce aceste informatii
au fost obtinute sunt stocate in data si nr_bytes. Daca adresa la care se
incearca scrierea nu se gaseste in allocated, se returneaza valoarea -1, care
va fi tratata in main, unde se afiseaza mesajul "Segmentation fault" si se
apeleaza dump_memory si destroy_heap. Acelasi lucru se intampla si daca
zona in care se scrie nu este continua. De asemenea, se verifica daca numarul
de bytes este mai mare decat sirul de caractere, acesta primind lungimea
acestuia daca se intampla acest lucru. Daca sirul de caractere se poate scrie
pe un singur nod, se verifica daca exista deja date in acesta si acestea
se suprascriu pana la numarul de bytes al noului sir de caractere, iar daca
sirul de caractere nu poate fi continut intr un singur nod, va fi scris pe
parcursul a mai multor noduri atata timp cat sunt alocate adresele urmatoare.
* Functia read are rolul de a afisa informatiile scrise anterior, avand
aceleasi cazuri care se sfarsesc prin segmentation fault. De asemenea,
se citeste un singur nod sau mai multe in functie de nr_bytes citit.
* Functia dump_memory caluleaza memoria alocata, memoria libera si memoria
totala pe care ulterior le si afiseaza. Aceasta afiseaza numarul de apeluri
ale functiei malloc_function si numarul de fragmentari efectuate in aceasta,
cat si numarul de apeluri ale functiei free_function. Se afiseaza toate
blocurile libere si blocurile alocate, cat numarul, adresele si dimensiunile
lor.
* Functia destroy_heap elibereaza toata memoria folosita in program.

