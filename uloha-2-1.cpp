// uloha-2-1.cpp -- Uloha 2 - Uroven 1 (200x200)
// Stanislav Jakubek, 4.5.2016 17:05:13

#include <stdio.h>
#include <algorithm>

/*
static bool DEBUG_INPUT = false;
static bool DEBUG_CYCLE = false;
static bool DEBUG_OUTPUT = false;
static bool DEBUG_ADD_FRONT = false;
*/

typedef struct cesta {
  int a,b;	//aktualna poloha
  int dlzka;	//aktualna dlzka
  struct cesta *pred;	//predchadzajuci clen
} CESTA;

typedef struct front {
  int dlzka;	//aktualna dlzka
  struct front *dalsi;	//nasledujuci clen frontu
  CESTA *cesta;	//aktualna cesta
} FRONT;

typedef struct vrchol {
  bool h;
  bool d;
  bool l;
  bool p;
} VRCHOL;

void addFront(FRONT **front, int **pomMapa, int K, int a, int b) {
  //FRONT *akt = *front, *novy = NULL;
  int dlzkaCesty;
  int predA = (*front)->cesta->a;
  int predB = (*front)->cesta->b;
  
  /* Zistenie dlzky cesty */
  if ((a - predA) == 1) { dlzkaCesty = pomMapa[2*predA+1][b]; }		//posun dole
  else if ((a - predA) == -1) { dlzkaCesty = pomMapa[2*a+1][b]; }	//posun hore
  else if ((b - predB) == 1) { dlzkaCesty = pomMapa[2*a][predB]; }	//posun doprava
  else if ((b - predB) == -1) { dlzkaCesty = pomMapa[2*a][b]; }		//posun dolava
  
  //printf("Dlzka cesty z (%d,%d) do (%d,%d): %d\n", predA, predB, a, b, dlzkaCesty);
  
  //if ((*front)->dlzka + dlzkaCesty > 2000) { return; }
  
  FRONT *akt = *front, *novy = NULL;
  
  /* Vytvorenie noveho clena frontu */
  novy = NULL;
  novy = (FRONT *) malloc (sizeof(FRONT));
  novy->cesta = (CESTA *) malloc (sizeof(CESTA));
  novy->cesta->a = a;
  novy->cesta->b = b;
  novy->cesta->pred = (*front)->cesta;
  novy->dlzka = (*front)->dlzka + dlzkaCesty;
  akt = (*front);
  
  while (akt->dalsi != NULL) {
    if (novy->dlzka < akt->dalsi->dlzka) { break; }
    akt = akt->dalsi;
  }
  
  novy->dalsi = akt->dalsi;
  akt->dalsi = novy;
}

using namespace std;

int main() {
  int a, b, i, j, k, l, m, n, N, K, cesta;
  
  /* Nacitanie poctu podvstupov */
  scanf("%d ", &N);
  
  for (i = 0; i < N; i++) {
    scanf("%d ", &K);
    //if (DEBUG_INPUT) printf("\nK: %d 2K-1: %d\n", K, (2*K-1));
    int **values;
    
    values = new int *[2*K-1];
    for (k = 0; k < (2*K-1); k++) { values[k] = new int[K]; }
    
    /* Nacitanie hodnot do pola */
    for (j = 0; j < (2*K-1); j++) {
      if ((j%2) == 0) {
        for (l = 0; l < (K-1); l++) {
          scanf("%d ", &values[j][l]);
          //if (DEBUG_INPUT) printf("%d ", values[j][l]);
        }
        //if (DEBUG_INPUT) printf("\n");
      }
      else {
        for (l = 0; l < K; l++) {
          scanf("%d ", &values[j][l]);
          //if (DEBUG_INPUT) printf("%d ", values[j][l]);
        }
        //if (DEBUG_INPUT) printf("\n");
      }
    }
    
    /* Algoritmus */
    FRONT *front = NULL, *temp = NULL;
    /* Pomocna mapa na zapamatanie uz navstivenych miest */
    VRCHOL *pomMapa[K][K];
    
    for (m = 0; m < K; m++) {
      for (n = 0; n < K; n++) {
        pomMapa[m][n] = (VRCHOL *) malloc (sizeof(VRCHOL));
        pomMapa[m][n]->h = false;
        pomMapa[m][n]->d = false;
        pomMapa[m][n]->l = false;
        pomMapa[m][n]->p = false;
      }
    }
    
    front = NULL;
    front = (FRONT *) malloc (sizeof(FRONT));
    front->cesta = (CESTA *) malloc (sizeof(CESTA));
    front->cesta->a = 0;
    front->cesta->b = 0;
    front->cesta->pred = NULL;
    front->dlzka = 0;
    front->dalsi = NULL;
    
    /* Prechadzanie frontu */
    while (front != NULL) {
      //if (DEBUG_CYCLE) printf("Zaciatok cyklu\n");
      a = front->cesta->a;
      b = front->cesta->b;
      
      //printf("P: (%d,%d) D: %d\n", a, b, front->dlzka);
      
      /* Ak uz nasiel koncovy bod */
      if ((a == K-1) && (b == K-1)) {
        //if (DEBUG_CYCLE) printf("Nasiel sa koncovy bod\n");
        cesta = front->dlzka;
        break;
      }
      
      /* Pridavanie do frontu */
      //if (DEBUG_CYCLE) printf("Pridavanie do frontu\n");
      if (a == 0) {
        //if (DEBUG_CYCLE) printf("a == 0\n");
        if (!pomMapa[a][b]->d) { addFront(&front, values, K, a+1, b); pomMapa[a][b]->d = true; }
        
        if (b == 0) {
          //if (DEBUG_CYCLE) printf("Lavy horny roh\n");
          if (!pomMapa[a][b]->p) { addFront(&front, values, K, a, b+1); pomMapa[a][b]->p = true; }
        }
        else if (b == K-1) {
          //if (DEBUG_CYCLE) printf("Pravy horny roh\n");
          if (!pomMapa[a][b]->l) { addFront(&front, values, K, a, b-1); pomMapa[a][b]->l = true; }
        }
        else {
          //if (DEBUG_CYCLE) printf("Horna hrana\n");
          if (!pomMapa[a][b]->p) { addFront(&front, values, K, a, b+1); pomMapa[a][b]->p = true; }
          if (!pomMapa[a][b]->l) { addFront(&front, values, K, a, b-1); pomMapa[a][b]->l = true; }
        }
      }
      else if (a == K-1) {
        //if (DEBUG_CYCLE) printf("a == K-1\n");
        if (!pomMapa[a][b]->h) { addFront(&front, values, K, a-1, b); pomMapa[a][b]->h = true; }
        
        if (b == 0) {
          //if (DEBUG_CYCLE) printf("Lavy dolny roh\n");
          if (!pomMapa[a][b]->p) { addFront(&front, values, K, a, b+1); pomMapa[a][b]->p = true; }
        }
        else {
          //if (DEBUG_CYCLE) printf("Dolna hrana\n");
          if (!pomMapa[a][b]->p) { addFront(&front, values, K, a, b+1); pomMapa[a][b]->p = true; }
          if (!pomMapa[a][b]->l) { addFront(&front, values, K, a, b-1); pomMapa[a][b]->l = true; }
        }
      }
      else if (b == 0) {
        //if (DEBUG_CYCLE) printf("Lava hrana\n");
        if (!pomMapa[a][b]->p) { addFront(&front, values, K, a, b+1); pomMapa[a][b]->p = true; }
        if (!pomMapa[a][b]->d) { addFront(&front, values, K, a+1, b); pomMapa[a][b]->d = true; }
        if (!pomMapa[a][b]->h) { addFront(&front, values, K, a-1, b); pomMapa[a][b]->h = true; }
      }
      else if (b == K-1) {
        //if (DEBUG_CYCLE) printf("Prava hrana\n");
        if (!pomMapa[a][b]->l) { addFront(&front, values, K, a, b-1); pomMapa[a][b]->l = true; }
        if (!pomMapa[a][b]->d) { addFront(&front, values, K, a+1, b); pomMapa[a][b]->d = true; }
        if (!pomMapa[a][b]->h) { addFront(&front, values, K, a-1, b); pomMapa[a][b]->h = true; }
      }
      else {
        //if (DEBUG_CYCLE) printf("Vnutorny bod\n");
        int predA = front->cesta->pred->a;
        int predB = front->cesta->pred->b;
        
        if ((a - predA) == 1) {
          //if (DEBUG_CYCLE) printf("Prisiel zhora\n");
          //prisiel zhora - nemoze ist do (a,b+1) a (a-1,b)
          if (!pomMapa[a][b]->l) { addFront(&front, values, K, a, b-1); pomMapa[a][b]->l = true; }
          if (!pomMapa[a][b]->d) { addFront(&front, values, K, a+1, b); pomMapa[a][b]->d = true; }
        }
        else if ((a - predA) == -1) {
          //if (DEBUG_CYCLE) printf("Prisiel zdola\n");
          //prisiel zdola - nemoze ist do (a,b-1) a (a+1,b)
          if (!pomMapa[a][b]->p) { addFront(&front, values, K, a, b+1); pomMapa[a][b]->p = true; }
          if (!pomMapa[a][b]->h) { addFront(&front, values, K, a-1, b); pomMapa[a][b]->h = true; }
        }
        else if ((b - predB) == 1) {
          //if (DEBUG_CYCLE) printf("Prisiel zlava\n");
          //prisiel zlava - nemoze ist do (a-1,b) a (a,b-1)
          if (!pomMapa[a][b]->d) { addFront(&front, values, K, a+1, b); pomMapa[a][b]->d = true; }
          if (!pomMapa[a][b]->p) { addFront(&front, values, K, a, b+1); pomMapa[a][b]->p = true; }
        }
        else if ((b - predB) == -1) {
          //if (DEBUG_CYCLE) printf("Prisiel zprava\n");
          //prisiel zprava - nemoze ist do (a+1,b) a (a,b+1)
          if (!pomMapa[a][b]->h) { addFront(&front, values, K, a-1, b); pomMapa[a][b]->h = true; }
          if (!pomMapa[a][b]->l) { addFront(&front, values, K, a, b-1); pomMapa[a][b]->l = true; }
        }
      }
      
      /* Odstranenie prveho prvku frontu a posun na dalsi */
      temp = front;
      if (front != NULL) {
        front = front->dalsi;
        free(temp);
      }
      //if (DEBUG_CYCLE) printf("Koniec terajsieho cyklu\n");
    }
    
    //printf(cesta ? "true" : "false");
    
    /* Vypis */
    //if (DEBUG_OUTPUT) printf("Koncovy vypis\n");
    printf("%d\n", cesta);
  }
  
  return 0;
}
