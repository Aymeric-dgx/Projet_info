#include <stdio.h>

//Passer une valeur d'une fonction au main

int ask6(char *i_or_p, int *nb) {
	char char1;
	int n;
	do
		{printf("Voulez vous :\nLa somme des entiers paires [P]\nLa somme des entiers impaires [I]\nQuitter[Q]\n");
		scanf("%s", &char1);
		getchar();
	}while(char1 != 'Q' && char1 != 'P' && char1 != 'I');

	if (char1 == 'Q'){return 0;}

	do{
		printf("donner n nombre limite entre 2 et 100 exclu, ]2,100[\n");
		scanf("%d", &n);
		getchar();
	}while(n<2 || n>100);
	*i_or_p = char1;
	*nb = n;
	return 1;

}

int main() {
	char i_or_p;
	int nb=0, i=0, j=0, res=0;
	if (ask6(&i_or_p , &nb)==0){return 0;};
	if (i_or_p == 'P'){i=2;}
	else {i=1;}
	printf("%d",i);
	for (j=i; j<=nb; j+=2) {
		res+=j;
	}
	printf("La somme des entiers %c de %d, vaut %d", i_or_p, nb ,res);
	return 0;
}
