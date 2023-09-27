/*CIUCAN Andrei-Alexandru - 313CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ull unsigned long long

typedef struct pixel
{
    unsigned char red, green, blue;
} pixel;

typedef struct nod
{
    unsigned char tip;
    pixel culoare;
    struct nod *child[4];
} nod;

// Calculeaza (x-y)^2, dar tine cont de tipul de data - unsigned
ull prod(ull x, ull y)
{
    if (x > y)
        return (x - y) * (x - y);
    return (y - x) * (y - x);
}

void CreateQuadTree(nod **r, pixel **arr, ull factor, int n, int cul, int i1, int j1, int i2, int j2)
{
    ull red = 0, green = 0, blue = 0, N = n, mean = 0;
    int i, j;
    // Calculam red, green, blue conform formulelor din enunt
    for (i = i1; i <= i2; i++)
        for (j = j1; j <= j2; j++)
        {
            red += arr[i][j].red;
            green += arr[i][j].green;
            blue += arr[i][j].blue;
        }
    red = red / (N * N);
    green = green / (N * N);
    blue = blue / (N * N);
    // Calculam mean
    for (i = i1; i <= i2; i++)
        for (j = j1; j <= j2; j++)
        {
            mean += prod(red, arr[i][j].red);
            mean += prod(green, arr[i][j].green);
            mean += prod(blue, arr[i][j].blue);
        }
    mean = mean / (3 * N * N);
    // Alocare nod nou
    if (*r == NULL)
    {
        *r = malloc(sizeof(nod));
        for (i = 0; i < 4; i++)
            (*r)->child[i] = NULL;
    }
    (*r)->tip = 0;
    // Aceste date vor fi luate in considerare doar daca (*r)->tip=1
    (*r)->culoare.red = red;
    (*r)->culoare.green = green;
    (*r)->culoare.blue = blue;
    //
    if (mean <= factor)
    {
        (*r)->tip = 1;
        return;
    }
    // Divizam (sub)imaginea in 4 si mai "avem de muncit"...
    if (n > 1)
    {
        int mi = (i1 + i2) / 2;
        int mj = (j1 + j2) / 2;
        CreateQuadTree(&((*r)->child[0]), arr, factor, n / 2, 1, i1, j1, mi, mj);
        CreateQuadTree(&((*r)->child[1]), arr, factor, n / 2, 2, i1, mj + 1, mi, j2);
        CreateQuadTree(&((*r)->child[2]), arr, factor, n / 2, 3, mi + 1, mj + 1, i2, j2);
        CreateQuadTree(&((*r)->child[3]), arr, factor, n / 2, 4, mi + 1, j1, i2, mj);
    }
}

// Determinam inaltimea, numarul frunzelor, respectiv dimensiunea laturii
// celui mai mare bloc, adica informatile cerute in cadrul cerintei 1
void parcurgere(nod *root, int top, int *hmax, int *leaf, int N, int *dim_bloc)
{
    if (root)
    {
        if (root->tip == 1)
        {
            (*leaf)++;
            if (*dim_bloc < N)
                *dim_bloc = N;
        }
        if (top > *hmax)
            *hmax = top;
        parcurgere(root->child[0], top + 1, hmax, leaf, N / 2, dim_bloc);
        parcurgere(root->child[1], top + 1, hmax, leaf, N / 2, dim_bloc);
        parcurgere(root->child[2], top + 1, hmax, leaf, N / 2, dim_bloc);
        parcurgere(root->child[3], top + 1, hmax, leaf, N / 2, dim_bloc);
    }
}

// Parcurgere in latime a arborelui cuaternar si rezolvarea cerintei 2
void BFS(nod *root, FILE **g)
{
    // Alocam coada dinamic: deoarece nu stim cu exactitate numarul de noduri
    // ale arborelui cuaternar... vom face realocari daca este necesar
    const int CNMAX = 1000;
    int NMAX = CNMAX;
    nod **Q = malloc(NMAX * sizeof(*Q));
    //
    int i, p = 0, u = 0;
    Q[u++] = root;
    while (p < u)
    {
        nod *current = Q[p++];
        // Scriem in fisier informatia ceruta
        if (current->tip == 0)
            fwrite(&(current->tip), sizeof(unsigned char), 1, *g);
        else
        {
            fwrite(&(current->tip), sizeof(unsigned char), 1, *g);
            fwrite(&(current->culoare.red), sizeof(unsigned char), 1, *g);
            fwrite(&(current->culoare.green), sizeof(unsigned char), 1, *g);
            fwrite(&(current->culoare.blue), sizeof(unsigned char), 1, *g);
        }
        //
        for (i = 0; i < 4; i++)
            if (current->child[i] != NULL)
            {
                Q[u++] = current->child[i];
                if (u % NMAX == 0)
                {
                    NMAX += CNMAX;
                    Q = realloc(Q, NMAX * sizeof(*Q));
                }
            }
    }
    // Eliberez memoria pentru coada
    free(Q);
}

// Citesc tipul nodului, eventual si pixelul aferent nodului(daca tip=1)
void citire(unsigned char *tip, pixel *color, FILE ***f)
{
    fread(tip, sizeof(unsigned char), 1, **f);
    if (*tip == 1)
    {
        fread(&(color->red), sizeof(unsigned char), 1, **f);
        fread(&(color->green), sizeof(unsigned char), 1, **f);
        fread(&(color->blue), sizeof(unsigned char), 1, **f);
    }
}

// Reconstruiesc arborele cuaternar pe baza parcurgerii in latime
void ReconstructQuadTree(nod **root, FILE **f)
{
    int i, j;
    unsigned char tip;
    pixel color;
    // Creez radacina
    citire(&tip, &color, &f);
    *root = malloc(sizeof(nod));
    for (i = 0; i < 4; i++)
        (*root)->child[i] = NULL;
    (*root)->tip = tip;
    (*root)->culoare = color;
    // Alocam coada dinamic: deoarece nu stim cu exactitate numarul de noduri
    // ale viitorului arbore cuaternar... vom face realocari daca este necesar
    const int CNMAX = 1000;
    int NMAX = CNMAX;
    nod **Q = malloc(NMAX * sizeof(*Q));
    //
    int p = 0, u = 0;
    Q[u++] = *root;
    while (p < u)
    {
        if (Q[p]->tip == 1) // daca in coada nodul curent e o frunza => nu are descendenti
        {
            p++;
            continue;
        }
        nod *current = Q[p++];
        for (i = 0; i < 4; i++)
        {
            // Adaugare nod nou
            citire(&tip, &color, &f);
            current->child[i] = malloc(sizeof(nod));
            for (j = 0; j < 4; j++)
                current->child[i]->child[j] = NULL;
            current->child[i]->tip = tip;
            current->child[i]->culoare = color;
            // Adaugare in coada
            Q[u++] = current->child[i];
            if (u % NMAX == 0)
            {
                NMAX += CNMAX;
                Q = realloc(Q, NMAX * sizeof(*Q));
            }
        }
    }
    // Eliberez memoria pentru coada
    free(Q);
}

// Reconstruiesc imaginea pe baza parcurgerii arborelui cuaternar
void ReconstructImage(pixel **arr, nod *r, int N, int i1, int j1, int i2, int j2)
{
    if (r->tip == 1) // daca nodul e frunza => zona aferenta in imagine e plina de r->culoare
    {
        int i, j;
        for (i = i1; i <= i2; i++)
            for (j = j1; j <= j2; j++)
                arr[i][j] = r->culoare;
        return;
    }
    //
    int mi = (i1 + i2) / 2;
    int mj = (j1 + j2) / 2;
    ReconstructImage(arr, r->child[0], N / 2, i1, j1, mi, mj);
    ReconstructImage(arr, r->child[1], N / 2, i1, mj + 1, mi, j2);
    ReconstructImage(arr, r->child[2], N / 2, mi + 1, mj + 1, i2, j2);
    ReconstructImage(arr, r->child[3], N / 2, mi + 1, j1, i2, mj);
}

// Eliberare memorie arbore cuaternar
void FreeQuadTree(nod **root)
{
    if (*root)
    {
        FreeQuadTree(&((*root)->child[0]));
        FreeQuadTree(&((*root)->child[1]));
        FreeQuadTree(&((*root)->child[2]));
        FreeQuadTree(&((*root)->child[3]));
        free(*root);
    }
}

// Eliberare memorie pentru matricea asociata imaginii
void FreeImage(pixel ***arr, int N)
{
    int i;
    for(i=0;i<N;i++)
        free((*arr)[i]);
    free(*arr);
}

int main(int argc, char *argv[])
{
    int i, j;
    nod *root = NULL;
    pixel **arr;
    FILE *in, *out;
    unsigned int M, N;
    //
    if (strcmp(argv[1], "-c1") == 0 || strcmp(argv[1], "-c2") == 0)
    {
        // Determin factor
        ull factor = 0;
        for (i = 0; argv[2][i]; i++)
            factor = factor * 10 + argv[2][i] - '0';
        // Citesc datele de intrare
        in = fopen(argv[3], "rb");
        char s[100], val[100];
        fgets(s, 100, in);
        fscanf(in, "%u%u\n", &M, &N);
        fgets(val, 100, in);
        arr = malloc(N * sizeof(pixel *));
        for (i = 0; i < N; i++)
            arr[i] = malloc(N * sizeof(pixel));
        for (i = 0; i < N; i++)
            fread(arr[i], sizeof(pixel), N, in);
        // Creez arborele cuaternar
        CreateQuadTree(&root, arr, factor, N, 0, 0, 0, N - 1, N - 1);
        //
        if (strcmp(argv[1], "-c1") == 0) // Cerinta 1
        {
            out = fopen(argv[4], "w");
            int h = 0, frunze = 0, dim_bloc = 0;
            parcurgere(root, 0, &h, &frunze, N, &dim_bloc);
            fprintf(out, "%d\n%d\n%d\n", h + 1, frunze, dim_bloc);
        }
        else // if (strcmp(argv[1], "-c2") == 0) // Cerinta 2
        {
            out = fopen(argv[4], "wb");
            fwrite(&N, sizeof(unsigned int), 1, out);
            BFS(root, &out);
        }
    }
    else // Cerinta 3
    {
        in = fopen(argv[2], "rb");
        out = fopen(argv[3], "wb");
        // Citesc datele si construiesc arborele cuaternar
        fread(&N, sizeof(unsigned int), 1, in);
        ReconstructQuadTree(&root, &in);
        // Aloc memorie pentru imagine
        arr = malloc(N * sizeof(pixel *));
        for (i = 0; i < N; i++)
            arr[i] = malloc(N * sizeof(pixel));
        // Reconstruiesc imaginea
        ReconstructImage(arr, root, N, 0, 0, N - 1, N - 1);
        // Scriu informatia in fisierul de iesire
        fprintf(out, "P6\n");
        fprintf(out, "%u %u\n", N, N);
        fprintf(out, "255\n");
        for (i = 0; i < N; i++)
            fwrite(arr[i], sizeof(pixel), N, out);
    }
    // Eliberez memoria pentru imagine si pentru arborele cuaternar
    FreeImage(&arr,N);
    FreeQuadTree(&root);
    // Inchid fisierele
    fclose(in);
    fclose(out);
    return 0;
}