#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NB_MAINS 5
#define NB_FACES 6
#define NB_TIRAGES_MAX 12


/** Variable globale du nombre de tirages modifable en argument du programme*/
int NB_TIRAGES =  7;

typedef struct Joueur{
    char nom[50];
    int mains[NB_MAINS][NB_TIRAGES_MAX];
    int nextIndice;
    int scoreTotal;
}Joueur;

const char *combinaisons[] = {"Rien", "Paire", "Triple", "Carre", "Penta", "Hexa", "Septa", "Octa",
                               "Nona", "Deca", "Hendeca", "Dodeca"};

/** Déclaration des joueurs */
Joueur *joueur;
Joueur *ordinateur;


/**
 * estConsecutif() - Vérifie si tous les chiffres du tableau sont consécutifs
 * @tab: Tableau dans lequel rechercher la suite consécutif
 * Return:
 *  0: Le tableau n'est pas une suite de chiffres consécutifs
 *  1: Le tableau est une suite de chiffres consécutifs
 */
int estConsecutif(const int tab[NB_TIRAGES_MAX]) {

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

int* identifie(const int hand[NB_TIRAGES_MAX]) {

    /** On ignore l'indice zéro pour associé chaque indice à la valeur de la face du dé */
    int frequence[] = {0, 0, 0, 0, 0, 0, 0};
    static int ffrequence[NB_TIRAGES_MAX];
    int i;

    /** Récupération des fréquences */
    for (i = 0; i < NB_TIRAGES; i++) {
        frequence[hand[i]]++;
    }

    /** Récupération des fréquences de fréquence */
    for(i = 0;i < NB_TIRAGES + 1; i++) {
        ffrequence[i] = 0;
    }
    for(i = 1;i < NB_FACES + 1; i++) {
        ffrequence[frequence[i]]++;
    }
    return ffrequence;
}

/**
 * affiche() - Affiche la main d'un joueur
 * @name: Nom du joueur
 * @hand: Main du joueur
 */
void affiche(char *name, const int hand[NB_TIRAGES_MAX]) {
    int i;

    printf("\n%s: ", name);
    for(i=0;i<NB_TIRAGES;i++) {
        printf("%d ", hand[i]);
    }
    printf("( ");
    int* ffrequence = identifie(hand);
    for(i=2;i<NB_TIRAGES;i++){
        int nombre = ffrequence[i];
        if(nombre != 0) {
            printf("%d %s ", nombre, combinaisons[i-1]);
        }
    }
    printf(")");
}

int calculerPoints(const int* ffrequence) {
    int i;
    int total = 0;
    for(i=2; i<NB_TIRAGES; i++) {
        total += (int)(ffrequence[i]*pow(2, i));
    }
    return total;
}

int manche() {

    int i;
    for (i = 0; i < NB_TIRAGES; i++) {
        joueur->mains[joueur->nextIndice][i] = rand() % 6 + 1;
        ordinateur->mains[ordinateur->nextIndice][i] = rand() % 6 + 1;
    }

    affiche(ordinateur->nom, ordinateur->mains[joueur->nextIndice]);
    affiche(joueur->nom, joueur->mains[joueur->nextIndice]);

    /**  Calcul des points et détermination du gagnant */
    int pointsJoueur = calculerPoints(identifie(joueur->mains[joueur->nextIndice]));
    int pointsOrdinateur = calculerPoints(identifie(ordinateur->mains[joueur->nextIndice]));

    /** Incrémentation de l'indice des mains du joueur */
    joueur->nextIndice++;
    ordinateur->nextIndice++;

    /** Mise à jour du score total */
    joueur->scoreTotal += pointsJoueur;
    ordinateur->scoreTotal += pointsOrdinateur;

    printf("\n");

    if(pointsJoueur < pointsOrdinateur) {
        return -1;
    }else{
        if(pointsJoueur > pointsOrdinateur) {
            return 1;
        }else{
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
int main(int argc, char *argv[]) {

    int i;
    if(argc == 2) {
        NB_TIRAGES = atoi(argv[1]);
    }else{
        return EXIT_FAILURE;
    }

    /** Initialisation des joueurs  */
    joueur = malloc(sizeof (Joueur));
    ordinateur = malloc(sizeof (Joueur));
    joueur->nextIndice = 0;
    ordinateur->nextIndice = 0;
    joueur->scoreTotal = 0;
    ordinateur->scoreTotal = 0;
    strcpy(joueur->nom, "Joueur");
    strcpy(ordinateur->nom, "Ordinateur");

    /** Jeu */
    srand(time(NULL));
    for(i=0; i<NB_MAINS; i++) {
        manche();
    }

    printf("\nPoints %s: %d\n", ordinateur->nom, ordinateur->scoreTotal);
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
    return EXIT_SUCCESS;
}
