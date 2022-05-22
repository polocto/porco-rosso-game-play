#include "biblio.h"
/**Ce sous programme permet de libérer la mémoire de toute la liste chaînée d'ennemi.
Reçoit le premier et le dernier en paramètre.*/
void suppression_tous_ennemis( t_ennemi**premier, t_ennemi**dernier )
{
    t_ennemi* courant = *premier ;
    t_ennemi* supp = NULL ;
    while( courant != NULL )
    {
        supp = courant ;
        courant = courant->suivant ;
        suppression_ennemi( premier, dernier, supp );
    }
    free(*premier) ;
    free(*dernier) ;
}
/**Ce sous programme permet de libérer la mémoire de toute la liste chaînée des tirs.
Reçoit le premier et l'ancre en paramètre.*/
void suppression_de_tout_les_tirs( t_munition* courant, t_collection* missiles )
{
    t_munition* supp = NULL ;
    while(courant!=NULL)
    {
        supp = courant ;
        courant = courant->suivant ;
        SuppressionTir( supp, missiles ) ;
    }
}
/**Ce sous programme permet de libérer la mémoire de toute la liste chaînée des explosions.
Reçoit l'ancre en paramètre.*/
void suppression_de_tout_les_explosions( t_explosions* ancre )
{
    t_explo* supp = NULL , *courant = ancre->premier ;
    while(courant!=NULL)
    {
        supp = courant ;
        courant = courant->suivant ;
        suppression_explosion( ancre , supp ) ;
    }
}
/**Ce sous programme rassemble les instructions pour une libération complète de la mémoire.
Données libérées : ennemis, missiles, objets, acteurs, scrollers.
Reçoit par paramètre toutes les données à libérer. */
void vider_memoire( t_ennemis* ennemis_collec, t_collection* missiles, t_perso* acteur, int n_perso, t_scroller* fond, t_scroller* collision, BITMAP* buffer, BITMAP* buffer_collision, int taille, t_scroller* tabAttente, BITMAP * barrebas )
{
    int i = 0 ;

    suppression_de_tout_les_objets( acteur->objet ) ;
    //libération de la mémoire des différents types d'ennemis
    suppression_tous_ennemis( &ennemis_collec->premier_fixe, &ennemis_collec->dernier_fixe );
    suppression_tous_ennemis( &ennemis_collec->premier_mouvant, &ennemis_collec->dernier_mouvant );
    suppression_tous_ennemis( &ennemis_collec->premier_boss, &ennemis_collec->dernier_boss );
    suppression_de_tout_les_tirs( missiles->premier, missiles );
    free( missiles->premier ) ;
    free( missiles->dernier ) ;
    free( acteur->explo );
    //libération de la mémoire de l'acteur
    for( i = 0 ; i < n_perso ; i++ )
    {
        free( acteur[i].base.image[0]) ;
        free( acteur[i].base.image ) ;
        free( acteur[i].pod ) ;
    }
    //libération de la mémoire du tableau d'évènement du niveau
    for( i = 0 ; i < taille ; i++ )
    {
        free(tabAttente[i].nom);
        ///free(tabAttente[i].type);
        free(tabAttente[i].image);
    }
    free(tabAttente);
    free( ennemis_collec ) ;
    free( missiles ) ;
    free( acteur ) ;
    //libération de la mémoire des scrollers et affichages
    free( fond[0].image) ;
    free( fond[0].nom ) ;
    ///free( fond[0].type ) ;
    free( fond[1].image) ;
    free( fond[1].nom ) ;
    ///free( fond[1].type ) ;
    free( fond ) ;
    free( collision->image ) ;
    free( collision->nom ) ;
    ///free( collision->type ) ;
    free( collision ) ;
    free( buffer ) ;
    free( buffer_collision ) ;
    free(barrebas);
}
