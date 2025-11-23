#ifndef MATRIX_H
#define MATRIX_H

#include "utils.h"

// Structure pour une matrice
typedef struct {
    float **data;  // Données de la matrice
    int rows;      // Nombre de lignes
    int cols;      // Nombre de colonnes
} t_matrix;

// Fonctions de base
t_matrix creer_matrice(int rows, int cols);
t_matrix creer_matrice_vide(int n);
void liberer_matrice(t_matrix *mat);
void copier_matrice(t_matrix *dest, t_matrix *src);
void afficher_matrice(t_matrix *mat, const char *nom);

// Conversion et opérations
t_matrix graphe_vers_matrice(t_liste_adjacence *graphe);
t_matrix multiplier_matrices(t_matrix *A, t_matrix *B);
t_matrix puissance_matrice(t_matrix *M, int n);
float difference_matrices(t_matrix *A, t_matrix *B);

// Distributions
void calculer_distribution_stationnaire(t_matrix *M, int max_iter);
t_matrix extraire_sous_matrice(t_matrix *M, t_partition *partition, int index_classe);

// Période (bonus)
int calculer_periode(t_matrix *sous_matrice);

#endif //MATRIX_H
