#ifndef __HASSE_H__
#define __HASSE_H__

#include "utils.h"
// Structure pour un lien entre classes
typedef struct {
    int from; // Classe de départ
    int to;   // Classe d'arrivée
} t_link;

// Structure pour stocker les liens
typedef struct {
    t_link *links;
    int log_size;    // Nombre de liens
    int capacity;    // Capacité
} t_link_array;


void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

t_link_array* creer_link_array(int capacite);
void ajouter_link(t_link_array *array, int from, int to);
void liberer_link_array(t_link_array *array);
t_link_array* trouver_liens_entre_classes(t_liste_adjacence *graphe, t_partition *partition);
void generer_diagramme_hasse_mermaid(t_partition *partition, t_link_array *liens, const char *filename);

#endif
