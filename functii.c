/*Ștefănică Matei-Costin - 311CB*/
#include "functii.h"

// functii de baza pentru lista 

TTren alocVagon() {
    // alocare vagon pentru structura de tren
    TTren vagon = (TTren)malloc(sizeof(TVagon));
    if (!vagon) return NULL;
    // vagoanele noi se initializeaza cu caracterul '#'
    vagon->info = '#';
    // initializez pointerii de legaturi cu NULL
    vagon->urm = NULL;
    vagon->pre = NULL;
    return vagon;
}

TTren initTren() {
    // alocarea structurii de tren cu santinela
    TTren t = (TTren)malloc(sizeof(TVagon));
    if (!t) return NULL;
    t->urm = t;
    t->pre = t;
    // trenul are acum doar santinela, care are ambii pointeri catre sine
    return t;
}

TJoc *initJoc() {
    // alocare a structurii joc
    // la final exista lista cu santinela, un vagon, si mecanicul care se afla
    // in acel vagon
    TJoc *joc = (TJoc*)malloc(sizeof(TJoc));
    if (!joc) return NULL;
    joc->tren = initTren();
    if (!joc->tren) return NULL;
    // pana in punctul asta, am alocat doar trenul cu locomotiva
    TTren aux = alocVagon('#');
    if (!aux) return NULL;
    // leg primul vagon de locomotiva
    joc->tren->urm = aux;
    aux->urm = joc->tren;
    joc->tren->pre = aux;
    aux->pre = joc->tren;
    // pun mecanicul in primul vagon
    joc->mecanic = aux;
    return joc;
}

void freeJoc(TJoc **joc) {
    // folosesc functia clearAll pentru a sterge toate vagoanele,
    // in afara de unul, pe care il eliberez manual, la fel ca pe
    // locomotiva si pe joc in sine
    clearAll(joc);
    free((*joc)->tren->urm);
    free((*joc)->tren);
    free((*joc));
    (*joc) = NULL;
}

// functii de baza pentru coada

TCoada * initCoada() {
    TCoada *c = (TCoada*)malloc(sizeof(TCoada));
    if (!c) return NULL;
    c->inc = NULL;
    c->sf = NULL;
    return c;
}

void inserareCoada(TCoada *c, char* comanda) {
    // alocare element nou de lista
    TListaComenzi aux = (TListaComenzi)malloc(sizeof(TComanda));
    if (!aux) printf("Eroare de alocare\n");
    strcpy(aux->comanda, comanda);
    if (c->inc == NULL && c->sf == NULL) {
        // cazul in care coada este vida
        c->inc = aux;
        c->sf = aux;
        aux->urm = NULL;
        aux->pre = NULL;
        return;
    }
    aux->pre = c->sf;
    c->sf->urm = aux;
    aux->urm = NULL;
    c->sf = aux;
}

void extragereCoada(TCoada *c, char *comanda) {
    // comanda extrasa din stiva va fi copiata in variabila transmisa prin
    // adresa, pentru a putea fi executata in main
    TListaComenzi aux = c->inc;
    if (c->inc == c->sf) {
        // cazul in care coada are un singur element
        c->inc = NULL;
        c->sf = NULL;
    } else {
        c->inc = c->inc->urm;
        c->inc->pre = NULL;
        c->sf->urm = NULL;
    }
    strcpy(comanda, aux->comanda);
    free(aux);
}

void afisareCoada(TCoada *c) { // functie folosita pentru debugging
    if (c->inc == NULL) {
        printf("Coada vida\n");
        return;
    }
    TListaComenzi p;
    p = c->inc;
    do {
        printf("%s\n", p->comanda);
        p = p->urm;
    } while (p != NULL);
}

void  a(TCoada **c) {
    TListaComenzi p = (*c)->inc, aux;
    while (p != (*c)->sf) {
        // parcurg lista de la primul pana la penultimul element si
        // eliberez elementele din lista pe parcurs
        aux = p;
        p = p->urm;
        free(aux->comanda);
        free(aux);
    }
    // eliberez si ultimul element si coada in sine
    free((*c)->sf);
    free((*c));
}

// functiile de implementat

void write(TJoc *joc, char info) {
    joc->mecanic->info = info;
}

void show(TJoc *joc, FILE *out) {
    // pointer auxiliar folosit pentru parcurgerea trenului
    TTren p = joc->tren->urm;
    while (p != joc->tren) {
        // parcurg incepand cu santinela pana ajung inapoi la santinela 
        // (lista e circulara)
        if (p == joc->mecanic) {
            // pentru a evidentia mecanicul
            fprintf(out, "|%c|", p->info);
        }
        else {
            fprintf(out, "%c", p->info);
        }
        p=p->urm;
    }
    fprintf(out, "\n");
}

void showCurrent(TJoc *joc, FILE *out) {
    fprintf(out, "%c\n", joc->mecanic->info);
}

void insertRight(TJoc *joc, char info) {
    TTren new = alocVagon();
    if (!new) printf("Eroare de alocare\n");
    new->info = info;
    // variabile folosite pentru a retine unde trebuie inserat vagonul
    // le-am folosit pentru a face codul mai clar
    TTren aux1 = joc->mecanic; // stanga vagonului nou
    TTren aux2 = joc->mecanic->urm; // dreapta vagonului nou
    // fac legaturile
    new->urm = aux2;
    new->pre = aux1;
    aux1->urm = new;
    aux2->pre = new;
    // mut mecanicul
    joc->mecanic = new;
}

void insertLeft(TJoc *joc, char info, FILE *out) {
    if (joc->mecanic->pre == joc->tren){
        // daca mecanicul se afla pe prima pozitie
        fprintf(out, "ERROR\n");
        return;
    }
    else {
        TTren new = alocVagon();
        if (!new) printf("Eroare de alocare\n");
        new->info = info;
        // variabile folosite pentru a retine unde trebuie inserat vagonul
        // le-am folosit pentru a face codul mai clar
        TTren aux1 = joc->mecanic->pre; // stanga vagonului nou
        TTren aux2 = joc->mecanic; // dreapta vagonului nou
        // fac legaturile
        new->urm = aux2;
        new->pre = aux1;
        aux1->urm = new;
        aux2->pre = new;
        // mut mecanicul
        joc->mecanic = new;
    }
}

void moveLeft(TJoc *joc) {
    if (joc->mecanic->pre == joc->tren) {
        // daca vagonul in care se afla mecanicul este ultimul
        // in acest caz mut mecanicul in primul vagon
        joc->mecanic = joc->tren->pre;
    }
    else {
        joc->mecanic = joc->mecanic->pre;
    }
}

void moveRight(TJoc *joc) {
    if (joc->mecanic->urm == joc->tren) {
        // in cazul in care mecanicul se afla in ultimul vagon, inserez
        // un nou vagon la final, cu caracterul '#'
        insertRight(joc, '#');
        return;
    }
    joc->mecanic = joc->mecanic->urm;
}

void clearCell(TJoc **joc) {
    TTren aux = (*joc)->mecanic;
    // folosesc variabila aux pentru a retine celula care trebuie stearsa
    if ((*joc)->tren->urm->urm == (*joc)->tren) {
        // cazul in care trenul are un singur vagon
        // eliberez respectivul vagon si reinitializez trenul
        free(aux);
        free((*joc));
        (*joc) = initJoc();
        return;
    }
    // in cazul in care sunt mai multe vagoane, mut mecanicul la stanga,
    // refac legaturile si eliberez vagonul, pe care l-am retinut in aux
    moveLeft(*joc);
    aux->pre->urm = aux->urm;
    aux->urm->pre = aux->pre;
    free(aux);
}

void clearAll(TJoc **joc) {
    // folosesc p pentru a parcurge vagoanele si aux pentru a retine la 
    // fiecare pas vagonul care trebuie eliberat
    TTren p = (*joc)->tren->pre;
    // pornesc cu p de la ultimul vagon
    TTren aux;
    while (p != (*joc)->tren) {
        // parcurg cu p in spate pana ajung la locomotiva si eliberez
        aux = p;
        p = p->pre;
        free(aux);
    }
    // eliberez si locomotiva, apoi jocul in sine si reinitializez jocul
    free((*joc)->tren);
    free((*joc));
    (*joc) = initJoc();
}

void switchCoada(TCoada *c) {
    char aux[100];
    int nrelem = 0, i;
    TListaComenzi p = c->inc;
    // numar cate comenzi sunt in coada
    while (p != NULL) {
        nrelem++;
        p = p->urm;
    }
    // folosesc 2 pointeri la inceputul si la finalul cozii ca apoi sa parcurg
    // coada spre jumatate, atat de la inceput, cat si de la final, pentru a
    // interschimba intre ele comenzile, rezultand coada cu comenzi in 
    // ordine inversa
    TListaComenzi q = c->sf;
    p = c->inc;
    for (i = 0; i < nrelem / 2; i++) {
        strcpy(aux, p->comanda);
        strcpy(p->comanda, q->comanda);
        strcpy(q->comanda, aux);
        p = p->urm;
        q = q->pre;
    }
}

void searchRight(TJoc *joc, char *sir, FILE *out) {
    int len = 0;
    // lungimea listei din dreapta mecanicului, pentru ca doar acolo caut
    int i;
    TTren p = joc->mecanic;
    // sir folosit pentru a retine caracterele in care vom cauta
    char *sirLista; 
    // numar cate elemente va avea sirul de caractere
    while (p != joc->tren) {
        len++;
        p = p->urm;
    }
    sirLista = (char *)calloc(len + 1, sizeof(char));
    if (!sirLista) {
        printf("Eroare de alocare\n");
        free(sirLista);
        return;
    } 
    // parcurgerea listei si scrierea caracterelor in sir
    p = joc->mecanic;
    // scriu in sir caracterele din tren
    for(i = 0; i < len; i++) {
        sirLista[i] = p->info;
        sirLista[i+1] = '\0';
        p = p->urm;
    }
    char *rez = strstr(sirLista, sir);
    // obtin in rez un pointer la locul din sir din care incepe sirul cautat
    if (!rez) { // nu a gasit sirul
        fprintf(out, "ERROR\n");
    }
    else {
        // cu cate pozitii trebuie sa mut mcanicul la dreapta pana ajunge
        // la ultima pozitie din sir
        int nrPozDeplasare = strlen(sirLista) - strlen(rez);
        for (i = 0; i < nrPozDeplasare; i++) {
            moveRight(joc);
        }
        for (i = 1; i < strlen(sir); i++) {
            moveRight(joc);
        }
    }
    free(sirLista);
}

void searchLeft(TJoc *joc, char *sir, FILE *out) {
    // acelasi rationament ca la searchRight
    // lungimea listei din stanga mecanicului, pentru ca doar acolo cautam
    int len = 0; 
    int i;
    TTren p = joc->mecanic;
    // sir folosit pentru a retine caracterele in care vom cauta
    char *sirLista; 
    while (p != joc->tren) {
        len++;
        p = p->pre;
    }
    sirLista = (char *)calloc(len + 1, sizeof(char));    
    if (!sirLista) {
        printf("Eroare de alocare\n");
        free(sirLista);
        return;
    } 
    p = joc->mecanic;
    for(i = 0; i < len; i++) {
        sirLista[i] = p->info;
        sirLista[i+1] = '\0';
        p = p->pre;
    }
    char *rez = strstr(sirLista, sir);
    if (!rez) { // nu a gasit sirul
        fprintf(out, "ERROR\n");
    }
    else {
        int nrPozDeplasare = strlen(sirLista) - strlen(rez);
        for (i = 0; i < nrPozDeplasare; i++) {
            moveLeft(joc);
        }
        for (i = 1; i < strlen(sir); i++) {
            moveLeft(joc);
        }
    }
    free(sirLista);
}

void search(TJoc *joc, char *sir, FILE *out) {
    int lenDr = 0; // lungimea listei din dreapta mecanicului
    int lenSt = 0; // lungimea listei din stanga mecanicului
    // mecanicul e luat in considerare doar "in dreapta" la numaratoare
    int i;
    TTren p = joc->mecanic;
    // sir folosit pentru a retine caracterele in care vom cauta
    char *sirLista; 
    while (p != joc->tren) {
        lenDr++;
        p = p->urm;
    }
    while (p->urm != joc->mecanic) {
        lenSt++;
        p = p->urm;
    }
    // sirul in care retin elementele din lista dupa parcurgerea circulara
    sirLista = (char *)calloc(lenDr + lenSt + 1, sizeof(char));
    if (!sirLista) {
        printf("Eroare de alocare\n");
        free(sirLista);
        return;
    } 
    p = joc->mecanic;
    // construirea sirului ce contine elementele listei
    // similar cu searchLeft si searchRight
    for(i = 0; i < lenDr; i++) {
        sirLista[i] = p->info;
        sirLista[i+1] = '\0';
        p = p->urm;
    }
    p = joc->tren->urm;
    for(; i < lenSt + lenDr; i++) {
        sirLista[i] = p->info;
        sirLista[i+1] = '\0';
        p = p->urm;
    }
    // verificare daca exista sirul in lista
    char *rez = strstr(sirLista, sir);
    if (!rez) {
        fprintf(out, "ERROR\n");
    }
    else {
        // mutarea mecanicului
        int nrPozDeplasare = strlen(sirLista) - strlen(rez);
        if (nrPozDeplasare < lenDr) {
            // cazul in care pozitia pe care trebuie sa ajunga mecanicul
            // se afla in dreapta pozitiei sale actuale (sau pe pozitia
            // sa acutala)
            for (i = 0; i < nrPozDeplasare; i++) {
                moveRight(joc);
            }
        }
        else {
            // cazul in care mecanicul trebuie mutat undeva in stanga sa
            // in acest caz calculez pe a cata pozitie de la inceput trebuie
            // sa ajunga, il pun in primul vagon si il mut la dreapta
            nrPozDeplasare = nrPozDeplasare - lenDr;
            joc->mecanic = joc->tren->urm;
            for (i = 0; i < nrPozDeplasare; i++) {
                moveRight(joc);
            }
        }
    }
    free(sirLista);
}
