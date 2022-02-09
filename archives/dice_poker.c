#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_TIRAGES 5
#define NB_FACES 6
#define NB_MAINS 5

typedef struct Joueur{
    char nom[50];
    int mains[NB_MAINS][NB_TIRAGES];
    int nextIndiceMains;
    int scoreTotal;
}Joueur;

char *combinaisons[] = {"Rien", "Paire", "Deux paires", "Trois d'un coup", "Straight", "House",
                               "Quatre d'un coup", "Full"};

/** Déclaration des joueurs */
Joueur *joueur;
Joueur *ordinateur;

/**
 * contient() - Vérifie la présence d'un nombre dans un tableau
 * @num: Nombre à vérifier
 * @tab: Tableau dans lequel rechercher @num
 * Return:
 *  0: Le nombre n'a pas été trouvé
 *  1: Le nombre a été trouvé
 */
int contient(int num, const int tab[NB_FACES]) {

    int i;
    for (i = 1; i < NB_FACES + 1; i++) {
        if (tab[i] == num) {
            return 1;
        }
    }
    return 0;
}

/**
 * verifierNupletIdentiques() - Compte le nombre de n-uplet dans un tableau
 * @nuplet: N-uplet à compter
 * @tab: Tableau dans lequel compter les @nuplet
 * Return:
 *  res: Nombre de @nuplet trouvé dans @tab
 */
int verifierNupletIdentiques(int nuplet, const int tab[NB_FACES]) {

    int i;
    int res = 0;
    for (i = 1; i < NB_FACES + 1; i++) {
        if (tab[i] == nuplet) {
            res++;
        }
    }
    return res;
}

/**
 * estConsecutif() - Vérifie si tous les chiffres du tableau sont consécutifs
 * @tab: Tableau dans lequel rechercher la suite consécutif
 * Return:
 *  0: Le tableau n'est pas une suite de chiffres consécutifs
 *  1: Le tableau est une suite de chiffres consécutifs
 */
int estConsecutif(const int tab[NB_TIRAGES]) {

    int minimum = tab[0];
    int i;

    for(i=1;i<NB_TIRAGES;i++) {
        if(tab[i] < minimum) {
            minimum = tab[i];
        }
    }

    int fin = 0;
    int taille = 1;
    int precedentMin = minimum;

    while(taille != NB_TIRAGES && !fin) {
        for(i=0;i<NB_TIRAGES;i++) {
            if(tab[i] == minimum+1) {
                minimum++;
                taille++;
            }
        }
        if(minimum == precedentMin) {
            fin = 1;
        }else{
            precedentMin = minimum;
        }
    }
    if(taille == NB_TIRAGES) {
        return 1;
    }else{
        return 0;
    }
}

int identifie(const int hand[NB_TIRAGES]) {

    /** On ignore l'indice zéro pour associé chaque indice à la valeur de la face du dé */
    int frequence[] = {0, 0, 0, 0, 0, 0, 0};
    int i;
    int nb_frequence_zero = 0;

    /** Récupération des fréquences */
    for (i = 0; i < NB_TIRAGES; i++) {
        frequence[hand[i]]++;
    }

    /** Cinq fois le même chiffre */
    if (contient(5, frequence)) {
        return 7;
    }

    /** Quatre fois le même chiffre le dernier est forcément différent */
    if (contient(4, frequence)) {
        return 6;
    }

    /** Trois fois le même chiffre */
    if (contient(3, frequence)) {
        /** Si deux derniers identiques "House" sinon "Trois d'un coup" */
        if (contient(2, frequence)) {
            return 5;
        } else {
            return 3;
        }
    }

    /** Vérification deux paires ou paire simple */
    int nombrePairesIdentiques = verifierNupletIdentiques(2, frequence);
    if (nombrePairesIdentiques == 2) {
        return 2;
    } else {
        if (nombrePairesIdentiques == 1)
            return 1;
    }

    /** Cas d'un straight */
    if (estConsecutif(hand)) {
        return 4;
    }else{
        /** Cas d'une main avec tous les chiffres différents non consécutifs */
        return 0;
    }
}

/**
 * affiche() - Affiche la main d'un joueur
 * @name: Nom du joueur
 * @hand: Main du joueur
 */
void affiche(char *name, int hand[NB_TIRAGES]) {
    printf("%s: %d %d %d %d %d (%s)\n", name, hand[0], hand[1], hand[2], hand[3], hand[4], combinaisons[identifie(hand)]);
}

int manche() {

    int i;
    for (i = 0; i < NB_TIRAGES; i++) {
        joueur->mains[joueur->nextIndiceMains][i] = rand() % 6 + 1;
        ordinateur->mains[ordinateur->nextIndiceMains][i] = rand() % 6 + 1;
    }

    affiche(ordinateur->nom, ordinateur->mains[joueur->nextIndiceMains]);
    affiche(joueur->nom, joueur->mains[joueur->nextIndiceMains]);

    /**  Calcul des points et détermination du gagnant */
    int pointsJoueur = identifie(joueur->mains[joueur->nextIndiceMains]);
    int pointsOrdinateur = identifie(ordinateur->mains[joueur->nextIndiceMains]);

    /** Incrémentation de l'indice des mains du joueur */
    joueur->nextIndiceMains++;
    ordinateur->nextIndiceMains++;

    /** Mise à jour du score total */
    joueur->scoreTotal += pointsJoueur;
    ordinateur->scoreTotal += pointsOrdinateur;

    if(pointsJoueur < pointsOrdinateur) {
        printf("Victoire de l'ordinateur\n\n");
        return -1;
    }else{
        if(pointsJoueur > pointsOrdinateur) {
            printf("Vous avez gagne !\n\n");
            return 1;
        }else{
            printf("Egalite\n\n");
            return 0;
        }
    }
}

/**
 * main() - Fonction principale démarrant le jeu
 * Return:
 *  0: Le jeu s'est bien déroulé
 *  1: Une erreur s'est produite
 *
 *  Initialise les joueurs, joue les manches et affiche les résultats.
 */
int main() {

    int i;

    /** Initialisation des joueurs  */
    joueur = malloc(sizeof (Joueur));
    ordinateur = malloc(sizeof (Joueur));
    joueur->nextIndiceMains = 0;
    ordinateur->nextIndiceMains = 0;
    joueur->scoreTotal = 0;
    ordinateur->scoreTotal = 0;
    strcpy(joueur->nom, "Joueur");
    strcpy(ordinateur->nom, "Ordinateur");

    /** Jeu */
    srand(time(NULL));
    for(i=0; i<NB_MAINS; i++) {
        manche();
    }

    printf("Points %s: %d\n", ordinateur->nom, ordinateur->scoreTotal);
    printf("Points %s: %d\n", joueur->nom, joueur->scoreTotal);


    if(joueur->scoreTotal < ordinateur->scoreTotal) {
        printf("Victoire de l'ordinateur\n\n");
    }else{
        if(joueur->scoreTotal > ordinateur->scoreTotal) {
            printf("Vous avez gagne !\n\n");
        }else{
            printf("Egalite\n\n");
        }
    }

    return 0;
}
