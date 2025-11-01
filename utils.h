#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>

// Structure pour une cellule (arête) de la liste chaînée
typedef struct s_cellule {
    int sommet_arrivee;
    float probabilite;
    struct s_cellule *suiv;
} t_cellule;

// Structure pour une liste chaînée
typedef struct {
    t_cellule *head;
} t_liste;

// Structure pour la liste d'adjacence
typedef struct {
    t_liste *listes;
    int nb_sommets;
} t_liste_adjacence;

// Fonction getID déjà existante (déclaration)
char* getID(int i);

// Fonctions pour les cellules et listes
t_cellule* creer_cellule(int sommet_arrivee, float probabilite);
t_liste* creer_liste_vide();
void ajouter_cellule(t_liste *liste, t_cellule *cellule);
void afficher_liste(t_liste *liste, int sommet_depart);
void liberer_liste(t_liste *liste);

// Fonctions pour la liste d'adjacence
t_liste_adjacence* creer_liste_adjacence_vide(int nb_sommets);
void afficher_liste_adjacence(t_liste_adjacence *adj);
void liberer_liste_adjacence(t_liste_adjacence *adj);

// Fonctions principales du projet
t_liste_adjacence* lire_graphe(const char *filename);
int verifier_graphe_markov(t_liste_adjacence *adj);
void generer_fichier_mermaid(t_liste_adjacence *adj, const char *output_filename);

#endif