#ifndef BIBLIO_H_INCLUDED
#define BIBLIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <conio.h>

/*structure commune à divers structures*/
typedef struct commun
{
    /*Coordonée corps/objet*/
    int x, y ;
    /*Vitesse*/
    int dx,dy ;
     /*Points de vie*/
    int vie ;
    /*Couleur spécifique à l'objet en question*/
    int c ;
    /*image de type .bmp corresspondant à l'objet/corps*/
    BITMAP**image;
} t_commun;

///DEFINITION DES COULEURS
///décors : makecol(255,255,255) blanc
///ennemis et tirs ennemis : makecol(255,0,0) rouge
///vie : makecol(0,255,0) vert
///joueur et tir jouer : makecol(0,0,255) bleu

typedef struct explosion
{
    t_commun base ;
    long int time ;
    struct explosion* suivant ;
} t_explo;

typedef struct ensemble_explosions
{
    t_explo* premier ;
    t_explo* dernier ;
} t_explosions ;

typedef struct pod
{
    t_commun base;
    /*cas où le pod sert de bélier*/
    int force;
    /*temps avant péremption*/
    long int temps;
} t_pod;

typedef struct munition
{
    t_commun base;
    t_explosions* explo ;
    /*liste chaînée*/
    struct munition*suivant;
} t_munition;

typedef struct collection
{
    t_munition* premier;
    t_munition* dernier;
} t_collection;

typedef struct ennemi
{
    t_commun base;
    t_collection* ancre;
    t_explosions* explo ;
    long int dernier_mouvement, nouveau_mouvement;
    long int tir, nouveau_tir ;
    int affichage ;
    /*liste chaînée*/
    struct ennemi*suivant;
} t_ennemi;

typedef struct ennemis
{
    t_ennemi * premier_fixe ;
    t_ennemi * dernier_fixe ;
    t_ennemi * premier_mouvant ;
    t_ennemi * dernier_mouvant ;
    t_ennemi * premier_boss ;
    t_ennemi * dernier_boss ;
} t_ennemis ;

typedef struct objet
{
    /*type d'objet*/
    t_commun base;
    /*liste chaînée*/
    struct objet*suivant;
} t_objet;

typedef struct objets
{
    t_objet* premier ;
    t_objet* dernier ;
} t_objets;

typedef struct perso
{
    t_commun base;
    t_explosions* explo ;
    /*Score perso pour les high score et passer les niveaux*/
    int score;
    /*Nombre de munitions pas infini doit attendre avant de recharcher*/
    int munition_reserve;
     /*coordonnées checkpoint*/
    int check[3];
    /*Nombre de vie avant game over (initialement 3 vies*/
    int nb_de_vie;
    /*pod de lancement a gagner*/
    t_pod*pod;
    /*Prendre le temps au moment où on appuie et le comparer au temps actuel*/
    long int chargement;
    t_collection* ancre;
    t_ennemis* opposants;
    t_objets* objet ;
} t_perso;

typedef struct scroller
{
    /*Position de la source*/
    int x,y;
    /*vitesse de déplacement*/
    int dx,dy;
    char * nom ;
    char type ;
    int taillex, tailley ;
    int fin ;
    /*image de fond*/
    BITMAP*image;
} t_scroller;


void probleme_de_memoire();

///Menu
void AffichageNouvPart(BITMAP * buffer, int tabchoix[7]);
void AffichageChargerSauv(BITMAP * buffer, int tabchoix[7]);
void AffichageQuitter(BITMAP * buffer, int tabchoix[7], int * sortie);
void AffichageNiveau(BITMAP * buffer, int tabchoix[7], int * niveau, BITMAP * fond[3]);
void AffichageOptions(BITMAP * buffer, int tabchoix[7], int * charge);
void AffichageNbJoueur(BITMAP * buffer, int tabchoix[7], int * joueur);
void AffichageSauvegarde(BITMAP * buffer, int tabchoix[7], int * charge, int chargement[3][2]);
void MiseAZeroMenu(int tabchoix[7], int *joueur, int * niveau, int * charge,int chargement[3][2]);

///Jeu
void jeu( int n_perso, char niveau[40], int charge );

///Gestion listes
/*tir*/
void tirAjout(t_collection*collec, t_commun * tireur, int puissance, t_explosions* feu, int dx, int dy, int i, char image[30] );
void SuppressionTir(t_munition * suppression, t_collection * ancre);
void Ajout_explosion ( t_explosions* ancre, int dx, int dy, t_commun* base );
void suppression_explosion( t_explosions* ancre, t_explo* suppression );
void Ajout_objet( t_objets* bonus, int couleur, char image[20], int x, int y, int vie, int dx, int dy ) ;

/*ennemi*/
void Ajout_ennemi(t_ennemi** premier, t_ennemi** dernier, t_collection* missiles,t_explosions* feu, int x, int y, int vie, int dy, char* image, int nb_image ) ;

///Sauvegarde
void choix_type( t_ennemis* ennemis_collec, t_objets* bonus, t_collection* missiles, t_explosions* feu, t_scroller* tabAttente, t_scroller* fond, t_perso* acteur, int n_perso, int k ) ;
t_scroller * chargement_niveau( char niveau[30], int* taille ) ;

///Initialisation
void initialisation( t_scroller** fond, t_perso** acteur, int n_perso, t_collection** missiles, t_scroller** collision, t_ennemis ** ennemis_collec, int nombre_de_perso[2], t_explosions** feu, t_objets** bonus, t_scroller* tabAttente, int* k, int taille, char * niveau );
void initialisation_des_collections( t_collection* missiles, t_ennemis* ennemis_collec, t_explosions*feu, t_objets* bonus ) ;

///Chargement
void chargement_perso(t_perso* acteur, t_collection* missiles, int nombre_vie, t_explosions* feu, int k, t_objets* objet, int* nb_de_perso, int i, int score, char * niveau, t_ennemis*mechant ) ;
int chargement_ennemi(t_ennemi*mechant, t_collection* missiles, t_explosions* feu, int x, int y, int vie, int dy, char image[30], int nb_image ) ;
void chargement_fond(t_scroller* fond, int taille_x, int taille_y, int dx, int dy, int fin, char* nom ) ;

///Action
void action_ennemi( t_ennemis* ancre, BITMAP* buffer, t_scroller* fond, BITMAP * collision, t_perso* acteur, int n_perso[2] );
void action_joueur( t_perso* acteur, BITMAP* buffer, int nombre_de_perso[2], BITMAP * collision, t_scroller * decors, t_ennemis* ancre, BITMAP* objet, int k) ;
void action_explosions( t_explosions* feu, BITMAP* buffer, t_scroller* decors ) ;
void action_objet( t_objets* ancre, BITMAP* buffer, BITMAP* bonus , t_scroller* decors ) ;

///Déplacement
void deplacement_ennemi_mouvant( t_ennemi* acteur, BITMAP * collision, t_scroller* decors ) ;
void deplacement_joueur( t_perso* acteur, BITMAP * collision, t_scroller * decors );
void deplacement_joueur_2( t_perso* acteur, BITMAP * collision, t_scroller * decors );
void deplacement_joueur_droite( t_perso* acteur, BITMAP * collision, t_scroller * decors );
void deplacement_joueur_gauche( t_perso* acteur, BITMAP * collision, t_scroller * decors );
void deplacement_joueur_haut( t_perso* acteur, BITMAP * collision, t_scroller * decors );
void deplacement_joueur_bas( t_perso* acteur, BITMAP * collision, t_scroller * decors );
void deplacement_ennemi_fixe( t_ennemi* courant, t_scroller* decors ) ;
void deplacement_tir(t_munition* courant) ;
void deplacement_explosions( t_explo* courant ) ;
void deplacement_objet( t_objet* courant );

///Tir
void tir_joueur( t_perso* acteur, int i );
void action_tir ( t_collection* missiles, BITMAP* buffer, BITMAP * collision, t_scroller* fond );
void tir_ennemi_mouvant( t_ennemi* courant ) ;
void tir_ennemi_fixe( t_ennemi* courant, t_perso* acteur, int n_perso[2] ) ;

///Collision
void tir_collision_avec_decor( BITMAP * collision, t_collection * ancre, t_scroller* fond );
void joueur_collision( t_perso* acteur, BITMAP* collision, t_ennemis* ancre, BITMAP* bonus, t_scroller* obstacles, int k ) ;
int acteur_collision( t_commun* acteur, BITMAP* collision );
int ennemi_collision( t_ennemi* acteur, BITMAP* collision ) ;
void collision_acteur_objet ( t_commun* acteur, t_objets* ancre );
/*Condition obstacle*/
int ConditionObstacleBas(t_commun * acteur, BITMAP * collision);
int ConditionObstacleHaut(t_commun * acteur, BITMAP * collision);
int ConditionObstacleGauche(t_commun * acteur, BITMAP * collision);
int ConditionObstacleDroite(t_commun * acteur, BITMAP * collision);
int objet_collision( t_perso* acteur, BITMAP* objet );

///Condition de supression
void condition_suppression_ennemi(  t_ennemi** premier, t_ennemi** dernier ) ;
void suppression_ennemi( t_ennemi** premier, t_ennemi** dernier, t_ennemi* suppression );

///Affichage
void AffichageTirs(t_collection * ancre, BITMAP*buffer, BITMAP * collision);
void scrolling_collision( t_scroller* collision, BITMAP* buffer_collision);
void scrolling( t_scroller* fond, BITMAP* buffer);
void affichage_ennemis(t_ennemi* courant, BITMAP * buffer, BITMAP* collision );
void AffichageExplosions( t_explosions* ancre, BITMAP* buffer ) ;
void AffichageObjet( t_objet* courant, BITMAP* buffer, BITMAP* bonus );
void AffichageBarreBas( t_perso * acteur, BITMAP * barrebas, BITMAP * buffer,const int k,const int nombreperso, long int timer, const t_scroller * scrol , const int score );
void AffichageTypePod(BITMAP * barrebas, int x, int y, int type);
void MenuPause(int * booleen, int nombrejoueur, char niveau[40], t_scroller * collision, t_perso * acteur);
void affichage_oiseau( t_ennemi* mechant, BITMAP* buffer, BITMAP* collision );
void AffichageChoixSauvegarde( int nomrejoueur, char niveau [40], t_scroller * collision, t_perso * acteur, BITMAP * bpause);
void affichage_bitmap_collision( t_commun* acteur, BITMAP* collision);
void FonduFin( BITMAP * debut, const char nom[40]);

///Gestion de libération de la mémoire
void vider_memoire( t_ennemis* ennemis_collec, t_collection* missiles, t_perso* acteur, int n_perso, t_scroller* fond, t_scroller* collision, BITMAP* buffer, BITMAP* buffer_collision, int taille, t_scroller* tabAttente, BITMAP * barrebas ) ;
void suppression_tous_ennemis( t_ennemi**premier, t_ennemi**dernier );
void suppression_de_tout_les_tirs( t_munition* courant, t_collection* missiles ) ;
void condition_suppression_objet( t_objet** courant, t_objets* ancre );
void SuppressionObjet(t_objet * suppression, t_objets * ancre);
void suppression_de_tout_les_objets( t_objets* ancre );

///Gestion bonus et nouveau corps
void nouveau_corps_sur_screen( t_ennemis* ennemis_collec, t_objets* bonus, t_scroller* tabAttente, t_collection* missiles, t_explosions* feu, int taille, int*k, t_scroller* fond, t_perso* acteur, int n_perso ) ;
void nouveau_tir_bouclier( int couleur, t_perso* acteur );
void recup_bonus( int couleur, t_perso* acteur, t_scroller* collision, int tabAttente );

///Sauvegarde
void sauvegarder(char nom[16], int nombre_joueur, char niveau[40], t_scroller * collision, t_perso * acteur );
void lectureMenu(char nom[16], int * niveau, int * nb_joueur);
void lectureJeu(char nom[20], int nombre_joueur, char niveau[40], t_scroller * fond, t_scroller * collision, t_perso * acteur);

///BOSS
void affichage_boss( t_ennemi* boss, BITMAP* buffer, BITMAP* collision) ;
void tir_boss( t_ennemi* boss, t_perso* acteur, int n_perso[2] ) ;
void changement_de_direction( t_ennemi* boss, BITMAP* collision );
void deplacement_boss( t_ennemi* boss );


#endif // BIBLIO_H_INCLUDED
