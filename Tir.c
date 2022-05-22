#include "biblio.h"
/**Ce sous programme permet de g�rer la cr�ation des tirs des ennemis fixes.
Il re�oit en param�tre le premier maillon, la structure acteur, et le nombre de personnage.
La structure du personnage est n�cessaire car les ennemis fixes visent le personnages.  */
void tir_ennemi_fixe( t_ennemi* courant, t_perso* acteur, int n_perso[2] )
{
    long int ecart = 0, i = 0, a=0 ;
    int ecarty = 0, memoire = SCREEN_H ;
    int dx = 0 ;
    //On effectue la cr�ation pour tous les ennemis fixes
    while( courant != NULL )
    {
        ecarty = 0 ;
        memoire = SCREEN_H ;
        //Les tirs des ennemis se font tous les intevralles de temps fix�s au pr�alable
        //La cr�ation de tir et ses calculs ne se fait que si l'intervalle de temps est pass�
        if( courant->tir + courant->nouveau_tir < clock() )
        {
            for( i = n_perso[0] ; i < n_perso[1] ; i++ )
            {
                //On calcul la position de l'acteur par rapport � l'ennemi
                ecarty = acteur[i].base.y + acteur[i].base.image[0]->h/2 - courant->base.y  ;
                if( courant->base.x - acteur[i].base.x > 0 )
                {
                    if( ecarty < memoire )
                    {
                        ecart = ecarty ;
                        a = i ;
                    }
                }
                memoire = courant->base.y - acteur[i].base.y ;
            }
            if( courant->base.dx < 0 )
                dx = -courant->base.dx ;
            else
                dx = courant->base.dx ;
            memoire = ( courant->base.x - acteur[a].base.x - acteur[a].base.image[0]->w/2 /*- ((courant->base.x - acteur[a].base.x)/4) */ )/ dx ;
            if( memoire != 0 )
                memoire = (ecart*4) / memoire ;
            if( memoire > 13 )
                memoire = 13 ;
            if( memoire < -13 )
                memoire = -13 ;
            //on appele le sous-programme pour la cr�ation du tir
            tirAjout( courant->ancre, &courant->base, 1, courant->explo, 7, memoire, 0, "tirennemifixe.bmp" ) ;
            courant->tir = clock() ;
        }
        courant = courant->suivant ;
    }
}
/**Cr�ation des tirs des �nnemis mouvants.
Re�oit en param�tre la premier maillon des ennemis mouvants */
void tir_ennemi_mouvant( t_ennemi* courant )
{
    while( courant != NULL )
    {
        //On regarde si l'intervalle de temps imparti entre chaque tir est effectu�
        if( courant->tir + courant->nouveau_tir < clock() )
        {
            tirAjout( courant->ancre, &courant->base, 1, courant->explo, 7, 0, 1, "graine1.bmp" );
            courant->tir = clock() ;
        }
        courant = courant->suivant ;
    }
}
/**Ce programme permet d'effectuer les tirs sp�ciaux du joueur.
Re�oit la structure de l'acteur et le num�ro du joueur. */
void tir_speciaux( t_perso* acteur, int i )
{
    switch( acteur->pod->force )
    {
    case 1 :
        //Tir tri-direction
        tirAjout( acteur->ancre, &acteur->base, 3, acteur->explo, 12, 0, i, "tirsimple.bmp" ) ;
        tirAjout( acteur->ancre, &acteur->base, 3, acteur->explo, 12, 6, i, "tirsimple.bmp" ) ;
        tirAjout( acteur->ancre, &acteur->base, 3, acteur->explo, 12, -6, i, "tirsimple.bmp" ) ;
        break;
    case 2 :
        //Tir mitraillette
        tirAjout( acteur->ancre, &acteur->base, 10, acteur->explo, 12, 0, i, "tirpuissant.bmp" ) ;
        break;
    }
}
/**Ce programme permet d'effectuer les tirs normaux du joueur.
Re�oit la structure de l'acteur et le num�ro du joueur. */
void tir_classique( t_perso* acteur, int i )
{
    //On effectue �galement les tirs charg�s du joueur
    if( acteur[i].chargement + 80 < clock() && acteur[i].chargement + 200 > clock() )
        tirAjout( acteur[i].ancre, &acteur[i].base, 1, acteur[i].explo, 12, 0, i, "tirsimple.bmp" ) ;
    else if( acteur[i].chargement + 200 <= clock() )
    {
        if(acteur[i].chargement + 1000 > clock() )
            tirAjout( acteur[i].ancre, &acteur[i].base, ( clock() - acteur[i].chargement ) / 100, acteur[i].explo, 12, 0, i, "tirmoyen.bmp" );        //Envoyer la valeur de la puissance � donner au missile (points de vie) eg : envoyer ( clock() - acteur->chargement) / 100
        else
            tirAjout( acteur[i].ancre, &acteur[i].base, 10, acteur[i].explo, 12, 0, i, "tirpuissant.bmp");
    }
}
/**Sous-programme pilier de la cr�ation des tirs du joueur.
Il permet de g�rer l'appel au bon sous programme de cr�ation de tir.
Re�oit la structure du joueur et le num�ro du joueur. */
void tir_joueur( t_perso* acteur, int i )
{
    if( acteur[i].pod == NULL || (acteur[i].pod->force != 1 && acteur[i].pod->force != 2 && acteur[i].pod->force != 3 ))
        tir_classique( acteur, i ) ;
    else if( acteur[i].chargement + 100 < clock() )
        tir_speciaux( &acteur[i], i ) ;
    if( acteur[i].pod == NULL || acteur[i].pod->force != 3 )
        acteur[i].chargement = clock() ;
}
