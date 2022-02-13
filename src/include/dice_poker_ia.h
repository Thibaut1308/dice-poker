#ifndef DICE_POKER_IA_H
#define DICE_POKER_IA_H

#include <stdio.h>
#include <stdlib.h>

/** Entêtes IA */
int *rejoue(int *hand, int hand_size);

/**
 * toutRejouer() - Rejoue toute une main d'ordinateur
 * @hand_size Taille de la main à rejouer
 * Return:
 *      retour: Tableau des dés à rejouer (1 de taille @hand_size)
 */
int *toutRejouer(int hand_size) {
    int *retour;
    retour = malloc(sizeof (int) * hand_size);
    int i;
    for(i=0; i<hand_size; i++) {
        retour[i] = 1;
    }
    return retour;
};

/**
 * rienRejouer() - Ne rejoue rien dans une main d'ordinateur
 * @hand_size Taille de la main à rien rejouer
 * Return:
 *      retour: Tableau des dés à rejouer (0 de taille @hand_size)
 */
int *rienRejouer(int hand_size) {
    int *retour;
    retour = malloc(sizeof (int) * hand_size);
    int i;
    for(i=0; i<hand_size; i++) {
        retour[i] = 0;
    }
    return retour;
};

/**
 * rejouerUnDeSurDeux() - Rejoue un dé sur deux dans une main d'ordinateur
 * @hand_size Taille de la main à rejouer
 * Return:
 *      retour: Tableau des dés à rejouer (0 et de 1 de taille @hand_size)
 */
int *rejouerUnDeSurDeux(int hand_size) {
    int *retour;
    retour = malloc(sizeof (int) * hand_size);
    int i;
    for(i=0; i<hand_size; i++) {
        retour[i] = i%2;
    }
    return retour;
};

int *rejouerAlea(int hand_size) {
    int *retour;
    retour = malloc(sizeof (int) * hand_size);
    int i;
    for(i=0; i<hand_size; i++) {
        retour[i] = rand() % 2;
    }
    return retour;
}

void afficherMainARejouer(int *hand, int hand_size) {
    printf("\n[ ");
    int i;
    for(i=0; i<hand_size; i++) {
        if(i==0) {
            printf("%d", hand[i]);
        }else{
            printf(", %d", hand[i]);
        }
    }
    printf(" ]\n");
}


#endif