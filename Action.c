#include "biblio.h"
///Reçois en paramètre toutes les informations ayant un impact sur le joueur
///Et effectuer des action en fonction de ces informations
void action_joueur( t_perso* acteur, BITMAP* buffer, int nombre_de_perso[2], BITMAP * collision, t_scroller * decors, t_ennemis* ancre, BITMAP* objet, int k)
{
    int i=0, j = 0 ;
    float coeff = 0 ;
    //Actions propres au joueur 1
    if( nombre_de_perso[0] == 0 )
    {
        //deplacement joueur 1
        deplacement_joueur( &acteur[0], collision, decors ) ;
        //tir lorsque la barre espace se relache
        if(!key[KEY_SPACE])
            tir_joueur( acteur, 0 ) ;
        else if( acteur[0].pod != NULL && acteur[0].pod->force == 3 && acteur[0].chargement + 200 < clock() )
        {
            //tir raffale si bonus
            tirAjout( acteur[0].ancre, &acteur[0].base, 5, acteur[0].explo, 12, 0, 0,"tirmoyen.bmp") ;
            acteur[0].chargement = clock() ;
        }

        else if( ( acteur[0].pod == NULL || acteur[0].pod->force == 0 ) && acteur[0].chargement + 200 < clock() )
        {
            //   afficher la bar de chargement du tir
            coeff = (clock()-acteur[0].chargement)/1000.000;
            if( coeff > 1 )
                coeff = 1 ;
            rectfill(buffer, acteur[0].base.x, acteur[0].base.y - 5, acteur[0].base.x + (coeff*acteur[0].base.image[0]->w), acteur[0].base.y, makecol( 0, 255, 0));
        }
    }
    //Actions propres au joueur 1
    if( nombre_de_perso[1] == 2 )
    {
        //deplacement joueur 2
        deplacement_joueur_2( &acteur[1], collision, decors );
        //tir lorsque la touche tab se relache
        if(!key[KEY_TAB])
            tir_joueur( acteur, 1 ) ;
        else if( acteur[1].pod != NULL && acteur[1].pod->force == 3 && acteur[1].chargement + 200 < clock() )
        {
            //tir raffale si bonus
            tirAjout( acteur[1].ancre, &acteur[1].base, 5, acteur[1].explo, 12, 0, 1, "tirmoyen.bmp") ;
            acteur[1].chargement = clock() ;
        }
        else if( ( acteur[1].pod == NULL || acteur[1].pod->force == 0 ) && acteur[1].chargement + 200 < clock() )
        {
            //afficher la bar de chargement du tir
            coeff = (clock()- acteur[1].chargement)/1000.000;
            if( coeff > 1 )
                coeff = 1 ;

            rectfill(buffer, acteur[1].base.x, acteur[1].base.y - 10, acteur[1].base.x + (coeff*acteur[1].base.image[0]->w), acteur[1].base.y - 5, makecol( 0, 255, 0));
        }
    }
    //Actions communes aux deux joueurs
    for( i = nombre_de_perso[0] ; i < nombre_de_perso[1] ; i++ )
    {
        //collision de joueurs avec missiles ou ennemis
        joueur_collision( &acteur[i], collision, ancre, objet, decors, k );
        //affichage de l'acteur sur le buffer
        draw_sprite( buffer, acteur[i].base.image[0], acteur[i].base.x, acteur[i].base.y ) ;
        if( acteur[i].pod != NULL &&  acteur[i].pod->force == 0 )
            circle( buffer , acteur[i].base.x + acteur[i].base.image[0]->w/2 , acteur[i].base.y + acteur[i].base.image[0]->h/2 , acteur[i].base.image[0]->w/2 , makecol( 255 , 255 , 255 )) ;
        //Suppression du pod
        if( acteur[i].pod != NULL && ( acteur[i].pod->temps + 10000 < clock() || acteur[i].pod->force == 4 ) )
        {
            if(acteur[i].pod->force == 3 )
                acteur[i].chargement = clock() ;
            //Réccupération checkpoint
            if(acteur[i].pod->force == 4 )
            {
                for( j = nombre_de_perso[0] ; j < nombre_de_perso[1] ; j++ )
                {
                    acteur[j].check[0] = acteur[i].base.x  + decors->x - 50 ;
                    acteur[j].check[1] = acteur[i].base.y ;
                    acteur[j].check[2] = acteur[0].score ;
                }

            }
            //liberation espace
            free( acteur[i].pod );
            acteur[i].pod = NULL ;
        }
    }
}
///Reçois en paramètre toutes les informations ayant un impact sur les ennemis
///Et effectuer des action en fonction de ces informations
void action_ennemi( t_ennemis* ancre, BITMAP* buffer, t_scroller* fond, BITMAP * collision, t_perso* acteur, int n_perso[2] )
{
    //deplacement des ennemis volant
    deplacement_ennemi_mouvant( ancre->premier_mouvant, collision, fond ) ;
    //deplacement des ennemis fixés au décors
    if(fond[1].fin - fond[1].x >= collision->w - 1)
        deplacement_ennemi_fixe( ancre->premier_fixe, fond ) ;

    deplacement_boss(ancre->premier_boss ) ;
    changement_de_direction( ancre->premier_boss, collision );
    //récupération du score lié au dégats infligés
    acteur->score += ennemi_collision( ancre->premier_fixe, collision ) ;
    acteur->score += ennemi_collision( ancre->premier_mouvant, collision );
    acteur->score += ennemi_collision( ancre->premier_boss, collision );
    //tirs différents pour chaque catégories d'ennemis
    tir_ennemi_mouvant( ancre->premier_mouvant ) ;
    tir_ennemi_fixe( ancre->premier_fixe, acteur, n_perso ) ;
    tir_boss( ancre->premier_boss, acteur, n_perso ) ;
    //conditions de suppressions des ennemis :
    //sortie de la fenêtre
    //vie<=0
    condition_suppression_ennemi( &ancre->premier_fixe, &ancre->dernier_fixe ) ;
    condition_suppression_ennemi( &ancre->premier_mouvant, &ancre->dernier_mouvant ) ;
    condition_suppression_ennemi( &ancre->premier_boss, &ancre->dernier_boss ) ;
    //affichage des différentes catégories d'ennemis
    affichage_ennemis( ancre->premier_fixe, buffer, collision ) ;
    affichage_oiseau(ancre->premier_mouvant, buffer, collision ) ;
    affichage_boss( ancre->premier_boss, buffer, collision ) ;
}
///Reçois en paramètre toutes les informations ayant un impact sur les tirs
///Et effectuer des action en fonction de ces informations
void action_tir ( t_collection* missiles, BITMAP* buffer, BITMAP * collision, t_scroller* fond )
{
    //affichage
    AffichageTirs( missiles, buffer, collision ) ;
    //collision
    tir_collision_avec_decor( collision, missiles, fond ) ;
    //deplacement
    deplacement_tir( missiles->premier ) ;
}
///Reçois en paramètre toutes les informations ayant un impact sur les explosions
///Et effectuer des action en fonction de ces informations
void action_explosions( t_explosions* feu, BITMAP* buffer, t_scroller* decors )
{
    //deplacement explosions
    if(decors->fin - decors->x >= buffer->w + decors->dx)
        deplacement_explosions( feu->premier );
    //affichage des explosions
    AffichageExplosions( feu, buffer );
}
///Reçois en paramètre toutes les informations ayant un impact sur les objets
///Et effectuer des action en fonction de ces informations
void action_objet( t_objets* ancre, BITMAP* buffer, BITMAP* bonus , t_scroller* decors )
{
    t_objet* courant = ancre->premier ;
    clear_bitmap( bonus ) ;
    while( courant != NULL )
    {
        if(decors->fin - decors->x >= buffer->w + decors->dx)
            deplacement_objet( courant ) ;
        AffichageObjet( courant, buffer, bonus ) ;
        condition_suppression_objet( &courant, ancre ) ;
    }
}
