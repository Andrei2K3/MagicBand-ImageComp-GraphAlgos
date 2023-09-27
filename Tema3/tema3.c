/*CIUCAN Andrei-Alexandru - 313CC*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF (1 << 29)

typedef struct graf
{
    int numar_noduri;
    struct nod **lista_adiacenta;
} graf;

typedef struct nod
{
    float scor;
    int y, cost;
    struct nod *next;
} nod;

/// Inserare nod la inceputul listei
void insert_node(nod **list, int y, int cost)
{
    nod *new = malloc(sizeof(nod));
    new->next = *list;
    new->y = y;
    new->cost = cost;
    *list = new;
}

/// Afisare graf
void print_graf(graf *Graf)
{
    for (int i = 1; i <= Graf->numar_noduri; i++)
    {
        printf("i:%d   ", i);
        for (nod *current = Graf->lista_adiacenta[i]; current; current = current->next)
            printf("%d %lf  ", current->y, current->scor);
        printf("\n");
    }
}

/// Dam free la graf
void eliberare_memorie_graf(graf **Graf)
{
    for (int i = 0; i <= (*Graf)->numar_noduri; i++)
        for (nod *current = (*Graf)->lista_adiacenta[i]; current;)
        {
            nod *temp = current;
            current = current->next;
            free(temp);
        }
    free((*Graf)->lista_adiacenta);
    free(*Graf);
}

/// Dam free la un vector de cuvinte
void eliberare_memorie_encode_nod(int nr, char ***encode)
{
    for (int i = 0; i <= nr; i++)
        free((*encode)[i]);
    free(*encode);
}

void DFS(int nodd, int *viz, graf *Graf)
{
    viz[nodd] = 1;
    for (nod *current = Graf->lista_adiacenta[nodd]; current; current = current->next)
        if (viz[current->y] == 0)
            DFS(current->y, viz, Graf);
}

/// Aplicam algoritmul lui Prim pentru un graf care nu este neaparat conex
int Prim(int nodd, graf *Graf)
{
    int *viz = malloc((Graf->numar_noduri + 1) * sizeof(int));
    int *dist = malloc((Graf->numar_noduri + 1) * sizeof(int));
    // Initializare
    for (int i = 1; i <= Graf->numar_noduri; i++)
    {
        viz[i] = 0;
        dist[i] = INF;
    }
    // Actualizam distantele
    viz[nodd] = 1;
    for (nod *current = Graf->lista_adiacenta[nodd]; current; current = current->next)
        dist[current->y] = current->cost;
    //
    for (int k = 1; k < Graf->numar_noduri; k++) //?
    {
        int minim = INF, poz = -1;
        for (int i = 1; i <= Graf->numar_noduri; i++)
            if (viz[i] == 0 && dist[i] < minim)
            {
                minim = dist[i];
                poz = i;
            }
        if (minim != INF)
        {
            viz[poz] = 1;
            for (int i = 1; i <= Graf->numar_noduri; i++)
            {
                // if(viz[i]==0 && dist[i] > cost[i][y])
                nod *nodulet;
                for (nodulet = Graf->lista_adiacenta[i]; nodulet; nodulet = nodulet->next)
                    if (nodulet->y == poz)
                        break;
                if (nodulet)
                    if (viz[i] == 0 && dist[i] > nodulet->cost)
                        dist[i] = nodulet->cost;
            }
        }
    }
    // Determinam "costul" arborelui
    int cost_arbore = 0;
    for (int i = 1; i <= Graf->numar_noduri; i++)
        if (dist[i] != INF)
            cost_arbore += dist[i];
    // Eliberam memoria
    free(viz);
    free(dist);
    //
    return cost_arbore;
}

int comp(const void *x, const void *y)
{
    return *(int *)x > *(int *)y;
}

void task1()
{
    FILE *f = fopen("tema3.in", "r");
    FILE *g = fopen("tema3.out", "w");
    //
    graf *Graf;
    Graf = malloc(sizeof(graf));
    Graf->numar_noduri = 0;
    Graf->lista_adiacenta = NULL;
    //
    char **encodare_noduri;
    int n; // n=numarul de noduri
    //
    int N, M;
    fscanf(f, "%d%d", &N, &M);
    // Initializare graf
    Graf->numar_noduri = N;
    Graf->lista_adiacenta = malloc((N + 1) * sizeof(nod *));
    for (int i = 0; i <= N; i++)
        Graf->lista_adiacenta[i] = NULL;
    // Initializare vector de cuvinte
    encodare_noduri = malloc((N + 1) * sizeof(char *));
    for (int i = 0; i <= N; i++)
        encodare_noduri[i] = malloc(43 * sizeof(char));
    n = 0;
    // Citire muchii
    for (int i = 1; i <= M; i++)
    {
        int cost;
        char x_string[43], y_string[43], nul[43];
        fscanf(f, "%s%s%d", x_string, y_string, &cost);
        // Analizare noduri
        int x = 0, y = 0;
        for (int k = 1; k <= n; k++)
        {
            if (strcmp(encodare_noduri[k], x_string) == 0)
                x = k;
            if (strcmp(encodare_noduri[k], y_string) == 0)
                y = k;
        }
        if (x == 0)
        {
            n++;
            x = n;
            strcpy(encodare_noduri[n], x_string);
        }
        if (y == 0)
        {
            n++;
            y = n;
            strcpy(encodare_noduri[n], y_string);
        } // Adaugare muchie in graf
        insert_node(&Graf->lista_adiacenta[x], y, cost);
        insert_node(&Graf->lista_adiacenta[y], x, cost);
    }
    // We solve task1
    int nr_comp_conexe = 0;
    int *vizitati = calloc(N + 1, sizeof(*vizitati));
    int *costuri = calloc((N + 1), sizeof(int)), index = 0;
    //
    for (int i = 1; i <= N; i++)
        if (vizitati[i] == 0)
        {
            nr_comp_conexe++;
            DFS(i, vizitati, Graf);
            costuri[++index] += Prim(i, Graf);
        }
    // Sortam raspunsurile
    qsort(&costuri[1], index, sizeof(int), comp);
    // Afisam datele cerute
    fprintf(g, "%d\n", nr_comp_conexe);
    for (int i = 1; i <= index; i++)
        fprintf(g, "%d\n", costuri[i]);
    // Eliberam memoria
    free(costuri);
    free(vizitati);
    eliberare_memorie_graf(&Graf);
    eliberare_memorie_encode_nod(N, &encodare_noduri);
    // Inchidem fisierele
    fclose(f);
    fclose(g);
}

/// "Convertim" un string la un numar
int string_to_decimal(char *s, int ins, int crb)
{
    if (strcmp(s, "Insula") == 0)
        return ins;
    if (strcmp(s, "Corabie") == 0)
        return crb;
    int nod = 0;
    s = s + 4; // s=Nod_456 => s=456
    while (*s)
    {
        nod = nod * 10 + (*s - '0');
        s++;
    }
    return nod;
}

/// Determinam scorul arcelor grafului conform enuntului problemei
void determinare_scor_arc(int *adancime, graf *Graf)
{
    for (int i = 1; i <= Graf->numar_noduri; i++)
        for (nod *current = Graf->lista_adiacenta[i]; current; current = current->next)
        {
            current->scor = current->cost; // facem conversie implicita la float
            current->scor /= adancime[current->y];
        }
}

int *Dijkstra(int nodd, graf *Graf) // returneaza vectorul de predecesori specific arborelui
{
    int *predecesori = malloc((Graf->numar_noduri + 1) * sizeof(*predecesori));
    int *vizitat = calloc((Graf->numar_noduri + 1), sizeof(*vizitat));
    // Vectorul de distante este cel de scoruri aici
    float *scoruri = malloc((Graf->numar_noduri + 1) * sizeof(*scoruri));
    //
    for (int i = 1; i <= Graf->numar_noduri; i++)
    {
        vizitat[i] = 0;
        scoruri[i] = INF;
        predecesori[i] = -1;
    }
    // Analizam nodul nodd
    scoruri[nodd] = 0;
    vizitat[nodd] = 1;
    predecesori[nodd] = 0;
    for (nod *current = Graf->lista_adiacenta[nodd]; current; current = current->next)
    {
        scoruri[current->y] = current->scor;
        predecesori[current->y] = nodd;
    }
    // Facem pentru restul de N-1 noduri
    for (int i = 1; i < Graf->numar_noduri; i++)
    {
        int poz = -1;
        float minn = INF;
        for (int j = 1; j <= Graf->numar_noduri; j++)
            if (vizitat[j] == 0 && scoruri[j] < minn)
            {
                minn = scoruri[j];
                poz = j;
            }
        if (poz == -1)
            continue;
        vizitat[poz] = 1;
        for (nod *current = Graf->lista_adiacenta[poz]; current; current = current->next)
        {
            if (vizitat[current->y] == 0 && scoruri[current->y] > scoruri[poz] + current->scor)
            {
                scoruri[current->y] = scoruri[poz] + current->scor;
                predecesori[current->y] = poz;
            }
        }
    }
    free(scoruri);
    free(vizitat);
    return predecesori;
}

/// Functie pentru prima citire a fisierului care ne ajuta la a determina
/// nodul asignat insulei, respectiv corabiei
int string_to_char_prev(char *s)
{
    if (strcmp(s, "Insula") == 0)
        return -1;
    if (strcmp(s, "Corabie") == 0)
        return -1;
    int nod = 0;
    s = s + 4; // s=Nod_456 => s=456
    while (*s)
    {
        nod = nod * 10 + (*s - '0');
        s++;
    }
    return nod;
}

/// Ajuta ca functia anterioara la prima citire din fisier
void determinare_corabie_insula(int *insula, int *corabie)
{
    FILE *f = fopen("tema3.in", "r");
    int N, M;
    fscanf(f, "%d%d", &N, &M);
    int *Aparitii_Noduri = calloc((N + 1), sizeof(int));
    for (int i = 1; i <= M; i++)
    {
        int cost;
        char x_string[43], y_string[43];
        fscanf(f, "%s%s%d", x_string, y_string, &cost);
        int x = string_to_char_prev(x_string);
        if (x != -1)
            Aparitii_Noduri[x] = 1;
        int y = string_to_char_prev(y_string);
        if (y != -1)
            Aparitii_Noduri[y] = 1;
    }
    *insula = -1;
    for (int i = 1; i <= N; i++)
        if (!Aparitii_Noduri[i])
            if (*insula == -1)
                *insula = i;
            else
                *corabie = i;
    free(Aparitii_Noduri);
    fclose(f);
}

void task2()
{
    int insula, corabie;
    determinare_corabie_insula(&insula, &corabie);
    //
    FILE *f = fopen("tema3.in", "r");
    FILE *g = fopen("tema3.out", "w");
    //
    graf *Graf;
    Graf = malloc(sizeof(graf));
    Graf->numar_noduri = 0;
    Graf->lista_adiacenta = NULL;
    // Citire date
    int N, M, G_comoara;
    fscanf(f, "%d%d", &N, &M);
    //
    Graf->numar_noduri = N;
    Graf->lista_adiacenta = malloc((N + 1) * sizeof(nod *));
    for (int i = 0; i <= N; i++)
        Graf->lista_adiacenta[i] = NULL;
    // Continuare citire
    for (int i = 1; i <= M; i++)
    {
        int cost;
        char x_string[43], y_string[43];
        fscanf(f, "%s%s%d", x_string, y_string, &cost);
        int x = string_to_decimal(x_string, insula, corabie),
            y = string_to_decimal(y_string, insula, corabie);
        //
        insert_node(&Graf->lista_adiacenta[x], y, cost);
    }
    //
    int *Adancime_apa = malloc((N + 1) * sizeof(*Adancime_apa));
    // Continuare citire
    for (int i = 1; i <= N; i++)
    {
        char string_node[43];
        int cost;
        fscanf(f, "%s%d", string_node, &cost);
        Adancime_apa[string_to_decimal(string_node, insula, corabie)] = cost;
        // printf("%d %d\n",string_to_decimal(string_node,N),cost);
    }
    fscanf(f, "%d", &G_comoara);
    // Am terminat citirea
    // Facem rezolvarea propriu-zisa
    int *vizitati = calloc(N + 1, sizeof(*vizitati));
    DFS(corabie, vizitati, Graf);
    if (vizitati[insula])
    {
        for (int i = 0; i <= N; i++)
            vizitati[i] = 0;
        DFS(insula, vizitati, Graf);
        if (vizitati[corabie]) // ajungem de la insula la corabie
        {
            // Determinam scorul arcelor grafului
            determinare_scor_arc(Adancime_apa, Graf);
            //
            int *pred = Dijkstra(insula, Graf);
            //
            int *drum_inversat = malloc((Graf->numar_noduri + 1) * sizeof(int));
            int index = 0;
            //
            int tata = corabie;
            while (tata)
            {
                drum_inversat[++index] = tata;
                tata = pred[tata];
            }
            // Afisare raspunsuri
            fprintf(g, "Insula ");
            for (int i = index - 1; i > 1; i--)
                fprintf(g, "Nod_%d ", drum_inversat[i]);
            fprintf(g, "Corabie \n");
            int cost_total = 0;
            // Facem costul total dat evident de suma costurilor
            // muchiilor drum_inversat[k]->drum_inversat[k-1]
            for (int i = index; i > 1; i--)
            {
                float minn = INF;
                int cost_current;
                for (nod *current = Graf->lista_adiacenta[drum_inversat[i]]; current; current = current->next)
                    if (current->y == drum_inversat[i - 1] && current->scor < minn)
                    {
                        cost_current = current->cost;
                        minn = current->scor;
                    }
                cost_total += cost_current;
            }
            fprintf(g, "%d\n", cost_total);
            // Determinam adancimea minima a apei
            int adancime_minima_apa = Adancime_apa[drum_inversat[index - 1]];
            for (int i = index - 1; i > 1; i--)
                if (adancime_minima_apa > Adancime_apa[drum_inversat[i]])
                    adancime_minima_apa = Adancime_apa[drum_inversat[i]];
            fprintf(g, "%d\n", adancime_minima_apa);
            int nr_drumuri = (G_comoara + adancime_minima_apa - 1) / adancime_minima_apa;
            // int nr_drumuri = G_comoara / adancime_minima_apa
            // if (G_comoara % adancime_minima_apa)
            //     nr_drumuri++;
            fprintf(g, "%d\n", nr_drumuri);
            free(drum_inversat);
            free(pred);
        }
        else
            fprintf(g, "Echipajul nu poate transporta comoara inapoi la corabie\n");
    }
    else // daca nu s-a ajuns pe insula(=1)
        fprintf(g, "Echipajul nu poate ajunge la insula\n");
    // Eliberare memorie
    free(vizitati);
    free(Adancime_apa);
    eliberare_memorie_graf(&Graf);
    // Inchidere fisiere
    fclose(f);
    fclose(g);
}

int main(int argc, char *argv[])
{
    if (strcmp(argv[1], "1") == 0)
        task1();
    else
        task2();
    return 0;
}