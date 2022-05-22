#include "biblio.h"
/**Gère le déplacement du boss et ses changements de directions.
Reçoit la structure du boss.*/
void deplacement_boss( t_ennemi* boss )
{
    while( boss != NULL )
    {
        //On determine le schéma d'action du boss en fonction de son nombre de vie
        if( boss->base.vie > 30 )
        {
            boss->base.x += boss->base.dx ;
            boss->base.y += boss->base.dy ;
            if( boss->dernier_mouvement + boss->nouveau_mouvement < clock() )
            {
                boss->base.dx = rand()%4 + 2 ;
                boss->base.dy = rand()%3 + 3 ;
                switch( rand()%20 )
                {
                case 0 :
                    boss->base.dx = -boss->base.dx ;
                    boss->base.dy = -boss->base.dy ;
                    break ;
                case 1 :
                    boss->base.dx = -boss->base.dx ;
                    break ;
                case 2 :
                    boss->base.dy = -boss->base.dy ;
                    break ;
                }
                boss->dernier_mouvement = clock() ;
                //on détermine le prochain changement d'action
                boss->nouveau_mouvement = rand() % 500 + 1500 ;
            }
        }
        //l'action est alors déterminer par le temps
        else if( boss->dernier_mouvement + boss->nouveau_mouvement < clock() )
        {
            //le boss change de position, il pop à un autre endroit aléatoire
            boss->base.x = ( SCREEN_W / 2 ) + rand() % ( ( SCREEN_W / 2 ) - boss->base.image[0]->w ) ;
            boss->base.y = rand() % ( ( SCREEN_H - 100 ) - boss->base.image[0]->h ) ;
            boss->dernier_mouvement = clock() ;
            //le prochain mouvement se fera à un temps aléatoire
            boss->nouveau_mouvement = rand() % 800 + 2000 ;
        }
        boss = boss->suivant ;
    }
}
/**Si le boss atteins un des bords de sa zone il rebomdi.
Reçoit la structure du boss, la BITMAP de collision.*/
void changement_de_direction( t_ennemi* boss, BITMAP* collision )
{
    while(boss!=NULL)
    {
        if( ( boss->base.x + boss->base.dx < SCREEN_W / 2 && boss->base.dx < 0 ) || ( boss->base.x + boss->base.dx + boss->base.image[0]->w > SCREEN_W && boss->base.dx > 0 ) )
            boss->base.dx = - boss->base.dx ;
        if( ( boss->base.y + boss->base.dy < 0 && boss->base.dy < 0 ) || ( boss->base.y + boss->base.dy + boss->base.image[0]->h > SCREEN_H - 100 && boss->base.dy > 0 ) )
            boss->base.dy = - boss->base.dy ;
        boss = boss->suivant ;
    }
}
/**Affichage du boss et de ses paramètres
Reçoit la structure du boss, la BITMAP buffer, la BITMAP collision.*/
void affichage_boss( t_ennemi* boss, BITMAP* buffer, BITMAP* collision)
{
    float coeff = 0 , vie = 0 ;
    while(boss!=NULL)
    {
        //affichage de la barre de vie
        vie = boss->base.vie ;
        coeff = vie/60.0000 ;
        rect( buffer ,boss->base.x, boss->base.y - 10 , boss->base.x + boss->base.image[0]->w , boss->base.y - 5 , makecol(255,255,255) ) ;
        rectfill( buffer ,boss->base.x, boss->base.y - 10 , boss->base.x + boss->base.image[0]->w * coeff , boss->base.y - 5 , makecol(255,255,150) ) ;
        //affichage de l'image du boss avec transparance
        draw_sprite( buffer, boss->base.image[0], boss->base.x, boss->base.y ) ;
        affichage_bitmap_collision(&boss->base,collision);
        boss = boss->suivant ;
    }
}
/**Création des tirs du boss.
Reçoit la structure du boss, la structure du perso, le nombre de perso.*/
void tir_boss( t_ennemi* boss, t_perso* acteur, int n_perso[2] )
{
    while(boss != NULL )
    {
        if( boss->tir + boss->nouveau_tir < clock() )
        {
            ///tir_ennemi_fixe
            ///tri_tir
            ///tir_raffal avec visé ( ennemi_fixe)
            tir_ennemi_fixe( boss, acteur, n_perso ) ;
            boss->tir = clock() ;
            boss->nouveau_tir = 300 ;
        }
        boss = boss->suivant ;
    }
}
