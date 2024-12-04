/* Pour les différentes règles, voila plusieurs idées :
    - Nom joueur :  - Faire un petit rectangle
                    - Si on clique commence l'enregistrement du texte + l'affiche
                    - Pour stopper, faire un bouton ou si on clique dessus ça stoppe l'enregistrement de texte
                    - Pour mode multi, faire en sorte que chaque joueur entre chacun son tour leur nom de cette manière :
                        - 1er joueur entre son nom, puis le valide
                        - Nom ajouté dans un petit tableau à coté
                        - Recommencer avec 2eme

    - Temps :       - Faire un rectangle pour faire une barre de progression
                    - Cliquer dans cette barre de progression
                    - Comparé les coordonnées du clic avec le debut de la barre (voir si proche ou eloigné)
                    - Calculer le temps à donner en fonction de sa disatnce avec le début de la barre
                    - Pour que ce soit plus visuel, colorier tout ce qui est à gauche du point cliqué

    - Mode de jeu : - Faire 2 carré de "sélection" avec écrit à coté le mode de jeu
                    - Quand on clique dans un des 2 carré, il se colorie pour qu on voit quel mode est choisie
                    - Evidement stocker dans une variable (avec 1 et 2 par exemple) pour savoir ensuite quel mode lancer
                    - Attention aussi a ce que si on clique sur carré 1 puis sur carré 2, "déselctionner" le carré 1

    - Nb de mots :  - A peu près identique à la selection du temps
