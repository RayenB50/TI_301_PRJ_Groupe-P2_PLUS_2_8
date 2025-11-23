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


