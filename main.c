/*Ștefănică Matei-Costin - 311CB*/
#include <stdio.h>
#include <stdlib.h>
#include "functii.h"

int main() {
    int nrComenzi, i;
    char comanda[100];
    TJoc *joc = initJoc();
    if (!joc) printf("Eroare de alocare\n");
    TCoada *coada = initCoada();
    if (!coada) printf("Eroare de alocare");

    FILE *in = fopen("tema1.in", "r");
    FILE *out = fopen("tema1.out", "w");

    fscanf(in, "%d", &nrComenzi);

    // pentru a scapa de caracterul '\n' de pe linia cu numarul comenzilor
    // el e lasat acolo de fscanf, care nu il citeste
    // toate fgets-urile citesc maxim 50 de 
    fgets(comanda, 100, in);
    
    for (i = 0; i < nrComenzi; i++) {
        //citesc comanda si ii sterg caracterul '\n' de la final
        fgets(comanda, 100, in);
        comanda[strcspn(comanda, "\n")] = '\0';
        // pentru comenzile de tip query, care se executa imediat
        if (strcmp(comanda, "SHOW_CURRENT") == 0) {
            showCurrent(joc, out);
        }
        else if (strcmp(comanda, "SHOW") == 0) {
            show(joc, out);
        }
        else if (strcmp(comanda, "SWITCH") == 0) {
            switchCoada(coada);
        }
        else if (strcmp(comanda, "EXECUTE") == 0) {
            // scot cate o comanda din coada si o execut
            char comandaExtrasa[100];
            extragereCoada(coada, comandaExtrasa);
            if (strcmp(comandaExtrasa, "MOVE_LEFT") == 0) {
                moveLeft(joc);
            }
            else if (strcmp(comandaExtrasa, "MOVE_RIGHT") == 0) {
                    moveRight(joc);
            }
            else if (strncmp(comandaExtrasa, "WRITE", strlen("WRITE")) == 0) {
                write(joc, comandaExtrasa[strlen("WRITE") + 1]);
            }
            else if (strcmp(comandaExtrasa, "CLEAR_CELL") == 0) {
                clearCell(&joc);
            }
            else if (strcmp(comandaExtrasa, "CLEAR_ALL") == 0) {
                clearAll(&joc);
            }
            // urmatoarele conditii vor fi puse pe doua linii, pentru a nu
            // depasi 80 de caractere/linie
            else if (strncmp(comandaExtrasa, "INSERT_LEFT",
            strlen("INSERT_LEFT")) == 0) {
                insertLeft(joc, comandaExtrasa[strlen("INSERT_LEFT") + 1],
                out);
            }
            else if (strncmp(comandaExtrasa, "INSERT_RIGHT",
            strlen("INSERT_RIGHT")) == 0) {
                insertRight(joc, comandaExtrasa[strlen("INSERT_RIGHT") + 1]);
            }
            else if (strncmp(comandaExtrasa, "SEARCH_LEFT",
            strlen("SEARCH_LEFT")) == 0) {
                searchLeft(joc, strstr(comandaExtrasa, " ") + 1, out);
            }
            else if (strncmp(comandaExtrasa, "SEARCH_RIGHT",
            strlen("SEARCH_RIGHT")) == 0) {
                searchRight(joc, strstr(comandaExtrasa, " ") + 1, out);
            }
            else if (strncmp(comandaExtrasa, "SEARCH",
            strlen("SEARCH")) == 0) {
                search(joc, strstr(comandaExtrasa, " ") + 1, out);
            }
        }
        else {
            // pentru orice comanda care nu e execute sau de tip query
            inserareCoada(coada, comanda);
        }
    }

    fclose(in);
    fclose(out);

    freeCoada(&coada);
    freeJoc(&joc);

    return 0;   
}