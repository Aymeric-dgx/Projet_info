#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Notes Aymeric, documenter dans annexe SDL :
rewind(fill)
fgets(ligne, sizeof(ligne), fil)
strcspm(ligne, "\n")
fopens("...", "r")
fclose()
La "structure" FILE
time_t
*/

int taille = 0;

void parcourire_liste(FILE*fil,int in, char*mot) {
    rewind(fil);
    char ligne[50]; // Buffer pour lire chaque ligne
    int compteur=0;
    while (fgets(ligne, sizeof(ligne), fil) != NULL && compteur!=in) {
        compteur++;
        if(compteur==in) {
            ligne[strcspn(ligne, "\n")] = '\0';// on recupère le mot et on enleve le \n
            strcpy(mot, ligne);// on met le mot qui etait stockée dans le tableau ligne dans le tableau mot
            return;
        }
    }
}
int verification(char chaine[taille][50],char *mot) {// ici une fonction pour vérifier si le mot est dans l'autre chaine
    int result=0;
    for (int i = 0; i < taille; i++) {
        result=strcmp(chaine[i], mot);// On utlise la fonction strcmp pour comparer les deux mot et si ceci renvoi 0 alors les mots sont les même
        if(result==0) {
            return 1;
        }
    }
    return 0;
}
// Ici la technique est déjà de vérifié si on peut ecrire dans l'indice du tableau avant de mettre le mot a l'indice i
//Ensuite on genère un indice pour le mot dans le fichier txt et on vérifie si il est deja dans l'autre liste
// si il y est pas alors on écrit sinon on fais une boucle pour re genérer un nombre et re faire la méthode et ceci pour les deux fonction
void tableau_joueur1(char joueur1[taille][50],char commun[taille][50],FILE*fil,char *mot,int taille) {
    for(int i=0;i<taille;i++){
        if(joueur1[i][0]=='\0'){
            int indexe=0;
            indexe=rand()%22739+1;
            parcourire_liste(fil,indexe,mot);
            if(verification(commun,mot)==0) {
                strcpy(joueur1[i], mot);
            }
            else if (verification(commun,mot)!=0) {
                do {
                    indexe=rand()%22739+1;
                    parcourire_liste(fil,indexe,mot);
                }while (verification(commun,mot)!=0);
                strcpy(joueur1[i], mot);
            }
        }
    }
}
void tableau_commun(char joueur1[taille][50],char commun[taille][50],FILE*fil,char *mot,int taille) {
    for(int i=0;i<taille;i++){
        if(commun[i][0]=='\0'){
            int indexe=0;
            indexe=rand()%22739+1;
            parcourire_liste(fil,indexe,mot);
            if(verification(joueur1,mot)==0) {
                strcpy(commun[i], mot);
            }
            else if (verification(joueur1,mot)!=0) {
                do {
                    indexe=rand()%22739+1;
                    parcourire_liste(fil,indexe,mot);
                }while (verification(joueur1,mot)!=0);
                strcpy(commun[i], mot);
            }
        }
    }
}

int main(void)
{
    int temps=0;
    //Aymeric deja je t'aime mais faudra que tu change ici car moi les entré je les fais sur la console
    do {
        printf("combien de mot dans t'as liste entre 6 et 12");
        scanf("%d",&taille);
        printf("Combien de temps veut tu jouer entre 60 et 180 secondes");
        scanf("%d",&temps);
    }while(taille<6 || taille>12 ||temps <60 || temps>180 );


    FILE*fil=fopen("../mots.txt","r");
    // des listes de taille definie grace a l'utilisateur mots avec 50 caractères maximum sur chaque mot
    char joueur1[taille][50];
    char commun[taille][50];

    int index=0,position=0;
    char mot[50],mot_commun[50],stock[50];
    //Ici on réinitialise les deux listes pour éviter de garder les anciens mots
    for (int i = 0; i < taille; i++) {
        commun[i][0] = '\0'; // On met le premier caractère de chaque chaîne à '\0' pour indiquer qu'elle est vide
        joueur1[i][0]= '\0';
    }
    srand(time(NULL));
    // Capturer le temps de début
    time_t start_time = time(NULL);
    time_t current_time;

    while(1){
        // Vérifier le temps écoulé
        current_time = time(NULL);
        if (difftime(current_time, start_time) >= temps) {
            printf("\nTemps écoulé ! Fin de la partie.\n");
            break;
        }
        //On définie l'index du mot commun
        index=rand()%22739+1;
        parcourire_liste(fil,index,mot);
        strcpy(stock,mot);
        printf("Commun %s\n",mot);// Ici aymeric je l'affiche pour les teste mais enleve le a la fin

        //La je les mets dans chaque liste a un indice aléatoire
        position=rand()%taille;
        strcpy(joueur1[position], mot);
        position=rand()%taille;
        strcpy(commun[position], mot);

        // je complete le tableau
        tableau_joueur1(joueur1,commun,fil,mot,taille);
        tableau_commun(joueur1,commun,fil,mot,taille);
        //Affiche les listes
        printf("Joueur1\n");
        for(int j=0;j<taille;j++) {
            printf("%s ",joueur1[j]);
        }
        printf("\ncommun\n");
        for(int y=0;y<taille;y++) {
            printf("%s ",commun[y]);
        }
        do {
            printf("\nQuelle est le mot commun des deux listes ?");
            scanf("%s",mot_commun);
            if (strcmp(mot_commun, stock) != 0) {  // Comparer le mot donné avec celui attendu
                printf("Erreur : Les mots ne correspondent pas. Essayez à nouveau.\n");
            }
            if (difftime(current_time, start_time) >= temps) {
                printf("\nTemps écoulé ! Fin de la partie.\n");
                break;
            }
        }while (strcmp(mot_commun,stock)!=0 );

    }
    fclose(fil);
    return 0;
}
