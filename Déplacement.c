#include "biblio.h"
/**G�re le d�placement des explosions pour qu'elles suivent le scroller.
Re�oit le maillon explosion que l'on souhaite d�placer.*/
void deplacement_explosions( t_explo* courant )
{
    while( courant != NULL )
    {
        courant->base.x += courant->base.dx ;
        courant->base.y += courant->base.dy ;
        courant = courant->suivant ;
    }
}
/**G�re le d�placement du personnage lorsque le joueur appuie pour aller � droite.
Re�oit la structure du perso, la BITMPA de collision, les param�tres du scroller d�cors. */
void deplacement_joueur_droite( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle � droite il continue � avancer avec le scroller
    if(ConditionObstacleDroite( &acteur->base, collision ) == 1)
    {
        //si on appuis sur la fl�che droite on va plus vite.
        if( key[KEY_RIGHT] && (acteur->base.x + acteur->base.image[0]->w + acteur->base.dx/* 0.5*/ < SCREEN_W - 1  || (decors->fin - decors->x < collision->w + decors->dx && acteur->opposants->premier_boss == NULL ) ))/// J'ai rajout� + acteur->base.dx
            acteur->base.x += acteur->base.dx ;
    }
    //Si on arrive au bout de l'�cran on ne peut aller plus loin
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.x -= decors->dx ;
}
/**G�re le d�placement du personnage lorsque le joueur appuie pour aller � gauche.
Re�oit la structure du perso, la BITMPA de collision, les param�tres du scroller d�cors. */
void deplacement_joueur_gauche( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle � gauche il continue � avancer avec le scroller
    if(ConditionObstacleGauche( &acteur->base, collision ) == 1)
    {
        //si on appuis sur la fl�che gauche on va plus vite.
        if( key[KEY_LEFT] && acteur->base.x - acteur->base.dx > 0 )
            acteur->base.x -= acteur->base.dx;
    }
    else
    {
        //si on arrive en bout d'�cran on ne peut aller plus loin
        if( key[KEY_LEFT] && acteur->base.x - acteur->base.dx > 0 )
        {
            if(decors->fin - decors->x >= collision->w + decors->dx)
                acteur->base.x -= decors->dx;
        }
    }
}
/**G�re le d�placement du personnage lorsque le joueur appuie pour aller en haut.
Re�oit la structure du perso, la BITMPA de collision, les param�tres du scroller d�cors. */
void deplacement_joueur_haut( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle en haut il continue � avancer avec le scroller
    if(ConditionObstacleHaut(&acteur->base, collision ) == 1)
    {
         //si on appuis sur la fl�che haut on va plus vite.
        if( key[KEY_UP] && acteur->base.y - acteur->base.dy > 0 )
            acteur->base.y -= acteur->base.dy;
    }
    //si on arrive en bout d'�cran on ne peut aller plus loin
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.y += decors->dy ;
}
/**G�re le d�placement du personnage lorsque le joueur appuie pour aller en bas.
Re�oit la structure du perso, la BITMPA de collision, les param�tres du scroller d�cors. */
void deplacement_joueur_bas( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle en bas il continue � avancer avec le scroller
    if(ConditionObstacleBas( &acteur->base, collision) == 1)
    {
         //si on appuis sur la fl�che bas on va plus vite.
        if( key[KEY_DOWN] && acteur->base.y + acteur->base.image[0]->h + acteur->base.dx /* 0.5 */< decors->image->h )
            acteur->base.y += acteur->base.dy;
    }
    //si on arrive en bout d'�cran on ne peut aller plus loin
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.y -= decors->dy ;
}
/**G�re les quatre directions du premier perso.
Re�oit la structure du perso, la BITMAP collision, les informations du d�cors. */
void deplacement_joueur( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    deplacement_joueur_droite( acteur, collision, decors );
    deplacement_joueur_gauche( acteur, collision, decors );
    deplacement_joueur_haut( acteur, collision, decors );
    deplacement_joueur_bas( acteur, collision, decors );
}
/**G�re le d�placement du personnage 2 lorsque le joueur appuie pour aller � droite.
Re�oit la structure du perso, la BITMPA de collision, les param�tres du scroller d�cors. */
void deplacement_joueur_2_droite( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle � droite il continue � avancer avec le scroller
    if(ConditionObstacleDroite( &acteur->base, collision ) == 1)
    {
        //si on appuis sur D on va plus vite.
        if( key[KEY_D] && (acteur->base.x + acteur->base.image[0]->w + acteur->base.dx/* 0.5*/ < SCREEN_W - 1 ||  decors->fin - decors->x < collision->w + decors->dx))/// J'ai rajout� + acteur->base.dx
            acteur->base.x += acteur->base.dx ;
    }
    else if( decors->fin - decors->x >= collision->w + decors->dx )
        acteur->base.x -= decors->dx ;
}
/**G�re le d�placement du personnage 2 lorsque le joueur appuie pour aller � gauche.
Re�oit la structure du perso, la BITMPA de collision, les param�tres du scroller d�cors. */
void deplacement_joueur_2_gauche( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle � gauche il continue � avancer avec le scroller
    if(ConditionObstacleGauche( &acteur->base, collision ) == 1)
    {
        //si on appuis sur Q on va plus vite.
        if( key[KEY_A] && acteur->base.x - acteur->base.dx > 0 )
            acteur->base.x -= acteur->base.dx;
    }
    else
    {
        if( key[KEY_A] && acteur->base.x - acteur->base.dx > 0 )
        {
            if(decors->fin - decors->x >= collision->w + decors->dx)
                acteur->base.x -= decors->dx;
        }
    }
}
/**G�re le d�placement du personnage 2 lorsque le joueur appuie pour aller en haut.
Re�oit la structure du perso, la BITMPA de collision, les param�tres du scroller d�cors. */
void deplacement_joueur_2_haut( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle en haut il continue � avancer avec le scroller
    if(ConditionObstacleHaut(&acteur->base, collision ) == 1)
    {
        //si on appuis sur Z on va plus vite.
        if( key[KEY_W] && acteur->base.y - acteur->base.dy > 0 )
            acteur->base.y -= acteur->base.dy;
    }
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.y += decors->dy ;
}
/**G�re le d�placement du personnage 2 lorsque le joueur appuie pour aller en bas.
Re�oit la structure du perso, la BITMPA de collision, les param�tres du scroller d�cors. */
void deplacement_joueur_2_bas( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle en bas il continue � avancer avec le scroller
    if(ConditionObstacleBas( &acteur->base, collision) == 1)
    {
        //si on appuis sur S on va plus vite.
        if( key[KEY_S] && acteur->base.y + acteur->base.image[0]->h + acteur->base.dx < decors->image->h )
            acteur->base.y += acteur->base.dy;
    }
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.y -= decors->dy ;
}
/**G�re les quatre directions du premier perso.
Re�oit la structure du perso, la BITMAP collision, les informations du d�cors. */
void deplacement_joueur_2( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    deplacement_joueur_2_droite( acteur, collision, decors );
    deplacement_joueur_2_gauche( acteur, collision, decors );
    deplacement_joueur_2_haut( acteur, collision, decors );
    deplacement_joueur_2_bas( acteur, collision, decors );
}
/**G�re le d�placement des tirs, le changement de position.
Re�oit la structure du tie � faire bouger. */
void deplacement_tir(t_munition* courant)
{
    while( courant != NULL)
    {
        courant->base.x += courant->base.dx ;
        courant->base.y += courant->base.dy ;
        courant = courant->suivant ;
    }
}
/**G�re le d�placement des ennemis fixes, donc avec le scroller.
Re�oit la structure l'ennemis fixe � d�placer, les param�tres du d�cors pour avoir un d�placement qui correspond � la vitesse. */
void deplacement_ennemi_fixe( t_ennemi* courant, t_scroller* decors )
{
    while( courant != NULL )
    {
        courant->base.x -= decors[1].dx;
        courant = courant->suivant ;
    }
}
/**G�re le d�placement des ennemis mouvant, donc avec des changements al�atoires de traj�ctoire..
Re�oit la structure l'ennemis mouvant � d�placer, la BITMAP de collision et les param�tres de d�fillement du scroller. */
void deplacement_ennemi_mouvant( t_ennemi* acteur, BITMAP * collision, t_scroller* decors )
{
    while( acteur != NULL)
    {
        acteur->base.y += acteur->base.dy ;
        if( acteur->dernier_mouvement + acteur->nouveau_mouvement < clock() )
        {
            acteur->dernier_mouvement = clock() ;
            acteur->nouveau_mouvement = (rand()% 6) * 300 +300 ;
            acteur->base.dy = -acteur->base.dy ;
        }
        //  changer de direction en attendant et a enlever ou conserver plus tard if ci-dessous
        if( acteur->base.y < 0 || acteur->base.y + acteur->base.image[0]->h > decors->image->h || ConditionObstacleBas( &acteur->base, collision) != 1 || ConditionObstacleHaut(&acteur->base, collision ) != 1 )
            acteur->base.dy = -acteur->base.dy ;
        if( ConditionObstacleDroite( &acteur->base, collision) == 0 )
            acteur->base.x -= decors->dx + 10;
        else if( ConditionObstacleGauche( &acteur->base, collision) == 0 )
            acteur->base.x += decors->dx;
        else
            acteur->base.x -= acteur->base.dx ;
        acteur = acteur->suivant ;
    }
}
/**G�re le d�placement des objets.
Re�oit la structure de l'objet � d�placer. */
void deplacement_objet( t_objet* courant )
{
    courant->base.x -= courant->base.dx ;
    courant->base.y += courant->base.dy ;
}
