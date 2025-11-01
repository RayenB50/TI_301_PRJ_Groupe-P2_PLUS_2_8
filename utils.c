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
void liberer_liste_adjacence(t_liste_adjacence *adj) {
    if (adj == NULL) return;

    for (int i = 0; i <= adj->nb_sommets; i++) {
        t_cellule *courant = adj->listes[i].head;
        while (courant != NULL) {
            t_cellule *temp = courant;
            courant = courant->suiv;
            free(temp);
        }
    }
    free(adj->listes);
    free(adj);
}

// Lire un graphe depuis un fichier
t_liste_adjacence* lire_graphe(const char *filename) {
    FILE *file = fopen(filename, "rt");
    int nbvert, depart, arrivee;
    float proba;
    t_liste_adjacence *adj = NULL;

    if (file == NULL) {
        perror("Impossible d'ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    // Lire le nombre de sommets
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Impossible de lire le nombre de sommets");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Créer la liste d'adjacence vide
    adj = creer_liste_adjacence_vide(nbvert);

    // Lire les arêtes
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        // Créer une cellule pour cette arête
        t_cellule *nouvelle = creer_cellule(arrivee, proba);
        // Ajouter la cellule à la liste du sommet de départ
        ajouter_cellule(&adj->listes[depart], nouvelle);
    }

    fclose(file);
    return adj;
}

// Vérifier si c'est un graphe de Markov valide
int verifier_graphe_markov(t_liste_adjacence *adj) {
    if (adj == NULL) return 0;

    int valide = 1;
    printf("\n=== Vérification du graphe de Markov ===\n");

    for (int i = 1; i <= adj->nb_sommets; i++) {
        float somme = 0.0;
        t_cellule *courant = adj->listes[i].head;

        // Calculer la somme des probabilités sortantes
        while (courant != NULL) {
            somme += courant->probabilite;
            courant = courant->suiv;
        }

        // Vérifier si la somme est proche de 1 (entre 0.99 et 1.01 pour les arrondis)
        if (somme < 0.99 || somme > 1.01) {
            printf("Erreur : la somme des probabilités du sommet %d est %.2f\n", i, somme);
            valide = 0;
        }
    }

    if (valide) {
        printf("Le graphe est un graphe de Markov\n");
    } else {
        printf("Le graphe n'est pas un graphe de Markov\n");
    }

    return valide;
}

// Générer un fichier au format Mermaid
void generer_fichier_mermaid(t_liste_adjacence *adj, const char *output_filename) {
    if (adj == NULL) return;

    FILE *file = fopen(output_filename, "w");
    if (file == NULL) {
        perror("Impossible de créer le fichier de sortie");
        return;
    }

    // Écrire l'en-tête de configuration Mermaid
    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "  layout: elk\n");
    fprintf(file, "  theme: neo\n");
    fprintf(file, "  look: neo\n");
    fprintf(file, "---\n\n");
    fprintf(file, "flowchart LR\n");

    // Déclarer tous les sommets
    for (int i = 1; i <= adj->nb_sommets; i++) {
        fprintf(file, "%s((%d))\n", getID(i), i);
    }
    fprintf(file, "\n");

    // Écrire toutes les arêtes avec leurs probabilités
    for (int i = 1; i <= adj->nb_sommets; i++) {
        t_cellule *courant = adj->listes[i].head;

        // Parcourir la liste dans l'ordre inverse pour respecter l'ordre du fichier
        // D'abord, compter les éléments
        int count = 0;
        t_cellule *temp = courant;
        while (temp != NULL) {
            count++;
            temp = temp->suiv;
        }

        // Créer un tableau temporaire pour inverser l'ordre
        if (count > 0) {
            t_cellule **arr = (t_cellule**)malloc(count * sizeof(t_cellule*));
            temp = courant;
            for (int j = count - 1; j >= 0; j--) {
                arr[j] = temp;
                temp = temp->suiv;
            }

            // Écrire les arêtes dans le bon ordre
            for (int j = 0; j < count; j++) {
                fprintf(file, "%s -->|%.2g|%s\n",
                        getID(i),
                        arr[j]->probabilite,
                        getID(arr[j]->sommet_arrivee));
            }

            free(arr);
        }
    }

    fclose(file);
    printf("\nFichier Mermaid généré : %s\n", output_filename);
}
