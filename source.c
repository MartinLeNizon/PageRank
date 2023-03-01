#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct{
    int n; // number of vertices in the graph
    int* nbSucc; // for each 0<=i<n, nbSucc[i] = number of successors of i
    int** succ; // for each 0<=i<n and each 0<=j<nbSucc[i], succ[i][j]=jth successor of i
} DIGRAPH;


DIGRAPH* readDigraph(FILE *fp){
    // return the DIGRAPH contained in file fp
    DIGRAPH *g = (DIGRAPH*)malloc(sizeof(DIGRAPH));
    if (fscanf(fp, "%d\n", &(g->n)) != 1 || g->n <= 0){
        printf("erreur de lecture du fichier\n");
        exit(1);
    }
    g->nbSucc = (int*)malloc(g->n*sizeof(int));
    g->succ = (int**)malloc(g->n*sizeof(int*));
    int succ[g->n];
    for (int i=0; i<g->n; i++){
        g->nbSucc[i] = 0;
        while (1){
            if (fscanf(fp, "%d", &(succ[g->nbSucc[i]])) != 1 || succ[g->nbSucc[i]] >= g->n){
                printf("erreur de lecture du fichier\n");
                exit(1);
            }
            if (succ[g->nbSucc[i]]<0) break;
            g->nbSucc[i]++;
        };
        g->succ[i] = (int*)malloc(g->nbSucc[i]*sizeof(int));
        memcpy(g->succ[i], succ, g->nbSucc[i]*sizeof(int));
    }
    return g;
}

void printDigraph(DIGRAPH *g){
    // For each vertex of g, display the list of its successors
    for (int i=0; i<g->n; i++){
        printf("Vertex %d has %d successors: ", i, g->nbSucc[i]);
        for (int j=0; j<g->nbSucc[i]; j++)
            printf("%d ", g->succ[i][j]);
        printf("\n");
    }
}

void copyArray(double *dest, double *source, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

void displayScores(double *scores, int size, int k) {
    printf("s%d = ( ", k);
    for (int i = 0; i < size; i++) {
        printf("%lf ", scores[i]);
    }
    printf(")\n");
}

void resetScores(double *scores, int size) {
    for (int i = 0; i < size; i++) {
        scores[i] = 0;
    }
}

// Question 2
/* void computeNextScores(double *scores, double *previousScores, DIGRAPH *g) {
    resetScores(scores, g->n);      // Initialise les nouveaux scores à 0

    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->nbSucc[i]; j++) {
            scores[g->succ[i][j]] += previousScores[i]/g->nbSucc[i];
        }
    }
} */

// Question 4
/*void computeNextScores(double *scores, double *previousScores, DIGRAPH *g) {
    resetScores(scores, g->n);      // Initialise les nouveaux scores à 0
    double qAbs = 0;

    for (int i = 0; i < g->n; i++) {
        if (g->nbSucc[i] == 0) {
            qAbs += previousScores[i];
        }
        for (int j = 0; j < g->nbSucc[i]; j++) {
            scores[g->succ[i][j]] += previousScores[i]/g->nbSucc[i];
        }
    }

    qAbs = qAbs/g->n;

    for (int j = 0; j < g->n; j++) {
        scores[j] += qAbs;
    }
}*/

// Question 6
void computeNextScores(double *scores, double *previousScores, DIGRAPH *g, const double alpha) {
    resetScores(scores, g->n);      // Initialise les nouveaux scores à 0
    double qAbs = 0;

    for (int i = 0; i < g->n; i++) {
        if (g->nbSucc[i] == 0) {
            qAbs += previousScores[i];
        }
        for (int j = 0; j < g->nbSucc[i]; j++) {
            scores[g->succ[i][j]] += (alpha*previousScores[i])/g->nbSucc[i];
        }
    }

    qAbs = qAbs/g->n;

    for (int j = 0; j < g->n; j++) {
        scores[j] += alpha*qAbs;
        scores[j] += (1-alpha)/g->n;
    }
}

void displaySum(double *scores, int size, int k) {
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += scores[i];
    }
    printf("Somme(k=%d) = %lf\n", k, sum);
}

int converge(double *previousScores, double *scores, int size, double epsilon) {
    for (int i = 0; i < size; i++) {
        if (previousScores[i]-scores[i] >= epsilon || previousScores[i]-scores[i] <= -1*epsilon) return 0;
    }

    return 1;
}

void top5(double *scores, int size) {

    printf("Le top 5 est : \n");

    int maxIndex;

    for (int i = 0; i < 5; i++) {
        maxIndex = 0;
        for (int j = 1; j < size; j++) {
            if (scores[j] > scores[maxIndex]) {
                maxIndex = j;
            }
        }
        scores[maxIndex] = 0;       // car toutes valeurs positive
        if (i < 4) printf("%d, ", maxIndex);
    }

    printf("%d\n", maxIndex);

}

int main(){
    FILE* fp  = fopen("genetic.txt", "r");
    DIGRAPH* g = readDigraph(fp);
    fclose(fp);
    // printDigraph(g);


    double previousScores[g->n];
    double scores[g->n];

    double epsilon = 1e-10;

    double alpha = 0.9;     // Après question 5 bis

    // Initialisation scores s0
    int k = 0;
    for (int i = 0; i < g->n; i++) {
        scores[i] = 1.0/(g->n);
    }
    // displayScores(scores, g->n, k);

    /*for (k=1; k <= 4; k++) {
        copyArray(previousScores, scores, g->n);
        computeNextScores(scores, previousScores, g);
        displayScores(scores, g->n, k);
        displaySum(scores, g->n, k);
    }*/   // Fin exercice 2

    // A partir 5 bis
    while ( !converge(previousScores, scores, g->n, epsilon) ) {
        k++;
        copyArray(previousScores, scores, g->n);
        computeNextScores(scores, previousScores, g, alpha);
        // displaySum(scores, g->n, k);
    }

    
    // displayScores(scores, g->n, k-1);

    top5(scores, g->n);

    return 0;
}