#include "biblio.h"
/**Reserve de l'espace pour un pod et lui attribue une valeur lié au bonus récupéré.
Reçoit la couleur déterminant le pod, la structure de l'acteur.*/
void nouveau_tir_bouclier( int couleur, t_perso* acteur )
{
    //si l'acteur n'a pas de structure pod on lui en attribue une
    if(!acteur->pod)
        acteur->pod = (t_pod*) malloc(sizeof(t_pod)) ;
    //si l'attribution n'a pas été effectuée on quite
    if(!acteur->pod)
        return;
    //a chaque couleur correspond une force
    if( couleur == makecol( 255, 0, 0 ) )
        acteur->pod->force = 0 ;
    else if( couleur == makecol( 0, 0, 255 ) )
        acteur->pod->force = 1 ;
    else if( couleur == makecol( 255, 255, 0 ) )
        acteur->pod->force = 2 ;
    else if( couleur == makecol( 0, 255, 255 ) )
        acteur->pod->force = 3 ;
    else if( couleur == makecol( 255, 255, 255 ))
        acteur->pod->force = 4 ;
    //le pod mémorise le temps de sa création
    acteur->pod->temps = clock() ;
}
/**Selon la couleur sur la map bonus au niveau du joueur définit quel bonus a été récupéré
Reçoit la couleur, la structure du perso, les paramètres du scroller collision*/
void recup_bonus( int couleur, t_perso* acteur, t_scroller* collision, int tabAttente )
{
    if( couleur == 0 )
        return ;
    if( couleur == makecol( 0, 255, 0 ) )
        acteur->nb_de_vie++ ;
    else
        nouveau_tir_bouclier( couleur, acteur ) ;
}
