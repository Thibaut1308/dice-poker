#ifndef DICE_POKER_IA_H
#define DICE_POKER_IA_H


#include "dice_poker_v2.h"
#include <stdio.h>
#include <stdlib.h>

int strategie;

int *rejouerIA(int *hand, int hand_size);

/** Enumérations */
typedef enum {
    RIEN_REJOUER = 0,
    TOUT_REJOUER = 1,
    REJOUER_UN_DE_SUR_DEUX = 2,
    REJOUER_ALEA = 3,
    REJOUER_IA = 4
} Strategie;


void choisirStrategie() {
    char rep = ' ';
    while (rep != '0' && rep != '1' && rep != '2' && rep != '3' && rep != '4') {
        printf("\nChoisir la strategie de l'ordinateur: \n");
        printf(" 0: Rien rejouer\n");
        printf(" 1: Tout rejouer\n");
        printf(" 2: Rejouer un de sur deux\n");
        printf(" 3: Rejouer Alea\n");
        printf(" 4: IA\n");
        scanf(" %c", &rep);
        strategie = (int)rep - (int)'0';
    }
}


/**
 * rejouerIA() - Rejoue les dés non compris dans des paires , triples ...
 * @hand_size Taille de la main à rejouer
 * Return:
 *      retour: Tableau des dés à rejouer de taille @hand_size
 */
int *rejouerIA(int *hand, int hand_size) {
    Identification* id;
    id = identifie(hand);
    int i;
    int j;
    int ffrequence;
    int facesPresentes[NB_FACES];
    int indiceFacesPresentes = 0;

    for(i=2;i<NB_TIRAGES;i++){
        ffrequence = id[i].ffrequence;
        if(ffrequence != 0){
            for(j=0; j<id[i].lfaces.indiceDernierElement; j++) {
                facesPresentes[indiceFacesPresentes] = id[i].lfaces.faces[j];
                indiceFacesPresentes++;
            }
        }
    }

    int *retour;
    retour = malloc(sizeof (int) * hand_size);

    // On set les dés à rejouer à 1
    for(i=0; i<hand_size; i++) {
        retour[i] = 1;
    }

    // On retire ceux dont les faces sont présentes dans facesPrésentes
    for(i=0; i<hand_size; i++) {
        for(j=0; j<NB_FACES; j++) {
            if(facesPresentes[j] == hand[i]) {
                retour[i] = 0;
            }
        }
    }

    return retour;
}

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

void afficherMainARejouer(int *desARejouer, int hand_size) {
    printf("\n[ ");
    int i;
    for(i=0; i<hand_size; i++) {
        if(i==0) {
            printf("%d", desARejouer[i]);
        }else{
            printf(", %d", desARejouer[i]);
        }
    }
    printf(" ]\n");
}



/** Méthode rejouer de l'IA */
int *rejoue(int *hand, int hand_size) {
    switch (strategie) {
        case RIEN_REJOUER:
            return rienRejouer(hand_size);
        case TOUT_REJOUER:
            return toutRejouer(hand_size);
        case REJOUER_UN_DE_SUR_DEUX:
            return rejouerUnDeSurDeux(hand_size);
        case REJOUER_ALEA:
            return rejouerAlea(hand_size);
        case REJOUER_IA:
            return rejouerIA(hand, hand_size);
        default:
            return (int *)EXIT_FAILURE;
    }
}

#endif