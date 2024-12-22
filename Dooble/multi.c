#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
int joueur=0, taille=0;
int verification2(char chaine[0][0][50],char *mot,const int taille) {// ici une fonction pour vérifier si le mot est dans l'autre chaine
    int result=0;
    for (int i = 0; i < taille; i++) {
        for(int j=0;j<taille;j++) {
            result=strcmp(chaine[i][j], mot);// On utlise la fonction strcmp pour comparer les deux mot et si ceci renvoi 0 alors les mots sont les même
            if(result==0) {
                return 1;
            }
        }

    }
    return 0;
}
int verification(char chaine[0][50],char *mot,const int taille) {// ici une fonction pour vérifier si le mot est dans l'autre chaine
    int result=0;
    for (int i = 0; i < taille; i++) {
        result=strcmp(chaine[i], mot);// On utlise la fonction strcmp pour comparer les deux mot et si ceci renvoi 0 alors les mots sont les même
        if(result==0) {
            return 1;
        }
    }
    return 0;
}
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
void tableau_commun(char commun[0][50],FILE*fil,char *mot, const int taille) {
    for(int i=0;i<taille;i++){
        if(commun[i][0]=='\0'){
            int indexe=0;
            indexe=rand()%22739+1;
            parcourire_liste(fil,indexe,mot);
            strcpy(commun[i], mot);
        }
    }
}
void tableau_joueur(char tab[joueur][taille][50],char commun[0][50],FILE*fil,char *mot,const int taille, const int joueur) {
    for(int i=0;i<joueur;i++){
        if(i>0) {
            strcpy(tab[i][rand()%taille], tab[i-1][rand()%taille]);
        }
        for (int j = 0; j < taille; j++) {
            if(tab[i][j][0]=='\0'){
                int indexe=0;
                indexe=rand()%22739+1;
                parcourire_liste(fil,indexe,mot);
                if(verification2(tab,mot,taille)==0 && verification(commun,mot,taille)==0 ) {
                    strcpy(tab[i][j], mot);
                }
                else if (verification2(tab,mot,taille)!=0 && verification(commun,mot,taille)!=0) {
                    do {
                        indexe=rand()%22739+1;
                        parcourire_liste(fil,indexe,mot);
                    }while (verification2(tab,mot,taille)!=0 && verification(commun,mot,taille)!=0);
                    strcpy(tab[i][j], mot);
                }
            }
        }
    }
}
int main(void)
{
    srand(time(NULL));
    FILE*fil=fopen("../mots.txt","r");
    //Aymeric deja je t'aime mais faudra que tu change ici car moi les entré je les fais sur la console
    do {
        printf("combien de mot dans t'as liste entre 6 et 12");
        scanf("%d",&taille);
        printf("Combien de joueur?");
        scanf("%d",&joueur);
    }while(taille<6 || taille>12 || joueur<=0);

    char liste_joueur[joueur][taille][50],commun[taille][50], mot[50],stock[50];
    // On initialise les tableaux
    for (int i = 0; i < joueur; i++) {
        for (int j = 0; j < taille; j++) {
            liste_joueur[i][j][0] = '\0'; // Chaque mot devient une chaîne vide
        }
    }
    for (int i = 0; i < taille; i++) {
        commun[i][0]='\0';
    }

    //On définie l'index du mot commun
    parcourire_liste(fil,rand()%22739+1,mot);
    strcpy(stock,mot);
    printf("Commun %s\n",mot);

    //La je les mets dans chaque liste a un indice aléatoire
    strcpy(liste_joueur[0][rand()%taille], mot);
    strcpy(commun[rand()%taille], mot);

    tableau_commun(commun,fil,mot,taille);
    tableau_joueur(liste_joueur,commun,fil,mot,taille,joueur);
    
    printf("commun:");
    for (int i = 0; i < taille; i++) {
        printf("%s,",commun[i]);
    }
    for (int i = 0; i < joueur; i++) {
        printf("\njoueur %d:",i+1);
        for (int j = 0; j < taille; j++) {
            printf("%s,",liste_joueur[i][j]);
        }
    }
    fclose(fil);
    return 0;
}
