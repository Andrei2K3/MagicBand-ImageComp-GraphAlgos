# Tema 3

Aceasta documentatie detaliaza solutiile adoptate pentru rezolvarea celor doua cerinte ale temei.

## Definirea INF

Am definit constanta `INF` (infinit) ca fiind `2^29`, un numar suficient de mare, avand grija ca `2*INF` sa fie mai mic decat `INT_MAX`, folosindu-ne de operatiile pe biti.

## Cerinta 1
    
Am rezolvat-o prin intermediul functiei task1().

Citim datele din fisier, insa am intampinat o problema: cum facem convertirea nodurilor date ca stringuri intr-un tip de date care sa ne multumeasca(int)? Strategia pe care am abordat-o este urmatoarea: asignam fiecarui nod(dat ca string) o valoare de la 1 la N astfel: mi-am luat un vector de cuvinte encodare_noduri in care cand intalnesc un nod nou(care nu se afla in encodare_noduri) adaug la final string-ul asociat nodului nou. O observatie esentiala este ca encodare_noduri[i] are semnificatia: nodul i din graf corespunde string-ului encodare_noduri[i] din fisiserul de intrare.

- **Punctele a) si b)**: marcam fiecare nod initial ca fiind nevizitat, apoi aplicam de cate ori este cazul DFS pentru a determina numarul componentelor conexe. In acelasi timp aplicam si Algoritmul lui Prim pentru a determina arborii partiali de cost minim. De regula, Algoritmul lui Prim se aplica pe un graf ponderat neorientat CONEX, insa eu l-am adaptat pentru grafuri care nu sunt neaparat conexe. Singura modificare facuta este adaugare acelui if de la linia 98 din cod.

Mai raman de afisat datele cerute. 

## Cerinta 2

Citim de doua ori din fisierul de intrare.
- Din **prima citire** aflam nodurile care corespund insulei si corabiei asltfel: marcam intr-un vector de aparitii, aparitia fiecarui nod de tipul Nod_x adica Aparitii_Noduri[x]=1. Cum avem un graf de N noduri si N-2 noduri corespund tipului descris anterior, ne rezulta 2 noduri care nu vor fi marcate si acestea devin de fapt insula si corabia. Eu am ales sa marchez ca primul nod de la 1 la N care n-a fost marcat sa fie insula, iar celalalt corabia.

- Cea de-a **doua citire** corespunde rezolvarii propriu-zise a problemei. Aplicam DFS din nodul corabie sa vedem daca putem ajunge la insula, aplicam si de la insula la corabie tot din acelasi motiv. Cum am tratat aceste 2 cazuri particulare, trecem sa facem ceea ce cerea cu adevarat cerinta :). Aplicam Dijkstra din nodul insula. Aceasta functie ne va intoarce un vector de predecesori(in literatura de specialitate mai apare si sub numele de vector de tati). Prin intermediul acestui vector vom afisa drumul de la insula la corabie, vom determina costul total al drumului si vom determina adancimea minima a apei. Numarul de drumuri este dat de partea intreaga superioara a fractiei G_comoara/adancime_minima_apa care in C se mai poate scrie ca (G_comoara + adancime_minima_apa - 1) / adancime_minima_apa.
