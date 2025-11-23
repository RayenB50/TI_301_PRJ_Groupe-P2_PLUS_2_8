#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

// Créer une matrice
t_matrix creer_matrice(int rows, int cols) {
    t_matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = (float**)malloc(rows * sizeof(float*));
    
    for (int i = 0; i < rows; i++) {
        mat.data[i] = (float*)calloc(cols, sizeof(float));
    }
    
    return mat;
}

// Créer une matrice carrée vide
t_matrix creer_matrice_vide(int n) {
    return creer_matrice(n, n);
}

// Libérer la mémoire d'une matrice
void liberer_matrice(t_matrix *mat) {
    if (mat && mat->data) {
        for (int i = 0; i < mat->rows; i++) {
            free(mat->data[i]);
        }
        free(mat->data);
        mat->data = NULL;
    }
}

// Copier une matrice
void copier_matrice(t_matrix *dest, t_matrix *src) {
    if (dest->rows != src->rows || dest->cols != src->cols) return;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[i][j] = src->data[i][j];
        }
    }
}

// Afficher une matrice
void afficher_matrice(t_matrix *mat, const char *nom) {
    printf("\n=== Matrice %s (%dx%d) ===\n", nom, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%.4f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

// Convertir graphe en matrice
t_matrix graphe_vers_matrice(t_liste_adjacence *graphe) {
    int n = graphe->nb_sommets;
    t_matrix M = creer_matrice(n, n);
    
    for (int i = 1; i <= n; i++) {
        t_cellule *courant = graphe->listes[i].head;
        while (courant != NULL) {
            M.data[i-1][courant->sommet_arrivee-1] = courant->probabilite;
            courant = courant->suiv;
        }
    }
    
    return M;
}

// Multiplier deux matrices
t_matrix multiplier_matrices(t_matrix *A, t_matrix *B) {
    t_matrix C = creer_matrice(A->rows, B->cols);
    
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->cols; j++) {
            float sum = 0.0;
            for (int k = 0; k < A->cols; k++) {
                sum += A->data[i][k] * B->data[k][j];
            }
            C.data[i][j] = sum;
        }
    }
    
    return C;
}

// Calculer M^n
t_matrix puissance_matrice(t_matrix *M, int n) {
    t_matrix result = creer_matrice(M->rows, M->cols);
    t_matrix temp = creer_matrice(M->rows, M->cols);
    
    // Initialiser avec matrice identité
    for (int i = 0; i < M->rows; i++) {
        result.data[i][i] = 1.0;
    }
    
    copier_matrice(&temp, M);
    
    // Calculer M^n par multiplications successives
    for (int i = 0; i < n; i++) {
        t_matrix new_result = multiplier_matrices(&result, M);
        liberer_matrice(&result);
        result = new_result;
    }
    
    liberer_matrice(&temp);
    return result;
}

// Calculer la différence entre deux matrices
float difference_matrices(t_matrix *A, t_matrix *B) {
    float diff = 0.0;
    
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            diff += fabs(A->data[i][j] - B->data[i][j]);
        }
    }
    
    return diff;
}

// Calculer la distribution stationnaire
void calculer_distribution_stationnaire(t_matrix *M, int max_iter) {
    printf("\n=== Recherche de la distribution stationnaire ===\n");
    
    t_matrix M_n = creer_matrice(M->rows, M->cols);
    t_matrix M_n_plus_1;
    copier_matrice(&M_n, M);
    
    int n = 1;
    float epsilon = 0.01;
    float diff;
    
    while (n < max_iter) {
        M_n_plus_1 = multiplier_matrices(&M_n, M);
        diff = difference_matrices(&M_n, &M_n_plus_1);
        
        if (diff < epsilon) {
            printf("Convergence après %d itérations\n", n);
            printf("Distribution stationnaire : π* = (");
            for (int j = 0; j < M_n.cols; j++) {
                printf("%.4f", M_n.data[0][j]);
                if (j < M_n.cols - 1) printf(", ");
            }
            printf(")\n");
            
            liberer_matrice(&M_n);
            liberer_matrice(&M_n_plus_1);
            return;
        }
        
        liberer_matrice(&M_n);
        M_n = M_n_plus_1;
        n++;
    }
    
    printf("Pas de convergence après %d itérations\n", max_iter);
    liberer_matrice(&M_n);
}

// Extraire sous-matrice pour une classe
t_matrix extraire_sous_matrice(t_matrix *M, t_partition *partition, int index_classe) {
    t_classe *classe = partition->classes[index_classe];
    int n = classe->nb_sommets;
    t_matrix sous_mat = creer_matrice(n, n);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sommet_i = classe->sommets[i] - 1;
            int sommet_j = classe->sommets[j] - 1;
            sous_mat.data[i][j] = M->data[sommet_i][sommet_j];
        }
    }
    
    return sous_mat;
}

// Fonction auxiliaire : PGCD
static int pgcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// PGCD d'un tableau
static int pgcd_tableau(int *vals, int nbvals) {
    if (nbvals == 0) return 0;
    int result = vals[0];
    for (int i = 1; i < nbvals; i++) {
        result = pgcd(result, vals[i]);
    }
    return result;
}

// Calculer la période d'une sous-matrice
int calculer_periode(t_matrix *sous_matrice) {
    int n = sous_matrice->rows;
    int *periodes = (int*)malloc(n * sizeof(int));
    int nb_periodes = 0;
    
    t_matrix puissance = creer_matrice(n, n);
    t_matrix temp;
    copier_matrice(&puissance, sous_matrice);
    
    for (int cpt = 1; cpt <= n; cpt++) {
        int diag_non_zero = 0;
        
        for (int i = 0; i < n; i++) {
            if (puissance.data[i][i] > 0.0f) {
                diag_non_zero = 1;
                break;
            }
        }
        
        if (diag_non_zero) {
            periodes[nb_periodes++] = cpt;
        }
        
        temp = multiplier_matrices(&puissance, sous_matrice);
        liberer_matrice(&puissance);
        puissance = temp;
    }
    
    int periode = pgcd_tableau(periodes, nb_periodes);
    
    free(periodes);
    liberer_matrice(&puissance);
    
    return periode;
}
