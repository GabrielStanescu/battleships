# battleships
A version of the Battleships game made in C using ncurses library

CERINTA 1
Am scris o functie care creeaza un window cu cele 3 optiuni pentru meniu.
Initial, optiunea "Resume Game" este blocata, fiind deblocata la inceperea meciului.
Optiunile sunt vizibile cu highlight (reverse attron).

CERINTA 2
Harta jucatorului se preia din terminal la executia executabilului. Este o matrice
10x10 care contine 1 sau 0 (1=nava, 0=gol)
Exemplu:
0111100001
0000001101
1110100001
0000001100
1101000001
0000000000
1000010000
0000000000
0000000000
0000000000
Citesc linie cu linie si introduc harta pe o SINGURA linie intr-o matrice care
stocheaza toate hartile din input. Matricea are [nr harti] linii si 100 coloane.
De exemplu, a 3-a linie din matrice contine a 3-a harta (a[2][0-99]).

Harta calculatorului este generata random.
Am folosit un vector v[100], unde v[i]=i, pe care l-am shuffle-uit. Astfel,
vectorul continea numere de la 0 la 99 pe pozitii diferite. Acestea sunt
pozitiile pe care le va incerca calculatorul daca poate sa construiasca o nava
pe pozitia respectiva. Este mult mai eficient (aprox cu 20 secunde) decat sa
aleg un numar random 0-99 de fiecare data cand vreau sa construiesc o nava.
Cand obtin o pozitie, directia este generata random (un numar de la 0 la 3).
0=construieste in sus, 1 in dreapta, 2 jos si 3 in stanga. Apoi verific cu mai
multe functii daca pot sa construiesc.

Un mic bonus este ca am implementat posibilitatea jucatorului de a alege cu ce
harta vrea sa joace. Acesta o alege de la inceput folosind sagetile stanga-dreapta
si apoi apasand Enter. Hartile pot fi previzualizate inainte de alegere.

CERINTA 3
Pozitia jucatorului pe harta ai-ului este marcata cu "@".
Simboluri: "X" harta nelovita, "O" harta gasita/lovita, "-" ratare.
Jucatorul nu poate alege de mai multe ori o nava gasita sau o ratare.
Jucatorul foloseste sagetile pentru deplasare si Enter pentru alegere.

Calculatorul alege o celula din harta jucatorului in mod random si la un
interval stabilit de 3 secunde.

CERINTA 4
Randomize Map
Am cautat navele ramase ale jucatorului si am introdus numarul lor intr-un vector.
Observatie: daca de exemplu o nava de 3 este lovita odata la jumatate, ea devine
o nava de 2.
Dupa ce am obtinut numarul de nave pentru fiecare lungime, am umplut matricea
cu valori de "0" si am folosit un algoritm identic cu cel al randomizarii
hartii calculatorului pentru a repozitiona navele jucatorului.

Destroy in Advance
Pentru a distruge navele de pe harta jucatorului am folosit functia prin care
este randul calculatorului (de 10 ori).
Pentru a distruge navele de pe harta calculatorului am scris o functie separata
care verifica pe pozitii random daca poate sa o incerce (de 10 ori)

Randomize Map se activeaza folosind tasta "r"
Destroy in Advance se activeaza folosind tasta "d"

Optiunile pot fi folosite o singura data in tot meciul.

CERINTA 5
Dupa terminarea jocului am deschis o fereastra care afiseaza cine a castigat si
cate nave a doborat invinsul.

Un alt mic bonus este ca am implementat posibilitatea unei remize in cazul
in care Destroy in Advance distruge ultimele nave atat playerului cat si
calculatorului. Astfel, se va afisa "REMIZA"
