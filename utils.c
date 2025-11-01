#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"

static char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

// Créer une nouvelle cellule
t_cellule* creer_cellule(int sommet_arrivee, float probabilite) {
    t_cellule *nouvelle = (t_cellule*)malloc(sizeof(t_cellule));
    if (nouvelle == NULL) {
        perror("Erreur d'allocation mémoire pour cellule");
        exit(EXIT_FAILURE);
    }
    nouvelle->sommet_arrivee = sommet_arrivee;
    nouvelle->probabilite = probabilite;
    nouvelle->suiv = NULL;
    return nouvelle;
}

// Créer une liste vide
t_liste* creer_liste_vide() {
    t_liste *liste = (t_liste*)malloc(sizeof(t_liste));
    if (liste == NULL) {
        perror("Erreur d'allocation mémoire pour liste");
        exit(EXIT_FAILURE);
    }
    liste->head = NULL;
    return liste;
}

// Ajouter une cellule à une liste
void ajouter_cellule(t_liste *liste, t_cellule *cellule) {
    if (liste == NULL || cellule == NULL) return;

    cellule->suiv = liste->head;
    liste->head = cellule;
}

// Libérer la mémoire d'une liste
void liberer_liste(t_liste *liste) {
    if (liste == NULL) return;

    t_cellule *courant = liste->head;
    while (courant != NULL) {
        t_cellule *temp = courant;
        courant = courant->suiv;
        free(temp);
    }
    free(liste);
}

// Créer une liste d'adjacence vide
t_liste_adjacence* creer_liste_adjacence_vide(int nb_sommets) {
    t_liste_adjacence *adj = (t_liste_adjacence*)malloc(sizeof(t_liste_adjacence));
    if (adj == NULL) {
        perror("Erreur d'allocation mémoire pour liste d'adjacence");
        exit(EXIT_FAILURE);
    }

    adj->nb_sommets = nb_sommets;
    adj->listes = (t_liste*)malloc((nb_sommets + 1) * sizeof(t_liste));
    if (adj->listes == NULL) {
        perror("Erreur d'allocation mémoire pour tableau de listes");
        exit(EXIT_FAILURE);
    }

    // Initialiser toutes les listes comme vides
    for (int i = 0; i <= nb_sommets; i++) {
        adj->listes[i].head = NULL;
    }

    return adj;
}

// Afficher la liste d'adjacence
void afficher_liste_adjacence(t_liste_adjacence *adj) {
    if (adj == NULL) return;

    printf("\n=== Liste d'adjacence ===\n");
    for (int i = 1; i <= adj->nb_sommets; i++) {
        afficher_liste(&adj->listes[i], i);
    }
    printf("\n");
}

// Libérer la mémoire de la liste d'adjacence
