/*Ștefănică Matei-Costin - 311CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vagon {
    char info;
    struct vagon *pre;
    struct vagon *urm;
} TVagon, *TTren;

typedef struct {
    TTren mecanic;
    TTren tren;
}TJoc;

typedef struct comanda {
    char comanda[20];
    struct comanda *urm;
    struct comanda *pre;
} TComanda, *TListaComenzi;

typedef struct coada {
    TListaComenzi inc, sf;
} TCoada;

TTren alocVagon();
TTren initTren();
TJoc *initJoc();

TCoada *initCoada();
void inserareCoada(TCoada *c, char* info);
void afisareCoada(TCoada *c);
void extragereCoada(TCoada *c, char *comanda);
void freeCoada(TCoada **c);

void write(TJoc *joc, char info);
void show(TJoc *joc, FILE *out);
void insertRight(TJoc *joc, char info);
void insertLeft(TJoc *joc, char info, FILE *out);
void moveLeft(TJoc *joc);
void moveRight(TJoc *joc);
void clearCell(TJoc **joc);
void clearAll(TJoc **joc);
void showCurrent(TJoc *joc, FILE *out);
void switchCoada(TCoada *c);
void searchRight(TJoc *joc, char *sir, FILE *out);
void searchLeft(TJoc *joc, char *sir, FILE *out);
void search(TJoc *joc, char *sir, FILE *out);
void freeJoc(TJoc **joc); 