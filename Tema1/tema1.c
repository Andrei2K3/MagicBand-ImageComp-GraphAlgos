/*CIUCAN Andrei-Alexandru - 313CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char info;
    struct node *next, *prev;
} node;

// Implementare lista
typedef struct DList
{
    node *santinel, *tail, *finger;
} DList;

void initL(DList **list)//Initializare lista
{
    *list = malloc(sizeof(**list));
    if (list == NULL)
    {
        printf("Alocarea pentru list n-a putut fi facuta!");
        exit(-1);
    }
    //
    node *nod_nou1, *nod_nou2;
    nod_nou1 = (node *)malloc(sizeof(*nod_nou1));
    if (nod_nou1 == NULL)
    {
        printf("Alocarea pentru nod_nou1 n-a putut fi facuta!");
        exit(-1);
    }
    nod_nou2 = (node *)malloc(sizeof(*nod_nou2));
    if (nod_nou2 == NULL)
    {
        printf("Alocarea pentru nod_nou2 n-a putut fi facuta!");
        exit(-1);
    }
    //Nu ma intereseaza informatia din nod_nou1(santinel)=> nu fac nicio atribuire
    nod_nou1->prev = NULL;
    nod_nou1->next = nod_nou2;

    nod_nou2->info = '#';
    nod_nou2->prev = nod_nou1;
    nod_nou2->next = NULL;
    //
    (*list)->santinel = nod_nou1;
    (*list)->tail = nod_nou2;
    (*list)->finger = (*list)->tail;
}

void free_list(DList **list)//eliberare (memorie) lista
{
    node *nod;
    while ((**list).santinel != NULL)
    {
        nod = (**list).santinel;
        (**list).santinel = nod->next;
        free(nod);
    }

    free(*list);
}

int is_it_possible_to_move_left(DList *list)//pentru operatia de UNDO
{
    return list->finger->prev == list->santinel;//1=mutare invalida, 0=mutare valida
}

void move_left(DList *list)
{
    if (list->finger->prev == list->santinel)
        return;
    list->finger = list->finger->prev;
}

void move_right(DList *list)
{
    if (list->finger == list->tail)
    {
        node *nod_nou;
        nod_nou = (node *)malloc(sizeof(*nod_nou));
        nod_nou->info = '#';
        nod_nou->next = NULL;
        nod_nou->prev = list->tail;
        list->tail->next = nod_nou;
        list->tail = nod_nou;
        list->finger = list->tail;
        return;
    }
    list->finger = list->finger->next;
}

void move_left_char(DList *list, char c, FILE **Fw)
{
    node *nod = list->finger;
    while (nod != list->santinel && nod->info != c)
    {
        nod = nod->prev;
    }
    if (nod == list->santinel)
    {
        fprintf(*Fw, "ERROR\n");
        return;
    }
    list->finger = nod;
}

void move_right_char(DList *list, char c)
{
    node *nod = list->finger;
    while (nod && nod->info != c)
    {
        nod = nod->next;
    }
    if (nod == NULL) // c n-a fost gasit
    {
        node *nod_nou;
        nod_nou = (node *)malloc(sizeof(*nod_nou));
        nod_nou->info = '#';
        nod_nou->next = NULL;
        nod_nou->prev = list->tail;
        list->tail->next = nod_nou;
        list->tail = nod_nou;
        list->finger = list->tail;
        return;
    }
    list->finger = nod;
}

void write_c(DList *list, char c)
{
    list->finger->info = c;
}

void insert_left(DList *list, char c, FILE **Fw)
{
    if (list->finger->prev == list->santinel)
    {
        fprintf(*Fw, "ERROR\n");
        return;
    }
    node *nod_nou = (node *)malloc(sizeof(*nod_nou));
    nod_nou->info = c;
    nod_nou->next = list->finger;
    nod_nou->prev = list->finger->prev;
    list->finger = list->finger->prev;
    list->finger->next->prev = nod_nou;
    list->finger->next = nod_nou;
    list->finger = nod_nou;
}

void insert_right(DList *list, char c)
{
    node *nod_nou = (node *)malloc(sizeof(*nod_nou));
    nod_nou->info = c;
    nod_nou->next = list->finger->next;
    nod_nou->prev = list->finger;
    if (list->finger == list->tail) // inserare la final
    {
        list->finger->next = nod_nou;
        list->finger = nod_nou;
        list->tail = nod_nou;
    }
    else
    {
        list->finger->next->prev = nod_nou;
        list->finger->next = nod_nou;
        list->finger = nod_nou;
    }
}

void show_current(DList *list, FILE **fw)
{
    fprintf(*fw, "%c\n", list->finger->info);
}

void show(DList *list, FILE **fw)
{
    node *current = list->santinel->next;
    while (current && current != list->finger)
    {
        fprintf(*fw, "%c", current->info);
        current = current->next;
    }
    fprintf(*fw, "|%c|", current->info);
    current = current->next;
    while (current)
    {
        fprintf(*fw, "%c", current->info);
        current = current->next;
    }
    fprintf(*fw, "\n");
}



// Implementare stiva
typedef struct nodS
{
    node *info;//informatia este de tip nod pentru a memora finger
    struct nodS *next;
} nodS;

void initS(nodS **stack)//initializare stiva vida
{
    *stack = NULL;
}

int isEmptyS(nodS *stack)
{
    return stack == NULL;
}

void free_stack(nodS **stack)//eliberare (memorie) stiva
{
    nodS *nod;
    while (*stack != NULL)
    {
        nod = *stack;
        *stack = nod->next;
        free(nod);
    }
    *stack = NULL;
    // free(*stack);
}

node *topS(nodS *stack)//extragere info din stiva
{
    if (!isEmptyS(stack))
        return stack->info;
    printf("Stiva este goala!\n");
    exit(-1);
}

void pushS(nodS **stack, node *data)//adaugare data in stiva
{
    nodS *nod_nou = (nodS *)malloc(sizeof(*nod_nou));
    nod_nou->info = data;
    nod_nou->next = *stack;
    *stack = nod_nou;
}

void popS(nodS **stack)//eliminare nodS din varful stive
{
    if (*stack != NULL)
    {
        nodS *nod_sters = *stack;
        *stack = (*stack)->next;
        free(nod_sters);
    }
    else
    {
        printf("Stiva vida!\n");
    }
}



// Implementare coada
typedef struct data
{
    char comanda[16], c;
} data;

typedef struct nodQ
{
    data info;
    struct nodQ *next;
} nodQ;

typedef struct Queue
{
    nodQ *head, *tail;
} Queue;

void initQ(Queue **Q)//Initializare coada
{
    *Q = malloc(sizeof(**Q));
    (*Q)->head = NULL;
    (*Q)->tail = NULL;
}

void free_queue(Queue **Q)//eliberare (memorie) coada
{
    nodQ *nod;
    while ((*Q)->head != NULL)
    {
        nod = (*Q)->head;
        (*Q)->head = nod->next;
        free(nod);
    }
    free(*Q);
}

int isEmptyQ(Queue *Q)
{
    return Q->tail == NULL;
}

data topQ(Queue *Q)//extragere data din coada
{
    if (!isEmptyQ(Q))
        return Q->head->info;
    data D;
    strcpy(D.comanda, "Coada e vida!");
    //D.c = 'X'; //un caracter random, l-am ales pe X
    printf("Coada este goala!");
    return D;
}

void pushQ(Queue *Q, data date)//adaugare data in coada
{
    nodQ *nod_nou = (nodQ *)malloc(sizeof(*nod_nou));
    nod_nou->info = date;
    nod_nou->next = NULL;
    if (Q->tail == NULL)
    {
        Q->head = nod_nou;
        Q->tail = nod_nou;
    }
    else
    {
        Q->tail->next = nod_nou;
        Q->tail = nod_nou;
    }
}

void popQ(Queue *Q)//eliminare data in coada
{
    if (!isEmptyQ(Q))
    {
        nodQ *nod_sters = Q->head;
        Q->head = Q->head->next;
        if (Q->head == NULL)
            Q->tail = NULL;
        free(nod_sters);
    }
    else
    {
        printf("Coada vida!\n");
    }
}

int main()
{
    //Initializare lista,coada,stive
    DList *L;
    initL(&L);
    Queue *Q;
    initQ(&Q);
    nodS *Undo, *Redo;
    initS(&Undo);
    initS(&Redo);
    //
    node *nod_undo_redo;
    data D;
    char s[16], c;
    int N, lg;
    //Deschidere fisiere
    FILE *Read = fopen("tema1.in", "r"),
         *Write = fopen("tema1.out", "w");
    //Citire date si rezolvare problema
    fscanf(Read, "%d", &N);
    while (N--)
    {
        fscanf(Read, "%s", D.comanda);
        lg = strlen(D.comanda);
        if (lg == 14 || lg == 15 || lg == 5 || lg == 11 || (lg == 12 && D.comanda[0] == 'I')) // cele 5 operatii ce implica <C>
        {
            fscanf(Read, "%s", s);
            D.c = s[0];
            pushQ(Q, D);
        }
        else
        {
            if (strcmp(D.comanda, "SHOW") == 0)
                show(L, &Write);
            else if (strcmp(D.comanda, "SHOW_CURRENT") == 0)
                show_current(L, &Write);
            else if (strcmp(D.comanda, "MOVE_LEFT") == 0 || strcmp(D.comanda, "MOVE_RIGHT") == 0)
                pushQ(Q, D);
            else if (strcmp(D.comanda, "UNDO") == 0)
            {
                nod_undo_redo = topS(Undo);
                popS(&Undo); //?
                pushS(&Redo, L->finger);
                L->finger = nod_undo_redo;
            }
            else if (strcmp(D.comanda, "REDO") == 0)
            {
                nod_undo_redo = topS(Redo);
                popS(&Redo); //?
                pushS(&Undo, L->finger);
                L->finger = nod_undo_redo;
            }
            else if (strcmp(D.comanda, "EXECUTE") == 0)
            {
                D = topQ(Q);
                popQ(Q);
                if (strcmp(D.comanda, "MOVE_LEFT") == 0)
                {
                    if (!is_it_possible_to_move_left(L))
                        pushS(&Undo, L->finger);
                    move_left(L);
                }
                else if (strcmp(D.comanda, "MOVE_RIGHT") == 0)
                {
                    pushS(&Undo, L->finger);
                    move_right(L);
                }
                else if (strcmp(D.comanda, "MOVE_LEFT_CHAR") == 0)
                {
                    move_left_char(L, D.c, &Write);
                }
                else if (strcmp(D.comanda, "MOVE_RIGHT_CHAR") == 0)
                {
                    move_right_char(L, D.c);
                }
                else if (strcmp(D.comanda, "WRITE") == 0)
                {
                    write_c(L, D.c);
                    free_stack(&Undo);
                    free_stack(&Redo);
                    initS(&Undo);
                    initS(&Redo);
                }
                else if (strcmp(D.comanda, "INSERT_LEFT") == 0)
                {
                    insert_left(L, D.c, &Write);
                }
                else if (strcmp(D.comanda, "INSERT_RIGHT") == 0)
                {
                    insert_right(L, D.c);
                }
            }
        }
    }
    //Eliberare memorie
    free_list(&L);
    free_stack(&Undo);
    free_stack(&Redo);
    free_queue(&Q);
    //Inchidere fisier
    fclose(Read);
    fclose(Write);
    return 0;
}