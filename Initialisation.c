#include "biblio.h"
/**Affichage un message d'erreur en cas de problème de mémoire, notament lors du chargement d'images. */
void probleme_de_memoire()
{
    //message allegro
    allegro_message("Probleme d'allocation de memoire !") ;
    allegro_exit();
    //fermeture du programme en cas de bug
    exit(EXIT_FAILURE) ;
}
/**Gère l'initialisation des collection à NULL.
Reçoit l'ancre de la chaîne des missiles, l'ancre des ennemis, l'ancre des explosions, l'ancre des objets. */
void initialisation_des_collections( t_collection* missiles, t_ennemis* ennemis_collec, t_explosions*feu, t_objets* bonus )
{
    missiles->premier = NULL ;
    missiles->dernier = NULL ;
    feu->premier = NULL ;
    feu->dernier = NULL ;
    ennemis_collec->premier_fixe = NULL ;
    ennemis_collec->dernier_fixe = NULL ;
    ennemis_collec->premier_mouvant = NULL ;
    ennemis_collec->dernier_mouvant = NULL ;
    ennemis_collec->premier_boss = NULL ;
    ennemis_collec->dernier_boss = NULL ;
    bonus->premier = NULL ;
    bonus->dernier = NULL ;
}
/**Initialise toutes les données du jeu au début.
Reçoit le pointeur pour la structure fond, le pointeur pour la structure perso,
le nombre de perso, le pointeur pour la structure missiles, le pointeur pour le scroller collision,
le pointeur pour l'ancre des ennemis, le tableau de données des persos, le pointeur pour les explosions,
le pointeur pour les bonus, le niveau choisi, la taille du niveau */
void initialisation( t_scroller** fond, t_perso** acteur, int n_perso, t_collection** missiles, t_scroller** collision, t_ennemis ** ennemis_collec, int nombre_de_perso[2], t_explosions** feu, t_objets** bonus, t_scroller* tabAttente, int* k, int taille, char * niveau )
{
    int i=0;
    nombre_de_perso[0] = 0 ;
    nombre_de_perso[1] = n_perso ;
    *fond = (t_scroller*) malloc(2*sizeof(t_scroller)) ;
    if( *fond == NULL )
        probleme_de_memoire();
    *collision = (t_scroller*) malloc(sizeof(t_scroller)) ;
    if( *collision == NULL )
        probleme_de_memoire();
    *acteur = (t_perso*) malloc(n_perso*sizeof(t_perso)) ;
    if( *acteur == NULL )
        probleme_de_memoire();
    *missiles = (t_collection*) malloc(sizeof(t_collection)) ;
    if( *missiles == NULL )
        probleme_de_memoire();
    *ennemis_collec = (t_ennemis*) malloc(sizeof(t_ennemis)) ;
    if( *ennemis_collec == NULL )
        probleme_de_memoire();
    *feu = (t_explosions*) malloc(sizeof(t_explosions)) ;
    if( *feu == NULL )
        probleme_de_memoire();
    *bonus = (t_objets*) malloc(sizeof(t_objets)) ;
    if( *bonus == NULL )
        probleme_de_memoire();
    initialisation_des_collections( *missiles, *ennemis_collec, *feu, *bonus );
    for(i = 0 ; i<2 ; i++)
        nouveau_corps_sur_screen( *ennemis_collec, *bonus, tabAttente, *missiles, *feu, taille, k, &fond[0][i], *acteur, n_perso ) ;
    nouveau_corps_sur_screen( *ennemis_collec, *bonus, tabAttente, *missiles, *feu, taille, k, *collision, *acteur, n_perso ) ;
    for(i = nombre_de_perso[0] ; i < nombre_de_perso[1] ; i++)
    {
        acteur[0][i].base.image = NULL ;
        chargement_perso( &acteur[0][i], *missiles, 3, *feu, *k, *bonus, nombre_de_perso, i, 0, niveau, *ennemis_collec ) ;
        acteur[0][i].check[0] = 0 ;
        acteur[0][i].check[1] = acteur[0][i].base.y ;
    }
}
