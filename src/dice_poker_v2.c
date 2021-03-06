#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "include/dice_poker_v2.h"
#include "include/declarations.h"
#include "include/dice_poker_ia.h"

/**
 * estConsecutif() - Vérifie si tous les chiffres du tableau sont consécutifs
 * @tab: Tableau dans lequel rechercher la suite consécutif
 * Return:
 *      0: Le tableau n'est pas une suite de chiffres consécutifs
 *      1: Le tableau est une suite de chiffres consécutifs de 1 à 5 (Five Straight)
 *      2: Le tableau est une suite de chiffres consécutifs de 2 à 6 (Six Straight)
 */
int estConsecutif(const int tab[NB_TIRAGES_MAX]) {

    int minimum = tab[0];
    int nb_consecutif = 5;
    int i;

    for(i=1;i<nb_consecutif;i++) {
        if(tab[i] < minimum) {
            minimum = tab[i];
        }
    }

    int fin = 0;
    int taille = 1;
    int precedentMin = minimum;

    while(taille != nb_consecutif && !fin) {
        for(i=0;i<nb_consecutif;i++) {
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
    if(taille == nb_consecutif) {
        if(minimum == 6) {
            return 2; // Six Straight
        }else{
            return 1; // Five Straight
        }
    }else{
        return 0;
    }
}

/**
 * identifie() - Fonction d'identification du type de main d'un joueur
 * @hand: Main du joueur à analyser
 * Return:
 *      Pointeur sur une structure identification créée
 */
Identification* identifie(const int hand[NB_TIRAGES_MAX]) {

    /** On ignore l'indice zéro pour associé chaque indice à la valeur de la face du dé */
    int frequence[] = {0, 0, 0, 0, 0, 0, 0};
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
        /** Obligé d'initialiser à 0 autrement ça fait n'importe quoi */
        id[i].ffrequence = 0;
        id[i].lfaces.indiceDernierElement = 0;
        for(int j=0; j<NB_FACES; j++) {
            id[i].lfaces.faces[j] = 0;
        }
    }

    for(i = 1;i < NB_FACES + 1; i++) {
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
    int rien = 1;

    printf("\n%s: ", name);
    for(i=0;i<NB_TIRAGES;i++) {
        printf("%d ", hand[i]);
    }
    printf("( ");
    id = identifie(hand);
    for(i=2;i<=NB_TIRAGES;i++){
        int nombre = id[i].ffrequence;
        if(nombre != 0) {
            rien = 0;
            if(i == NB_TIRAGES) {
                printf("%s ", "Full !" );
            }else{
                printf("%d %s ", nombre, combinaisons[i-1]);
            }
        }
    }

    // Cas d'un straight
    int straight = estConsecutif(hand);
    switch(straight) {
        case 1:
            rien = 0;
            printf("%s ", "Five Straight");
            break;
        case 2:
            rien = 0;
            printf("%s ", "Six Straight");
            break;
        default:
            break;
    }

    if(rien) {
        printf("%s ", "Rien");
    }
    printf(" )");
}

/**
 * calculerPoints() - Calcule les points d'une main
 * @idJoueur: structure d'indentification de la main du joueur
 * @idOrdinateur: structure d'identification de la main de l'ordinateur
 * Return:
 *      Calcule les points et renvoie le gagnant (nécessaire pour la comparaison des faces dans les égalités)
 *      -1: Victoire de l'ordinateur
 *       0: Egalité
 *       1: Victoire du joueur
 */
int calculerPoints(const Identification* idJoueur, const Identification* idOrdinateur) {
    int i;
    int totalJoueur = 0;
    int totalOrdinateur = 0;
    int straightOrdinateur;
    int straightJoueur;

    for(i=2; i<=NB_TIRAGES; i++) {
        totalJoueur += (int)(idJoueur[i].ffrequence*pow(2, i));
        totalOrdinateur += (int)(idOrdinateur[i].ffrequence*pow(2, i));
    }

    /** Choix Five Straight = 10 points et Six Straight = 11 points pour valoir plus qu'un triple mais moins qu'un house  */

    straightOrdinateur = 9 + estConsecutif(ordinateur->mains[ordinateur->nextIndiceMains]);
    straightJoueur = 9 + estConsecutif(joueur->mains[joueur->nextIndiceMains]);

    if(straightJoueur != 9) totalJoueur += straightJoueur;
    if(straightOrdinateur != 9) totalOrdinateur += straightOrdinateur;

    if(totalJoueur < totalOrdinateur) {
        return -1;
    }else{
        if(totalJoueur > totalOrdinateur) {
            return 1;
        }else{
            // Parcours dans le sens inverse pour prendre d'abord en compte les N-d'un coup les plus grands.
            for(i=NB_TIRAGES-1; i>=2; i--){
                // Si le joueur et l'ordinateur ont le même N-d'un coup.
                if(idJoueur[i].ffrequence != 0 && idOrdinateur[i].ffrequence != 0) {
                    // Addition de toutes les faces, le gagnant est la somme la plus grande.
                    int sommeFacesJoueur = 0;
                    int sommeFacesOrdinateur = 0;
                    int j = 0;
                    for(j=0; j<idJoueur[i].lfaces.indiceDernierElement; j++) {
                        sommeFacesJoueur += idJoueur[i].lfaces.faces[j];
                    }
                    for(j=0; j<idOrdinateur[i].lfaces.indiceDernierElement; j++) {
                        sommeFacesOrdinateur += idOrdinateur[i].lfaces.faces[j];
                    }
                    if(sommeFacesOrdinateur > sommeFacesJoueur) return -1;
                    if(sommeFacesJoueur > sommeFacesOrdinateur) return 1;
                }
            }
            return 0;
        }
    }
}

/**
 * manche() - Fonction exécutant une manche de dice poker.
 * Return:
 *      -1: Victoire de l'ordinateur
 *       0: Egalité
 *       1: Victoire du joueur
 */
int manche() {

    int i;
    for (i = 0; i < NB_TIRAGES; i++) {
        joueur->mains[joueur->nextIndiceMains][i] = rand() % 6 + 1;
        ordinateur->mains[ordinateur->nextIndiceMains][i] = rand() % 6 + 1;
    }

    affiche(ordinateur->nom, ordinateur->mains[joueur->nextIndiceMains]);
    affiche(joueur->nom, joueur->mains[joueur->nextIndiceMains]);


    /** Choix de relance des dés */
    int relance[NB_TIRAGES];
    char rep = ' ';
    while (rep != 'O' && rep != 'N' && rep != 'o' && rep != 'n') {
        printf("\nRelancer ? (O/N)\n");
        scanf(" %c", &rep);
    }

    if (rep == 'O' || rep == 'o') {
        printf("\nQue voulez-vous faire ? (Entrez une serie de chiffre 0 ou 1 avec 1 les chiffres a rejouer)\n");
        for (i = 0; i < NB_TIRAGES; i++) {
            scanf(" %d", &relance[i]);
        }

        /** Relance des dés choisis */
        for(i=0;i<NB_TIRAGES;i++) {
            if(relance[i] == 1) {
                joueur->mains[joueur->nextIndiceMains][i] = rand() % 6 + 1;
            }
        }
        printf("\nNouvelle main:");

        /** Affichage de la nouvelle main */
        affiche(joueur->nom, joueur->mains[joueur->nextIndiceMains]);
    }



    /**  Calcul des points et détermination du gagnant */
    int calculGagnant = calculerPoints(identifie(joueur->mains[joueur->nextIndiceMains]), identifie(ordinateur->mains[joueur->nextIndiceMains]));

    if(calculGagnant == 1 || calculGagnant == 0) {
        printf("\n\nL'ordinateur rejoue les des suivants: ");
        int *desARejouer = rejoue(ordinateur->mains[ordinateur->nextIndiceMains], NB_TIRAGES);
        afficherMainARejouer(desARejouer, NB_TIRAGES);

        /** Relance des dés choisis */
        for(i=0;i<NB_TIRAGES;i++) {
            if(desARejouer[i] == 1) {
                ordinateur->mains[ordinateur->nextIndiceMains][i] = rand() % 6 + 1;
            }
        }
        printf("\nNouvelle main:");

        /** Affichage de la nouvelle main */
        affiche(ordinateur->nom, ordinateur->mains[ordinateur->nextIndiceMains]);

        /**  Recalcul des points et détermination du gagnant */
        calculGagnant = calculerPoints(identifie(joueur->mains[joueur->nextIndiceMains]), identifie(ordinateur->mains[joueur->nextIndiceMains]));
    }

    switch (calculGagnant) {
        case 1:
            printf("\n\nVous remportez la manche !\n");
            break;
        case -1:
            printf("\n\nL'ordinateur remporte la manche.\n");
            break;
        default:
            printf("\n\nEgalite\n");
            break;
    }


    /** Incrémentation de l'indice des mains du joueur */
    joueur->nextIndiceMains++;
    ordinateur->nextIndiceMains++;

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
        printf("Combien de des jouer par main ?\n");
        scanf(" %d", &NB_TIRAGES);
    }

    if(NB_TIRAGES <= 0) {
        return EXIT_FAILURE;
    }

    /** Initialisation des joueurs  */
    joueur = malloc(sizeof (Joueur));
    ordinateur = malloc(sizeof (Joueur));
    joueur->nextIndiceMains = 0;
    ordinateur->nextIndiceMains = 0;
    strcpy(joueur->nom, "Joueur");
    strcpy(ordinateur->nom, "Ordinateur");

    int scoreTotal = 0;

    /** Demande au joueur de choisir la stratégie utilisée par l'ordinateur */
    choisirStrategie();

    /** Jeu */
    srand(time(NULL));
    for(i=0; i<NB_MAINS; i++) {
        printf("===============\n");
        printf("===============\n");
        printf("===MANCHE %d===\n", i+1);
        printf("===============\n");
        printf("===============\n");

        scoreTotal += manche();
    }

    printf("=================\n");
    printf("=================\n");
    printf("====RESULTATS====\n");
    printf("=================\n");
    printf("=================\n");
    printf("\nPoints totaux: %d\n\n", scoreTotal);


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
