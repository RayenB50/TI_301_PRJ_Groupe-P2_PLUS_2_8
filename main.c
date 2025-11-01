#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void afficher_menu() {
    printf("\n=======================================\n");
    printf("            GRAPHES DE MARKOV           \n");
    printf("=======================================\n");
    printf("1. Charger et afficher un graphe\n");
    printf("2. Vérifier si c'est un graphe de Markov\n");
    printf("3. Générer le fichier Mermaid\n");
    printf("4. Tout faire (charger, vérifier, générer)\n");
    printf("5. Changer de fichier\n");
    printf("0. Quitter\n");
    printf("=======================================\n");
    printf("Votre choix : ");
}

int main(int argc, char *argv[]) {
    t_liste_adjacence *graphe = NULL;
    char filename[256] = "";
    char output_filename[256] = "";
    int choix;
    int graphe_charge = 0;

    printf("=======================================\n");
    printf("            GRAPHES DE MARKOV           \n");
    printf("=======================================\n");

    // Demander le fichier d'entrée
    if (argc > 1) {
        strcpy(filename, argv[1]);
    } else {
        printf("Entrez le nom du fichier de données : ");
        scanf("%s", filename);
    }

    do {
        afficher_menu();
        scanf("%d", &choix);

        switch(choix) {
            case 1: // Charger et afficher
                if (graphe != NULL) {
                    liberer_liste_adjacence(graphe);
                }
                printf("\n=== ÉTAPE 1 : Chargement du graphe ===\n");
                printf("Lecture du fichier : %s\n", filename);
                graphe = lire_graphe(filename);
                afficher_liste_adjacence(graphe);
                graphe_charge = 1;
                printf("Graphe chargé avec succès !\n");
                break;

            case 2: // Vérifier le graphe
                if (!graphe_charge) {
                    printf("\nErreur : Aucun graphe chargé. Chargez d'abord un graphe (option 1).\n");
                } else {
                    printf("\n=== ÉTAPE 2 : Vérification du graphe ===\n");
                    verifier_graphe_markov(graphe);
                }
                break;

            case 3: // Générer fichier Mermaid
                if (!graphe_charge) {
                    printf("\nErreur : Aucun graphe chargé. Chargez d'abord un graphe (option 1).\n");
                } else {
                    printf("\n=== ÉTAPE 3 : Génération du fichier Mermaid ===\n");
                    printf("Nom du fichier de sortie (ex: graphe.mmd) : ");
                    scanf("%s", output_filename);
                    generer_fichier_mermaid(graphe, output_filename);
                    printf("\nPour visualiser le graphe :\n");
                    printf("1. Copiez le contenu du fichier %s\n", output_filename);
                    printf("2. Collez-le sur https://www.mermaidchart.com/\n");
                }
                break;

            case 4: // Tout faire
                printf("\n=== EXÉCUTION COMPLÈTE DES 3 ÉTAPES ===\n");

                // Étape 1
                if (graphe != NULL) {
                    liberer_liste_adjacence(graphe);
                }
                printf("\n--- ÉTAPE 1 : Chargement du graphe ---\n");
                printf("Lecture du fichier : %s\n", filename);
                graphe = lire_graphe(filename);
                afficher_liste_adjacence(graphe);
                graphe_charge = 1;

                // Étape 2
                printf("\n--- ÉTAPE 2 : Vérification ---\n");
                int valide = verifier_graphe_markov(graphe);

                // Étape 3
                if (valide) {
                    printf("\n--- ÉTAPE 3 : Génération Mermaid ---\n");
                    // Générer automatiquement le nom du fichier de sortie
                    strcpy(output_filename, filename);
                    char *ext = strrchr(output_filename, '.');
                    if (ext) {
                        strcpy(ext, "_mermaid.mmd");
                    } else {
                        strcat(output_filename, "_mermaid.mmd");
                    }
                    generer_fichier_mermaid(graphe, output_filename);
                    printf("\nPour visualiser le graphe :\n");
                    printf("1. Copiez le contenu du fichier %s\n", output_filename);
                    printf("2. Collez-le sur https://www.mermaidchart.com/\n");
                } else {
                    printf("\nLe graphe n'est pas valide, génération Mermaid annulée.\n");
                }
                break;

            case 5: // Changer de fichier
                printf("Entrez le nom du nouveau fichier : ");
                scanf("%s", filename);
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