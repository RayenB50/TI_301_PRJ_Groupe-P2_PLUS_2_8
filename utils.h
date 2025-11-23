#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>

//     STRUCTURES PARTIE 1

// Structure pour une cellule (arête) de la liste chaînée
typedef struct s_cellule {
    int sommet_arrivee;     // Sommet destination
    float probabilite;      // Probabilité de transition
    struct s_cellule *suiv; // Pointeur vers la cellule suivante
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

//     STRUCTURES PARTIE 2

// Structure représentant un sommet dans l'algorithme de Tarjan
typedef struct {
    int id;                 // Identifiant du sommet (1, 2, 3, ...)
    int num;                // Numéro d'ordre de visite (-1 si non visité)
    int num_accessible;     // Plus petit numéro accessible depuis ce sommet
    int dans_pile;
} t_tarjan_vertex;

// Structure représentant une classe (composante fortement connexe)
typedef struct {
    char nom[10];
    int *sommets;
    int nb_sommets;
    int capacite;
} t_classe;

// Structure représentant une partition du graphe
typedef struct {
    t_classe **classes;
    int nb_classes;
    int capacite;
} t_partition;

// Structure pour une pile d'entiers
typedef struct {
    int *data;
    int top;
    int capacite;
} t_pile;

//     FONCTIONS PARTIE 1

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

//     FONCTIONS PARTIE 2

// Fonctions pour la pile
t_pile* creer_pile(int capacite);        // Créer une pile vide
void empiler(t_pile *pile, int val);     // Ajouter un élément au sommet
int depiler(t_pile *pile);               // Retirer et retourner le sommet
int pile_vide(t_pile *pile);             // Vérifier si la pile est vide
void liberer_pile(t_pile *pile);         // Libérer la mémoire de la pile

// Fonctions pour les classes 
t_classe* creer_classe(const char *nom); // Créer une nouvelle classe
void ajouter_sommet_classe(t_classe *classe, int sommet); // Ajouter un sommet
void liberer_classe(t_classe *classe);   // Libérer la mémoire d'une classe

// Fonctions pour la partition 
t_partition* creer_partition();          // Créer une partition vide
void ajouter_classe_partition(t_partition *partition, t_classe *classe);
void liberer_partition(t_partition *partition);
void afficher_partition(t_partition *partition);

// Algorithme de Tarjan 
t_tarjan_vertex* initialiser_sommets_tarjan(t_liste_adjacence *graphe);
void parcours(int v, t_liste_adjacence *graphe, t_tarjan_vertex *sommets, t_pile *pile, int *num_courant, t_partition *partition);
t_partition* algorithme_tarjan(t_liste_adjacence *graphe);

// Analyse des propriétés
int* creer_tableau_classes(t_partition *partition, int nb_sommets);

#endif
