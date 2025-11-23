#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "hasse.h"
#include "matrix.h"

void afficher_menu() {
    printf("\n=======================================\n");
    printf("            GRAPHES DE MARKOV           \n");
    printf("=======================================\n");
    printf("1. PARTIE 1 - Charger et afficher un graphe\n");
    printf("2. PARTIE 1 - Vérifier si c'est un graphe de Markov\n");
    printf("3. PARTIE 1 - Générer le fichier Mermaid\n");
    printf("4. PARTIE 1 - Tout faire (1+2+3)\n");
    printf("5. PARTIE 2 - Composantes (Tarjan) et Hasse\n");
    printf("6. PARTIE 3 - Calculs matriciels\n");
    printf("7. Changer de fichier\n");
    printf("0. Quitter\n");
    printf("=======================================\n");
    printf("Votre choix : ");
}

int main(int argc, char *argv[]) {
    t_liste_adjacence *graphe = NULL;
    char nom_fichier[256] = "";
    char nom_sortie[256] = "";
    int choix;
    int graphe_charge = 0;

    // Créer les répertoires
    system("mkdir -p data output");

    printf("=======================================\n");
    printf("            GRAPHES DE MARKOV           \n");
    printf("=======================================\n");

    // Demander le fichier d'entrée
    if (argc > 1) {
        strcpy(nom_fichier, argv[1]);
        char *ext = strstr(nom_fichier, ".txt");
        if (ext) *ext = '\0';
    } else {
        printf("Nom du fichier (sans extension, dans data/) : ");
        scanf("%s", nom_fichier);
    }

    do {
        afficher_menu();
        scanf("%d", &choix);

        switch(choix) {
            case 1: // Charger et afficher
                if (graphe != NULL) {
                    liberer_liste_adjacence(graphe);
                }
                printf("\n=== PARTIE 1 - ÉTAPE 1 : Chargement ===\n");
                printf("Lecture du fichier : %s\n", nom_fichier);
                graphe = lire_graphe(nom_fichier);

                if (graphe != NULL) {
                    afficher_liste_adjacence(graphe);
                    graphe_charge = 1;
                    printf("Graphe chargé avec succès !\n");
                } else {
                    graphe_charge = 0;
                }
                break;

            case 2: // Vérifier le graphe Markov
                if (!graphe_charge) {
                    printf("\nErreur : Aucun graphe chargé. Chargez d'abord un graphe (option 1).\n");
                } else {
                    printf("\n=== PARTIE 1 - ÉTAPE 2 : Vérification ===\n");
                    verifier_graphe_markov(graphe);
                }
                break;

            case 3: // Générer fichier Mermaid
                if (!graphe_charge) {
                    printf("\nErreur : Aucun graphe chargé. Chargez d'abord un graphe (option 1).\n");
                } else {
                    printf("\n=== PARTIE 1 - ÉTAPE 3 : Mermaid ===\n");
                    printf("Nom du fichier de sortie (sans extension) : ");
                    scanf("%s", nom_sortie);
                    generer_fichier_mermaid(graphe, nom_sortie);
                    printf("\nPour visualiser le graphe :\n");
                    printf("1. Copiez le contenu du fichier %s\n", nom_sortie);
                    printf("2. Collez-le sur https://www.mermaidchart.com/\n");
                }
                break;

            case 4: // Tout faire PARTIE 1
                printf("\n=== PARTIE 1 COMPLÈTE ===\n");

                if (graphe != NULL) {
                    liberer_liste_adjacence(graphe);
                }

                // ÉTAPE 1 : Charger
                printf("\n--- ÉTAPE 1 : Chargement du graphe ---\n");
                printf("Lecture du fichier : %s\n", nom_fichier);
                graphe = lire_graphe(nom_fichier);
                if (graphe == NULL) break;
                afficher_liste_adjacence(graphe);
                graphe_charge = 1;

                // Étape 2 : Vérifier
                printf("\n--- ÉTAPE 2 : Vérification ---\n");
                int valide = verifier_graphe_markov(graphe);

                // Étape 3 : Générer Mermaid
                if (valide) {
                    printf("\n--- ÉTAPE 3 : Génération Mermaid ---\n");
                    sprintf(nom_sortie, "%s_graphe", nom_fichier);
                    generer_fichier_mermaid(graphe, nom_sortie);

                    printf("\nPour visualiser le graphe :\n");
                    printf("1. Copiez le contenu du fichier %s\n", nom_sortie);
                    printf("2. Collez-le sur https://www.mermaidchart.com/\n");
                } else {
                    printf("\nLe graphe n'est pas valide, génération Mermaid annulée.\n");
                }
                break;

            case 5: // Partie 2 - Tarjan et Hasse
                if (!graphe_charge) {
                    printf("\n✗ Chargez d'abord un graphe (option 1)\n");
                } else {
                    printf("\n=== PARTIE 2 : Tarjan et Hasse ===\n");

                    // Tarjan
                    t_partition *partition = algorithme_tarjan(graphe);
                    afficher_partition(partition);

                    // Liens entre classes
                    t_link_array *liens = trouver_liens_entre_classes(graphe, partition);

                    // Option pour supprimer les liens transitifs
                    printf("\nSupprimer les liens transitifs ? (1=oui, 0=non) : ");
                    int supprimer;
                    scanf("%d", &supprimer);
                    if (supprimer) {
                        removeTransitiveLinks(liens);
                        printf("✓ Liens transitifs supprimés\n");
                    }

                    // Générer Hasse
                    generer_diagramme_hasse_mermaid(partition, liens, nom_fichier);

                    // Propriétés
                    analyser_proprietes_graphe(partition, liens);

                    liberer_partition(partition);
                    liberer_link_array(liens);
                }
                break;

            case 6: // Partie 3 - Matrices
                if (!graphe_charge) {
                    printf("\n✗ Chargez d'abord un graphe (option 1)\n");
                } else {
                    printf("\n=== PARTIE 3 : Calculs matriciels ===\n");
                    t_matrix M = graphe_vers_matrice(graphe);

                    printf("1. Afficher M\n");
                    printf("2. Calculer M^3\n");
                    printf("3. Calculer M^7\n");
                    printf("4. Distribution stationnaire\n");
                    printf("5. Calculer périodes\n");
                    printf("Sous-choix : ");
                    int sous_choix;
                    scanf("%d", &sous_choix);

                    switch(sous_choix) {
                        case 1:
                            afficher_matrice(&M, "M");
                            break;

                        case 2: {
                            t_matrix M3 = puissance_matrice(&M, 3);
                            afficher_matrice(&M3, "M^3");
                            liberer_matrice(&M3);
                            break;
                        }

                        case 3: {
                            t_matrix M7 = puissance_matrice(&M, 7);
                            afficher_matrice(&M7, "M^7");
                            liberer_matrice(&M7);
                            break;
                        }

                        case 4:
                            calculer_distribution_stationnaire(&M, 100);
                            break;

                        case 5: {
                            t_partition *partition = algorithme_tarjan(graphe);
                            printf("\n=== Périodes des classes ===\n");
                            for (int i = 0; i < partition->nb_classes; i++) {
                                t_matrix sous_mat = extraire_sous_matrice(&M, partition, i);
                                int periode = calculer_periode(&sous_mat);
                                printf("Classe %s : période = %d\n",
                                       partition->classes[i]->nom, periode);
                                liberer_matrice(&sous_mat);
                            }
                            liberer_partition(partition);
                            break;
                        }
                    }

                    liberer_matrice(&M);
                }
                break;

            case 7: // Changer de fichier
                printf("Entrez le nom du nouveau fichier (sans extension) : ");
                scanf("%s", nom_fichier);
                graphe_charge = 0;

                if (graphe != NULL) {
                    liberer_liste_adjacence(graphe);
                    graphe = NULL;
                }
                printf("Fichier changé. N'oubliez pas de charger le graphe.\n");
                break;

            case 0: // Quitter
                printf("\nAu revoir !\n");
                break;

            default:
                printf("\nChoix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 0);

    // Libérer la mémoire
    if (graphe != NULL) {
        liberer_liste_adjacence(graphe);
    }

    return 0;
}