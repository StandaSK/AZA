// uloha-1.cpp -- Uloha 1
// Stanislav Jakubek, 5.4.2016 14:12:33

#include <stdio.h>
#include <algorithm>
using namespace std;

typedef struct vrchol {
  int pocet_susedov;
  bool prejdeny;
  int *susedia;
}VRCHOL;

typedef struct graf {
  int pocet_vrcholov;
  VRCHOL **vrcholy;
}GRAF;

typedef struct front {
  int dlzka;
  int rodic;
  int vrchol;
  struct front *dalsi;
}FRONT;

GRAF *graf;
FRONT *zaciatok, *koniec;

int addFront(int zdroj) {
  int i;
  FRONT *novy = NULL;
  
  //uvodna inicializacia frontu
  zaciatok = (FRONT *) malloc (sizeof(FRONT));
  zaciatok->dlzka = 0;
  zaciatok->rodic = -1;
  zaciatok->vrchol = zdroj;
  koniec = zaciatok;
  
  //uvodne pridanie susedov do frontu
  for (i = 0; i < graf->vrcholy[zdroj]->pocet_susedov; i++) {
    if ((graf->vrcholy[zdroj]->susedia[i] - 1) != zaciatok->rodic) {
      novy = (FRONT *) malloc (sizeof(FRONT));
      novy->vrchol = graf->vrcholy[zdroj]->susedia[i] - 1;
      novy->dlzka = 1;
      novy->rodic = zdroj;
      koniec->dalsi = novy;
      koniec = koniec->dalsi;
      novy = NULL;
    }
  }
  
  zaciatok = zaciatok->dalsi;
  
  while ((zaciatok) && (zaciatok->vrchol != zdroj)) {
    //printf("Zac->Vrch: %d\n", zaciatok->vrchol + 1);
    
    for (i = 0; i < graf->vrcholy[zaciatok->vrchol]->pocet_susedov; i++) {
      //printf("Sused: %d\n", graf->vrcholy[zaciatok->vrchol]->susedia[i]);
      
      if ((graf->vrcholy[zaciatok->vrchol]->susedia[i] - 1) != zaciatok->rodic) {
        
        //ak je jeden zo susedov zdroj, tak hned skonci hladanie
        if ((graf->vrcholy[zaciatok->vrchol]->susedia[i] - 1) == zdroj) {
          graf->vrcholy[zdroj]->prejdeny = true;
          return zaciatok->dlzka + 1;
        }
        
        novy = (FRONT *) malloc (sizeof(FRONT));
        novy->vrchol = graf->vrcholy[zaciatok->vrchol]->susedia[i] - 1;
        novy->dlzka = zaciatok->dlzka + 1;
        novy->rodic = zaciatok->vrchol;
        koniec->dalsi = novy;
        koniec = koniec->dalsi;
        novy = NULL;
      }
    }
    
    zaciatok = zaciatok->dalsi;
  }
  
  if (!zaciatok) {
    graf->vrcholy[zdroj]->prejdeny = true;
    return -1;
  }
  
  if (zaciatok->vrchol == zdroj) {
    graf->vrcholy[zdroj]->prejdeny = true;
    return zaciatok->dlzka;
  }
  
  graf->vrcholy[zdroj]->prejdeny = true;
  return -1;
}

int main() {
  int i, j, k, l, pocet_susedov, K, N, result=-1, temp;
  
  //nacitanie poctu grafov
  scanf("%d\n", &K);
  
  for (i = 0; i < K; i++) {
    //nacitanie poctu vrcholov
    scanf("\n%d\n", &N);
    
    //alokacia grafu a jeho vrcholov
    graf = (GRAF *) malloc (sizeof(GRAF));
    graf->pocet_vrcholov = N;
    graf->vrcholy = (VRCHOL **) malloc (N * sizeof(VRCHOL *));
    for (l = 0; l < N; l++) {
      graf->vrcholy[l] = (VRCHOL *) malloc (sizeof(VRCHOL));
      graf->vrcholy[l]->prejdeny = false;
    }
    
    //nacitanie hodnot jednotlivych vrcholov
    for (j = 0; j < N; j++) {
      //nacitanie poctu susedov vrchola
      scanf("%d ", &pocet_susedov);

      //alokacia susedov vrchola
      graf->vrcholy[j]->pocet_susedov = pocet_susedov;
      graf->vrcholy[j]->susedia = (int *) malloc (pocet_susedov * sizeof(int));
      
      //nacitanie susedov vrchola
      for (k = 0; k < pocet_susedov; k++) {
        scanf("%d ", &graf->vrcholy[j]->susedia[k]);
      }
      
      //ak ma <2 susedov tak sa ani neprehladava
      if (pocet_susedov < 2) graf->vrcholy[j]->prejdeny = true;
    }
    
    int akt;
    bool test=false;
    VRCHOL *pom;
    
    //kontrola "obojsmernosti" hran - ak je cesta z 1 do 4, tak ci je cesta aj z 4 do 1
    for (j = 0; j < N; j++) {
      for (k = 0; k < graf->vrcholy[j]->pocet_susedov; k++) {
        akt = graf->vrcholy[j]->susedia[k];	//cislo aktualneho susedneho bodu
        pom = graf->vrcholy[akt - 1];	//aktualny susedny bod
        
        test = false;
        
        //hlada v zozname susedov bod j
        for (l = 0; l < pom->pocet_susedov; l++) {
          if (pom->susedia[l] == j + 1) {
            //printf("Nasiel sa bod j(od 0 po N-1) %d\n", j);
            test = true;
            break;
          }
        }
        
        if (test == false) {
          pom->pocet_susedov += 1;
          pom->susedia = (int *) realloc (pom->susedia, pom->pocet_susedov * sizeof(int));
          pom->susedia[pocet_susedov] = j + 1;
        }
      }
    }
    
    //algoritmus na vypocet
    for (j = 0; j < N; j++) {
      if (result == 3) {
        //printf("Koniec programu, nasiel sa najkratsi mozny cyklus dlzky 3\n");
        break;
      }
      if (!graf->vrcholy[j]->prejdeny) {
        //printf("Vrchol este nebol prejdeny\n");
        zaciatok = NULL;
        koniec = NULL;
        //printf("addFront(%d)\n", j);
        temp = addFront(j);
        if ((temp > 0) && ((result < 0) || (temp < result))) result = temp;
      }
    }
    
    printf("%d\n", result);
    free(graf);
    result = -1;
  }
  
  return 0;
}