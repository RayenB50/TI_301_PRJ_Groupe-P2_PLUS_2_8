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
