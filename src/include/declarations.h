
#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "dice_poker_v2.h"

/** Déclaration des joueurs */
Joueur *joueur;
Joueur *ordinateur;

/** Déclaration des combinaisons possible pour les multiples */
const char *combinaisons[] = {"Rien", "Paire", "Triple", "Carre", "Penta", "Hexa", "Septa", "Octa",
                              "Nona", "Deca", "Hendeca", "Dodeca"};
int NB_TIRAGES = 5;

#endif
