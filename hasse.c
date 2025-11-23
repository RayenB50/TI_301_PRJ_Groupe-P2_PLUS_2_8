#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasse.h"

// Fonction pour supprimer les liens transitifs
void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->log_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->log_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.from == link2.from)
                {
                    // look for a link from link2.to to link1.to
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.from == link2.to) && (link3.to == link1.to))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        }
        else
        {
            i++;
        }
    }
}


// Trouver les liens entre classes
t_link_array* trouver_liens_entre_classes(t_liste_adjacence *graphe, t_partition *partition) {
    t_link_array *liens = (t_link_array*)malloc(sizeof(t_link_array));
    liens->capacity = 20;
    liens->links = (t_link*)malloc(liens->capacity * sizeof(t_link));
    liens->log_size = 0;

    int *tableau_classes = creer_tableau_classes(partition, graphe->nb_sommets);

    // Parcourir tous les sommets et leurs arêtes
    for (int i = 1; i <= graphe->nb_sommets; i++) {
        int classe_i = tableau_classes[i];
        t_cellule *courant = graphe->listes[i].head;

        while (courant != NULL) {
            int j = courant->sommet_arrivee;
            int classe_j = tableau_classes[j];

            if (classe_i != classe_j) {
                // Vérifier si le lien existe déjà
                int existe = 0;
                for (int k = 0; k < liens->log_size; k++) {
                    if (liens->links[k].from == classe_i &&
                        liens->links[k].to == classe_j) {
                        existe = 1;
                        break;
                    }
                }

                if (!existe) {
                    if (liens->log_size >= liens->capacity) {
                        liens->capacity *= 2;
                        liens->links = (t_link*)realloc(liens->links,
                                                        liens->capacity * sizeof(t_link));
                    }
                    liens->links[liens->log_size].from = classe_i;
                    liens->links[liens->log_size].to = classe_j;
                    liens->log_size++;
                }
            }

            courant = courant->suiv;
        }
    }

    free(tableau_classes);
    return liens;
}

// Générer le diagramme de Hasse en Mermaid
void generer_diagramme_hasse_mermaid(t_partition *partition, t_link_array *liens, const char *nom_fichier) {
    char filepath[256];
    sprintf(filepath, "../output/%s_hasse.mmd", nom_fichier);

    FILE *file = fopen(filepath, "w");
    if (!file) return;

    fprintf(file, "graph TD\n");

    // Déclarer les classes
    for (int i = 0; i < partition->nb_classes; i++) {
        fprintf(file, "  %s[\"", partition->classes[i]->nom);
        fprintf(file, "{");
        for (int j = 0; j < partition->classes[i]->nb_sommets; j++) {
            fprintf(file, "%d", partition->classes[i]->sommets[j]);
            if (j < partition->classes[i]->nb_sommets - 1) fprintf(file, ",");
        }
        fprintf(file, "}\"]\n");
    }

    // Ajouter les liens
    for (int i = 0; i < liens->log_size; i++) {
        fprintf(file, "  C%d --> C%d\n",
                liens->links[i].from + 1,
                liens->links[i].to + 1);
    }

    fclose(file);
    printf("Diagramme de Hasse généré : %s\n", filepath);
}

// Analyser les propriétés du graphe
void analyser_proprietes_graphe(t_partition *partition, t_link_array *liens) {
    printf("\n=== Propriétés du graphe ===\n");

    // Marquer les classes avec liens sortants
    int *a_liens_sortants = (int*)calloc(partition->nb_classes, sizeof(int));

    for (int i = 0; i < liens->log_size; i++) {
        a_liens_sortants[liens->links[i].from] = 1;
    }

    // Analyser chaque classe
    for (int i = 0; i < partition->nb_classes; i++) {
        printf("Classe %s: ", partition->classes[i]->nom);

        if (a_liens_sortants[i]) {
            printf("TRANSITOIRE\n");
        } else {
            printf("PERSISTANTE");
            if (partition->classes[i]->nb_sommets == 1) {
                printf(" (État %d est ABSORBANT)", partition->classes[i]->sommets[0]);
            }
            printf("\n");
        }
    }

    // Vérifier l'irréductibilité
    if (partition->nb_classes == 1) {
        printf("\nLe graphe est IRRÉDUCTIBLE\n");
    } else {
        printf("\nLe graphe n'est PAS IRRÉDUCTIBLE (%d classes)\n", partition->nb_classes);
    }

    free(a_liens_sortants);
}

// Libérer la mémoire
void liberer_link_array(t_link_array *array) {
    if (array) {
        free(array->links);
        free(array);
    }
}