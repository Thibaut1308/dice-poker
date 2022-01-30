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
}Joueur;

typedef struct ListeFaces{
    int faces[NB_FACES+1];
    int indiceDernierElement;
}ListeFaces;

typedef struct Identification{
    int ffrequence;
    ListeFaces lfaces;
}Identification;


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

/**
 * identifie() - Fonction d'identification du type de main d'un joueur
 * @hand: Main du joueur à analyser
 * Return:
 *      Tableau des fréquences de fréquence d'apparition des chiffres sur les dés
 */
Identification* identifie(const int hand[NB_TIRAGES_MAX]) {

    /** On ignore l'indice zéro pour associé chaque indice à la valeur de la face du dé */
    int frequence[] = {0, 0, 0, 0, 0, 0, 0};
    static int ffrequence[NB_TIRAGES_MAX];
    Identification *id = NULL;
    id = malloc(NB_TIRAGES_MAX * (sizeof (Identification)));
    int i;

    /** Récupération des fréquences */
    for (i = 0; i < NB_TIRAGES; i++) {
        frequence[hand[i]]++;
    }

    /** Récupération des fréquences de fréquence */
    for(i = 0;i < NB_TIRAGES + 1; i++) {
        /** Initialisation des fréquences de fréquences*/
        //ffrequence[i] = 0;
        id[i].ffrequence = 0;
        id[i].lfaces.indiceDernierElement = 0;
    }

    for(i = 1;i < NB_FACES + 1; i++) {
        //ffrequence[frequence[i]]++;
        id[frequence[i]].ffrequence++;
        id[frequence[i]].lfaces.faces[id[frequence[i]].lfaces.indiceDernierElement] = i;
        id[frequence[i]].lfaces.indiceDernierElement++;
    }

    return id;
}

/**
 * affiche() - Affiche la main d'un joueur
 * @name: Nom du joueur
 * @hand: Main du joueur
 */
void affiche(char *name, const int hand[NB_TIRAGES_MAX]) {
    int i;
    Identification *id;

    printf("\n%s: ", name);
    for(i=0;i<NB_TIRAGES;i++) {
        printf("%d ", hand[i]);
    }
    printf("( ");
    //int* ffrequence = identifie(hand);
    id = identifie(hand);
    for(i=2;i<NB_TIRAGES;i++){
        //int nombre = ffrequence[i];
        int nombre = id[i].ffrequence;
        if(nombre != 0) {
            printf("%d %s ", nombre, combinaisons[i-1]);
        }
    }
    printf(")");
}

/**
 * calculerPoints() - Calcule les points d'une main
 * @ffrequence: Tableau de fréquence des fréquences d'apparition des chiffres sur les dés
 * Return:
 *      Nombre de points d'une main d'après le tableau retourner par la fonction identifier.
 */
int calculerPoints(const Identification* idJoueur, const Identification* idOrdinateur) {
    int i;
    int totalJoueur = 0;
    int totalOrdinateur = 0;
    for(i=2; i<NB_TIRAGES; i++) {
        totalJoueur += (int)(idJoueur[i].ffrequence*pow(2, i));
        totalOrdinateur += (int)(idOrdinateur[i].ffrequence*pow(2, i));
    }

    if(totalJoueur < totalOrdinateur) {
        printf("Manche remportee par l'ordinateur\n");
        return -1;
    }else{
        if(totalJoueur > totalOrdinateur) {
            printf("Manche remportee par le joueur\n");
            return 1;
        }else{
            printf("Manche nulle\n");
            return 0;
        }
    }
}

/**
 * manche() - Fonction exécutant une manche de dice poker.
 * Retourne:
 *      -1: Victoire de l'ordinateur
 *       0: Egalité
 *       1: Victoire du joueur
 */
int manche() {

    int i;
    for (i = 0; i < NB_TIRAGES; i++) {
        joueur->mains[joueur->nextIndice][i] = rand() % 6 + 1;
        ordinateur->mains[ordinateur->nextIndice][i] = rand() % 6 + 1;
    }

    affiche(ordinateur->nom, ordinateur->mains[joueur->nextIndice]);
    affiche(joueur->nom, joueur->mains[joueur->nextIndice]);

    /** Choix de relance des dés */
    int relance[NB_TIRAGES];
    char rep = ' ';
    while (rep != 'O' && rep != 'N' && rep != 'o' && rep != 'n') {
        printf("\nRelancer ? (O/N)\n");
        scanf("%c", &rep);
    }

    if (rep == 'O' || rep == 'o') {
        printf("\nQue voulez-vous faire ? (Entrez une serie de chiffre 0 ou 1 avec 1 les chiffres a rejouer)\n");
        for (i = 0; i < NB_TIRAGES; i++) {
            scanf("%d", &relance[i]);
        }

        /** Relance des dés choisis */
        for(i=0;i<NB_TIRAGES;i++) {
            if(relance[i] == 1) {
                joueur->mains[joueur->nextIndice][i] = rand() % 6 + 1;
            }
        }
        printf("\nNouvelle main:");
        /** Affichage de la nouvelle main */
        affiche(joueur->nom, joueur->mains[joueur->nextIndice]);
    }


    /**  Calcul des points et détermination du gagnant */
    int calculGagnant = calculerPoints(identifie(joueur->mains[joueur->nextIndice]), identifie(ordinateur->mains[joueur->nextIndice]));

    /** Incrémentation de l'indice des mains du joueur */
    joueur->nextIndice++;
    ordinateur->nextIndice++;

    printf("\n");

    return calculGagnant;
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
    strcpy(joueur->nom, "Joueur");
    strcpy(ordinateur->nom, "Ordinateur");

    int scoreTotal = 0;

    /** Jeu */
    srand(time(NULL));
    for(i=0; i<NB_MAINS; i++) {
        scoreTotal += manche();
    }

    printf("Points totaux: %d\n\n", scoreTotal);


    if(scoreTotal < 0) {
        printf("Victoire de l'ordinateur\n\n");
    }else{
        if(scoreTotal > 0) {
            printf("Vous avez gagne !\n\n");
        }else{
            printf("Egalite\n\n");
        }
    }
    return EXIT_SUCCESS;
}
