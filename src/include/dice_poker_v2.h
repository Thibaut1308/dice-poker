#ifndef DICE_POKER_V2_H
#define DICE_POKER_V2_H

/** Déclaration des constantes **/
#define NB_MAINS 5
#define NB_FACES 6
#define NB_TIRAGES_MAX 12

/** Déclaration des structures **/
typedef struct Joueur{
    char nom[50];
    int mains[NB_MAINS][NB_TIRAGES_MAX];
    int nextIndice;
}Joueur;

typedef struct ListeFaces{
    int faces[NB_FACES+1];
    int indiceDernierElement;
}ListeFaces;

typedef struct Identification{
    int ffrequence;
    ListeFaces lfaces;
}Identification;


/** Déclaration des entêtes des fonctions **/
int estConsecutif(const int tab[NB_TIRAGES_MAX]);
Identification* identifie(const int hand[NB_TIRAGES_MAX]);
void affiche(char *name, const int hand[NB_TIRAGES_MAX]);
int calculerPoints(const Identification* idJoueur, const Identification* idOrdinateur);
int manche();

#endif