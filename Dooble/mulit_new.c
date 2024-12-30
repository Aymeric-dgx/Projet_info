#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int joueur = 0, taille = 0;

int verification(char chaine[0][0][50], char *mot, int stop) {
    for (int i = 0; i < stop; i++) {
        for (int j = 0; j < taille; j++) {
            if (strcmp(chaine[i][j], mot) == 0) {
                return 1; // Le mot existe déjà
            }
        }
    }
    return 0; // Le mot est unique
}

void parcourire_liste(FILE *fil, int in, char *mot) {
    rewind(fil);
    char ligne[50];
    int compteur = 0;

    while (fgets(ligne, sizeof(ligne), fil) != NULL) {
        compteur++;
        if (compteur == in) {
            ligne[strcspn(ligne, "\n")] = '\0'; // Retirer le '\n'
            strcpy(mot, ligne);
            return;
        }
    }
}

// Fonction pour remplir la liste des joueurs
void remplir_liste(char tableau[joueur + 1][taille][50], FILE *fil, char stock[joueur][50]) {
    char mot[50];

    for (int i = 0; i < joueur + 1; i++) {
        if (i >= 1) { // Ajouter un mot en commun entre la liste précédente et la nouvelle
            int position = 0;
            if (i >= 2) {
                do {
                    position = rand() % taille;
                } while (strcmp(tableau[i - 1][position], stock[i - 2]) == 0); // Éviter de répéter le précédent mot commun
            } else {
                position = rand() % taille;
            }

            strcpy(stock[i - 1], tableau[i - 1][position]); // Stocker le mot commun
            int commun_index = rand() % taille; // Insérer le mot commun à une position aléatoire
            strcpy(tableau[i][commun_index], tableau[i - 1][position]);
        }

        // Compléter le reste de la liste
        for (int j = 0; j < taille; j++) {
            if (tableau[i][j][0] == '\0') { // Remplir les cases vides uniquement
                int indexe = 0;
                do {
                    indexe = rand() % 22739 + 1;
                    parcourire_liste(fil, indexe, mot);
                } while (verification(tableau, mot, i)); // Vérifier que le mot est unique
                strcpy(tableau[i][j], mot);
            }
        }
    }
}


void succes(int gagnant,char tab[joueur+1][taille][50],FILE*fil,char stock[joueur][50]) {
    char mot[50];
    int position=0;
    //La on échange les most de sa liste avec la liste commun
    for (int i = 0; i < taille; i++) {
        strcpy(tab[0][i], tab[gagnant][i]);
    }
    for (int j = 0; j < taille; j++) {
        tab[gagnant][j][0] = '\0';
    }

    if (joueur==gagnant) {
        position = rand() % taille;
    }else {
        do {
            position = rand() % taille;
        } while (strcmp(tab[gagnant-1][position], stock[gagnant+1]) == 0); // On regarde si le mot choisis n'est pas déja en commun a la liste de joueur suivant
    }

    strcpy(stock[gagnant - 1], tab[gagnant- 1][position]); // Stocker le nouveau mot commun
    int commun_index = rand() % taille; // Insérer le mot commun à une position aléatoire
    strcpy(tab[gagnant][commun_index], tab[gagnant - 1][position]);

    // Compléter le reste de la liste
    for (int j = 0; j < taille; j++) {
        if (tab[gagnant][j][0] == '\0') { // Remplir les cases vides uniquement
            int indexe = 0;
            do {
                indexe = rand() % 22739 + 1;
                parcourire_liste(fil, indexe, mot);
            } while (verification(tab, mot, joueur)); // Vérifier que le mot est unique
            strcpy(tab[gagnant][j], mot);
        }
    }

}
int main(void) {
    srand(time(NULL));
    FILE *fil = fopen("../mots.txt", "r");

    // Entrées utilisateur
    do {
        printf("Combien de mots dans la liste (entre 6 et 12) ? ");
        scanf("%d", &taille);
        printf("Combien de joueurs ? ");
        scanf("%d", &joueur);
    } while (taille < 6 || taille > 12 || joueur < 0);

    char liste_joueur[joueur + 1][taille][50], stock[joueur][50];

    // Initialiser les tableaux à '\0'
    for (int i = 0; i < joueur + 1; i++) {
        for (int j = 0; j < taille; j++) {
            liste_joueur[i][j][0] = '\0';
        }
    }

    for (int i = 0; i < joueur; i++) {
        stock[i][0] = '\0';
    }

    // Remplir les listes des joueurs
    remplir_liste(liste_joueur, fil, stock);

    // Afficher les listes
    printf("\nListe commune :");
    for (int j = 0; j < taille; j++) {
        printf("%s ", liste_joueur[0][j]);
    }

    for (int i = 1; i < joueur + 1; i++) {
        printf("\nJoueur %d :", i);
        for (int j = 0; j < taille; j++) {
            printf("%s ", liste_joueur[i][j]);
        }
    }

    // Afficher les mots communs
    for (int i = 0; i < joueur; i++) {
        printf("Mot commun pour joueur %d : %s\n", i + 1, stock[i]);
    }

    int gagnant=2;
    succes(gagnant,liste_joueur,fil,stock);

    printf("\nListe commune :");
    for (int j = 0; j < taille; j++) {
        printf("%s ", liste_joueur[0][j]);
    }

    for (int i = 1; i < joueur + 1; i++) {
        printf("\nJoueur %d :", i);
        for (int j = 0; j < taille; j++) {
            printf("%s ", liste_joueur[i][j]);
        }
    }

    // Afficher les mots communs
    for (int i = 0; i < joueur; i++) {
        printf("Mot commun pour joueur %d : %s\n", i + 1, stock[i]);
    }
    fclose(fil);
    return 0;
}
