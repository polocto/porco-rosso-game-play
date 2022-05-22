#include "biblio.h"
/**Gère le déplacement des explosions pour qu'elles suivent le scroller.
Reçoit le maillon explosion que l'on souhaite déplacer.*/
void deplacement_explosions( t_explo* courant )
{
    while( courant != NULL )
    {
        courant->base.x += courant->base.dx ;
        courant->base.y += courant->base.dy ;
        courant = courant->suivant ;
    }
}
/**Gère le déplacement du personnage lorsque le joueur appuie pour aller à droite.
Reçoit la structure du perso, la BITMPA de collision, les paramètres du scroller décors. */
void deplacement_joueur_droite( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle à droite il continue à avancer avec le scroller
    if(ConditionObstacleDroite( &acteur->base, collision ) == 1)
    {
        //si on appuis sur la flèche droite on va plus vite.
        if( key[KEY_RIGHT] && (acteur->base.x + acteur->base.image[0]->w + acteur->base.dx/* 0.5*/ < SCREEN_W - 1  || (decors->fin - decors->x < collision->w + decors->dx && acteur->opposants->premier_boss == NULL ) ))/// J'ai rajouté + acteur->base.dx
            acteur->base.x += acteur->base.dx ;
    }
    //Si on arrive au bout de l'écran on ne peut aller plus loin
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.x -= decors->dx ;
}
/**Gère le déplacement du personnage lorsque le joueur appuie pour aller à gauche.
Reçoit la structure du perso, la BITMPA de collision, les paramètres du scroller décors. */
void deplacement_joueur_gauche( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle à gauche il continue à avancer avec le scroller
    if(ConditionObstacleGauche( &acteur->base, collision ) == 1)
    {
        //si on appuis sur la flèche gauche on va plus vite.
        if( key[KEY_LEFT] && acteur->base.x - acteur->base.dx > 0 )
            acteur->base.x -= acteur->base.dx;
    }
    else
    {
        //si on arrive en bout d'écran on ne peut aller plus loin
        if( key[KEY_LEFT] && acteur->base.x - acteur->base.dx > 0 )
        {
            if(decors->fin - decors->x >= collision->w + decors->dx)
                acteur->base.x -= decors->dx;
        }
    }
}
/**Gère le déplacement du personnage lorsque le joueur appuie pour aller en haut.
Reçoit la structure du perso, la BITMPA de collision, les paramètres du scroller décors. */
void deplacement_joueur_haut( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle en haut il continue à avancer avec le scroller
    if(ConditionObstacleHaut(&acteur->base, collision ) == 1)
    {
         //si on appuis sur la flèche haut on va plus vite.
        if( key[KEY_UP] && acteur->base.y - acteur->base.dy > 0 )
            acteur->base.y -= acteur->base.dy;
    }
    //si on arrive en bout d'écran on ne peut aller plus loin
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.y += decors->dy ;
}
/**Gère le déplacement du personnage lorsque le joueur appuie pour aller en bas.
Reçoit la structure du perso, la BITMPA de collision, les paramètres du scroller décors. */
void deplacement_joueur_bas( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle en bas il continue à avancer avec le scroller
    if(ConditionObstacleBas( &acteur->base, collision) == 1)
    {
         //si on appuis sur la flèche bas on va plus vite.
        if( key[KEY_DOWN] && acteur->base.y + acteur->base.image[0]->h + acteur->base.dx /* 0.5 */< decors->image->h )
            acteur->base.y += acteur->base.dy;
    }
    //si on arrive en bout d'écran on ne peut aller plus loin
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.y -= decors->dy ;
}
/**Gère les quatre directions du premier perso.
Reçoit la structure du perso, la BITMAP collision, les informations du décors. */
void deplacement_joueur( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    deplacement_joueur_droite( acteur, collision, decors );
    deplacement_joueur_gauche( acteur, collision, decors );
    deplacement_joueur_haut( acteur, collision, decors );
    deplacement_joueur_bas( acteur, collision, decors );
}
/**Gère le déplacement du personnage 2 lorsque le joueur appuie pour aller à droite.
Reçoit la structure du perso, la BITMPA de collision, les paramètres du scroller décors. */
void deplacement_joueur_2_droite( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle à droite il continue à avancer avec le scroller
    if(ConditionObstacleDroite( &acteur->base, collision ) == 1)
    {
        //si on appuis sur D on va plus vite.
        if( key[KEY_D] && (acteur->base.x + acteur->base.image[0]->w + acteur->base.dx/* 0.5*/ < SCREEN_W - 1 ||  decors->fin - decors->x < collision->w + decors->dx))/// J'ai rajouté + acteur->base.dx
            acteur->base.x += acteur->base.dx ;
    }
    else if( decors->fin - decors->x >= collision->w + decors->dx )
        acteur->base.x -= decors->dx ;
}
/**Gère le déplacement du personnage 2 lorsque le joueur appuie pour aller à gauche.
Reçoit la structure du perso, la BITMPA de collision, les paramètres du scroller décors. */
void deplacement_joueur_2_gauche( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle à gauche il continue à avancer avec le scroller
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
/**Gère le déplacement du personnage 2 lorsque le joueur appuie pour aller en haut.
Reçoit la structure du perso, la BITMPA de collision, les paramètres du scroller décors. */
void deplacement_joueur_2_haut( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle en haut il continue à avancer avec le scroller
    if(ConditionObstacleHaut(&acteur->base, collision ) == 1)
    {
        //si on appuis sur Z on va plus vite.
        if( key[KEY_W] && acteur->base.y - acteur->base.dy > 0 )
            acteur->base.y -= acteur->base.dy;
    }
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.y += decors->dy ;
}
/**Gère le déplacement du personnage 2 lorsque le joueur appuie pour aller en bas.
Reçoit la structure du perso, la BITMPA de collision, les paramètres du scroller décors. */
void deplacement_joueur_2_bas( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    //s'il n'y a pas d'obstacle en bas il continue à avancer avec le scroller
    if(ConditionObstacleBas( &acteur->base, collision) == 1)
    {
        //si on appuis sur S on va plus vite.
        if( key[KEY_S] && acteur->base.y + acteur->base.image[0]->h + acteur->base.dx < decors->image->h )
            acteur->base.y += acteur->base.dy;
    }
    else if(decors->fin - decors->x >= collision->w + decors->dx)
        acteur->base.y -= decors->dy ;
}
/**Gère les quatre directions du premier perso.
Reçoit la structure du perso, la BITMAP collision, les informations du décors. */
void deplacement_joueur_2( t_perso* acteur, BITMAP * collision, t_scroller * decors )
{
    deplacement_joueur_2_droite( acteur, collision, decors );
    deplacement_joueur_2_gauche( acteur, collision, decors );
    deplacement_joueur_2_haut( acteur, collision, decors );
    deplacement_joueur_2_bas( acteur, collision, decors );
}
/**Gère le déplacement des tirs, le changement de position.
Reçoit la structure du tie à faire bouger. */
void deplacement_tir(t_munition* courant)
{
    while( courant != NULL)
    {
        courant->base.x += courant->base.dx ;
        courant->base.y += courant->base.dy ;
        courant = courant->suivant ;
    }
}
/**Gère le déplacement des ennemis fixes, donc avec le scroller.
Reçoit la structure l'ennemis fixe à déplacer, les paramètres du décors pour avoir un déplacement qui correspond à la vitesse. */
void deplacement_ennemi_fixe( t_ennemi* courant, t_scroller* decors )
{
    while( courant != NULL )
    {
        courant->base.x -= decors[1].dx;
        courant = courant->suivant ;
    }
}
/**Gère le déplacement des ennemis mouvant, donc avec des changements aléatoires de trajéctoire..
Reçoit la structure l'ennemis mouvant à déplacer, la BITMAP de collision et les paramètres de défillement du scroller. */
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
/**Gère le déplacement des objets.
Reçoit la structure de l'objet à déplacer. */
void deplacement_objet( t_objet* courant )
{
    courant->base.x -= courant->base.dx ;
    courant->base.y += courant->base.dy ;
}
