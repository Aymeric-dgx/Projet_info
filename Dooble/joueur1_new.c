    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <string.h>
    #include <windows.h>

    int taille = 0;

    // Fonction pour parcourir un fichier et récupérer un mot à une position donnée
    void parcourire_liste(FILE *fil, int in, char *mot) {
        rewind(fil);
        char ligne[50]; // Buffer pour lire chaque ligne
        int compteur = 0;
        while (fgets(ligne, sizeof(ligne), fil) != NULL && compteur != in) {
            compteur++;
            if (compteur == in) {
                ligne[strcspn(ligne, "\n")] = '\0'; // Supprimer le '\n'
                strcpy(mot, ligne); // Copier le mot dans le tableau `mot`
                return;
            }
        }
    }

    // Fonction pour vérifier si un mot est présent dans une liste donnée
    int verification(char liste[50][50], char *mot, int taille) {
        for (int i = 0; i < taille; i++) {
            if (strcmp(liste[i], mot) == 0) {
                return 1; // Le mot existe
            }
        }
        return 0; // Le mot n'existe pas
    }

    // Fonction pour remplir la liste du joueur
    void remplir_liste(char tableau[2][50][50], int plan_cible, FILE *fil, int taille, char *mot_commun) {
        char mot[50];

        if(plan_cible==1) {
            int position=0;
            do {
                position = rand() % taille;
            }while(strcmp(tableau[0][position],mot_commun)==0);
            strcpy(mot_commun,tableau[0][position]);
            strcpy(tableau[1][position], tableau[0][position]);
        }

        for (int i = 0; i < taille; i++) {
            if (tableau[plan_cible][i][0] == '\0') {
                int indexe = rand() % 22739 + 1;
                parcourire_liste(fil, indexe, mot);

                int autre_plan = (plan_cible == 0) ? 1 : 0;// condition ? valeur_si_vrai : valeur_si_faux;

                while (verification(tableau[autre_plan], mot, taille)) {
                    indexe = rand() % 22739 + 1;
                    parcourire_liste(fil, indexe, mot);
                }
                strcpy(tableau[plan_cible][i], mot);
            }
        }
    }

    // Fonction pour "échanger" (copier) la liste joueur dans la liste commun
    void change(char tableau[2][50][50]) {
        for (int i = 0; i < taille; i++) {
            strcpy(tableau[0][i], tableau[1][i]);
        }
    }

    int main(void) {
        int temps = 0,score=0;
        SetConsoleOutputCP(65001); // Définit l'encodage UTF-8

        // Configuration des paramètres de jeu
        do {
            printf("Combien de mots dans ta liste ? (entre 6 et 12) : ");
            scanf("%d", &taille);
            if(taille < 6 || taille > 12) {
                printf("Tu dois choisir un nombre entre 6 et 12 inclu");
            }
        } while (taille < 6 || taille > 12);
        do {
            printf("Combien de temps veux-tu jouer ? (entre 60 et 180 secondes) : ");
            scanf("%d", &temps);
            if(temps < 60 || temps > 180) {
                printf("Tu dois choisir un nombre entre 60 et 180 inclu");
            }
        }while ( temps < 60 || temps > 180);

        FILE *fil = fopen("../mots.txt", "r");
        if (fil == NULL) {
            printf("Erreur : Impossible d'ouvrir le fichier des mots.\n");
            return 1;
        }

        // Tableau 3D : [0] pour commun, [1] pour joueur
        char tableau[2][50][50] = {{{'\0'}}}; // Initialisation avec des chaînes vides

        int index = 0, position = 0;
        char mot[50], mot_commun[50],mot_choisis[50];

        srand(time(NULL));

        // Capturer le temps de début
        time_t start_time = time(NULL);
        time_t current_time;

        // Compléter les deux listes
        remplir_liste(tableau, 0, fil, taille,mot_commun); // Remplir la liste commun
        remplir_liste(tableau, 1, fil, taille,mot_commun); // Remplir la liste joueur

        // Jeu principal
        while (1) {
            // Vérifier le temps écoulé
            current_time = time(NULL);
            if (difftime(current_time, start_time) >= temps) {
                printf("\nTemps écoulé ! Fin de la partie.\n");
                break;
            }

            // Afficher les listes
            printf("Liste Joueur : ");
            for (int i = 0; i < taille; i++) {
                printf("%s ", tableau[1][i]);
            }
            printf("\nListe Commun : ");
            for (int i = 0; i < taille; i++) {
                printf("%s ", tableau[0][i]);
            }

            // Demander au joueur d'identifier le mot commun
            do {
                printf("\nQuel est le mot commun des deux listes ? ");
                scanf("%s", mot_choisis);

                if (difftime(current_time, start_time) >= temps) {
                    printf("\nTemps écoulé ! Fin de la partie.\n");
                    break;
                }
                if (strcmp(mot_choisis, mot_commun) != 0) {
                    printf("Erreur : Les mots ne correspondent pas. Essayez à nouveau.\n");
                }
                if(strcmp(mot_choisis, mot_commun) == 0 && difftime(current_time, start_time) >= temps) {
                    score++;
                }else {
                    printf("Ton point sera pas pris en compte car le temps était dépassé");
                }

            } while (strcmp(mot_choisis, mot_commun) != 0);

            // Copier la liste joueur dans la liste commun
            change(tableau);
            memset(tableau[1], '\0', sizeof(tableau[1]));
            remplir_liste(tableau, 1, fil, taille,mot_commun); // Remplir la liste joueur
        }
        printf("le score est de %d",score);
        fclose(fil);
        return 0;
    }
