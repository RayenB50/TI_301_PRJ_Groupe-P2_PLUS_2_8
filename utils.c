#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"

//     FONCTIONS PARTIE 1

char *getID(int i)
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

// Afficher une liste
void afficher_liste(t_liste *liste, int sommet_depart) {
    if (liste == NULL) return;

    printf("Liste pour le sommet %d: [head] -> ", sommet_depart);
    t_cellule *courant = liste->head;
    while (courant != NULL) {
        printf("(%d, %.2f) -> ", courant->sommet_arrivee, courant->probabilite);
        courant = courant->suiv;
    }
    printf("NULL\n");
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
t_liste_adjacence* lire_graphe(const char *nom_fichier) {
    int nbvert, depart, arrivee;
    float proba;
    t_liste_adjacence *adj = NULL;

    char filepath[256];
    sprintf(filepath, "data/%s.txt", nom_fichier);

    FILE *file = fopen(filepath, "rt");
    if (file == NULL) {
        sprintf(filepath, "%s.txt", nom_fichier);
        file = fopen(filepath, "rt");
        if (file == NULL) {
            file = fopen(nom_fichier, "rt");
            if (file == NULL) {
                printf("Erreur : Impossible d'ouvrir le fichier\n");
                return NULL;
            }
        }
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
void generer_fichier_mermaid(t_liste_adjacence *adj, const char *nom_fichier) {
    if (adj == NULL) return;

    char filepath[256];
    sprintf(filepath, "output/%s.mmd", nom_fichier);

    system("mkdir -p output");
    FILE *file = fopen(filepath, "w");
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
        // Parcourir la liste dans l'ordre inverse pour respecter l'ordre du fichier
        // D'abord, compter les éléments
        int count = 0;
        t_cellule *temp = adj->listes[i].head;
        while (temp != NULL) {
            count++;
            temp = temp->suiv;
        }

        // Créer un tableau temporaire pour inverser l'ordre
        if (count > 0) {
            t_cellule **arr = (t_cellule**)malloc(count * sizeof(t_cellule*));
            temp = adj->listes[i].head;

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
    printf("\nFichier Mermaid généré : %s\n", filepath);
}

//     FONCTIONS PARTIE 2

// Créer une pile
t_pile* creer_pile(int capacite) {
    t_pile *pile = (t_pile*)malloc(sizeof(t_pile));
    pile->data = (int*)malloc(capacite * sizeof(int));
    pile->top = -1;
    pile->capacite = capacite;
    return pile;
}

// Empiler
void empiler(t_pile *pile, int val) {
    if (pile->top < pile->capacite - 1) {
        pile->data[++pile->top] = val;
    }
}

// Dépiler
int depiler(t_pile *pile) {
    if (pile->top >= 0) {
        return pile->data[pile->top--];
    }
    return -1;
}

// Vérifier si pile vide
int pile_vide(t_pile *pile) {
    return pile->top == -1;
}

// Libérer la pile
void liberer_pile(t_pile *pile) {
    if (pile) {
        free(pile->data);
        free(pile);
    }
}

// Créer une classe
t_classe* creer_classe(const char *nom) {
    t_classe *classe = (t_classe*)malloc(sizeof(t_classe));
    strcpy(classe->nom, nom);
    classe->capacite = 10;
    classe->sommets = (int*)malloc(classe->capacite * sizeof(int));
    classe->nb_sommets = 0;
    return classe;
}

// Ajouter un sommet à une classe
void ajouter_sommet_classe(t_classe *classe, int sommet) {
    if (classe->nb_sommets >= classe->capacite) {
        classe->capacite *= 2;
        classe->sommets = (int*)realloc(classe->sommets, classe->capacite * sizeof(int));
    }
    classe->sommets[classe->nb_sommets++] = sommet;
}

// Libérer une classe
void liberer_classe(t_classe *classe) {
    if (classe) {
        free(classe->sommets);
        free(classe);
    }
}

// Créer une partition
t_partition* creer_partition() {
    t_partition *partition = (t_partition*)malloc(sizeof(t_partition));
    partition->capacite = 10;
    partition->classes = (t_classe**)malloc(partition->capacite * sizeof(t_classe*));
    partition->nb_classes = 0;
    return partition;
}

// Ajouter une classe à la partition
void ajouter_classe_partition(t_partition *partition, t_classe *classe) {
    if (partition->nb_classes >= partition->capacite) {
        partition->capacite *= 2;
        partition->classes = (t_classe**)realloc(partition->classes, 
                                                 partition->capacite * sizeof(t_classe*));
    }
    partition->classes[partition->nb_classes++] = classe;
}

// Libérer une partition
void liberer_partition(t_partition *partition) {
    if (partition) {
        for (int i = 0; i < partition->nb_classes; i++) {
            liberer_classe(partition->classes[i]);
        }
        free(partition->classes);
        free(partition);
    }
}

// Afficher la partition
void afficher_partition(t_partition *partition) {
    printf("\n=== Composantes fortement connexes ===\n");
    for (int i = 0; i < partition->nb_classes; i++) {
        printf("Composante %s: {", partition->classes[i]->nom);
        for (int j = 0; j < partition->classes[i]->nb_sommets; j++) {
            printf("%d", partition->classes[i]->sommets[j]);
            if (j < partition->classes[i]->nb_sommets - 1) printf(",");
        }
        printf("}\n");
    }
}

// Fonction auxiliaire min
static int min(int a, int b) {
    return (a < b) ? a : b;
}

// Fonction de parcours pour Tarjan
void parcours(int v, t_liste_adjacence *graphe, t_tarjan_vertex *sommets,
                    t_pile *pile, int *num_courant, t_partition *partition) {
    
    sommets[v].num = *num_courant;
    sommets[v].num_accessible = *num_courant;
    (*num_courant)++;
    
    empiler(pile, v);
    sommets[v].dans_pile = 1;
    
    t_cellule *courant = graphe->listes[v].head;
    while (courant != NULL) {
        int w = courant->sommet_arrivee;
        
        if (sommets[w].num == -1) {
            parcours(w, graphe, sommets, pile, num_courant, partition);
            sommets[v].num_accessible = min(sommets[v].num_accessible, 
                                           sommets[w].num_accessible);
        } else if (sommets[w].dans_pile) {
            sommets[v].num_accessible = min(sommets[v].num_accessible, 
                                           sommets[w].num);
        }
        
        courant = courant->suiv;
    }
    
    if (sommets[v].num_accessible == sommets[v].num) {
        char nom_classe[10];
        sprintf(nom_classe, "C%d", partition->nb_classes + 1);
        t_classe *nouvelle_classe = creer_classe(nom_classe);
        
        int w;
        do {
            w = depiler(pile);
            sommets[w].dans_pile = 0;
            ajouter_sommet_classe(nouvelle_classe, w);
        } while (w != v);
        
        ajouter_classe_partition(partition, nouvelle_classe);
    }
}

// Algorithme de Tarjan principal
t_partition* algorithme_tarjan(t_liste_adjacence *graphe) {
    t_partition *partition = creer_partition();
    
    // Initialiser les sommets
    t_tarjan_vertex *sommets = (t_tarjan_vertex*)malloc((graphe->nb_sommets + 1) * sizeof(t_tarjan_vertex));
    for (int i = 0; i <= graphe->nb_sommets; i++) {
        sommets[i].id = i;
        sommets[i].num = -1;
        sommets[i].num_accessible = -1;
        sommets[i].dans_pile = 0;
    }
    
    t_pile *pile = creer_pile(graphe->nb_sommets);
    int num_courant = 0;
    
    // Parcourir tous les sommets
    for (int i = 1; i <= graphe->nb_sommets; i++) {
        if (sommets[i].num == -1) {
            parcours(i, graphe, sommets, pile, &num_courant, partition);
        }
    }
    
    free(sommets);
    liberer_pile(pile);
    
    return partition;
}

// Créer tableau d'appartenance aux classes
int* creer_tableau_classes(t_partition *partition, int nb_sommets) {
    int *tableau = (int*)malloc((nb_sommets + 1) * sizeof(int));
    
    for (int i = 0; i < partition->nb_classes; i++) {
        for (int j = 0; j < partition->classes[i]->nb_sommets; j++) {
            int sommet = partition->classes[i]->sommets[j];
            tableau[sommet] = i;
        }
    }
    
    return tableau;
}

