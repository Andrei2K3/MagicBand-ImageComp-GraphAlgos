# BANDA MAGICA

Acest proiect este structurat in trei parti principale, concentrandu-se pe implementarea si utilizarea unei liste dublu inlantuite, a unei stive si a unei cozi, fiecare cu operatiile si reprezentarile de date specifice lor.

## Detalii Implementare

- **Prima Parte**: Implementare lista si operatiile aferente acesteia.
- **A Doua Parte**: Stiva (informatia o reprezinta un pointer la degetul din lista) si operatiile specifice ei.
- **A Treia Parte**: Coada (informatia din coada este data de o structura care memoreaza comanda, un sir,  si un caracter care conteaza, daca comanda respectiva implica caracter, sau nu conteaza) cu functiile specifice.

## Rezolvarea Problemei si Citirea Datelor

In timpul citirii datelor am facut si rezolvarea efectiva a problemei.
Operatiile posibile sunt:

### Operatiuni de Actualizare (UPDATE):
- MOVE_LEFT;
- MOVE_RIGHT;
- MOVE_LEFT_CHAR <C>;
- MOVE_RIGHT_CHAR <C>;
- WRITE <C>;
- INSERT_LEFT <C>;
- INSERT_RIGHT <C>;

### Operatiuni de Interogare (QUERY):
- SHOW_CURRENT;
- SHOW;

### UNDO/REDO si EXECUTE

Se observa ca cele care contin "<C>" au lungimile(lungimea comenzii fara <C>) 14, 15, 5, 11 si sunt singurele cu aceste lungimi si mai exista si comanda INSERT_RIGHT cu aceeasi lungime ca SHOW_CURRENT, iar din acest motiv am adaugat conditia auxiliara lg == 12 && D.comanda[0] == 'I'.

Cand intalnim una din aceste 5 comenzi(care sunt de tip UPDATE) le adaugam in coada. La fel procedam si cu celelalte comenzi de tip UPDATE.

Daca intalnim operatii de tip QUERY sau UNDO/REDO, executam operatiile instant.

La intalnirea comenzii EXECUTE: extrag din coada o comanda si o execut.
