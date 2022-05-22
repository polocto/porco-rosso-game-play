#include "biblio.h"
/**Verifie s'il y a un obstacle de type décors à droite.
Retourne 0 si il y a collision, 1 sinon.
Reçoit la structure de base de l'acteur et la BITMAP de collision.*/
int ConditionObstacleDroite(t_commun * acteur, BITMAP * collision)
{
    int y = 0, x = 0 ;
    for( y = acteur->y + 1 ; y < acteur->y + acteur->image[0]->h - 1 ; y++)
    {
        x = acteur->image[0]->w - 1 ;
        while(getpixel(acteur->image[0], x, y - acteur->y) == makecol(255,0,255)  && x >= 0 )
            x--;
        if(getpixel(collision, acteur->x + x + 1, y ) == makecol(255,255,255) && getpixel(acteur->image[0], x, y - acteur->y) != makecol(255,0,255) )
            return 0 ;
    }
    return 1 ;
}
/**Verifie s'il y a un obstacle de type décors à gauche.
Retourne 0 si il y a collision, 1 sinon.
Reçoit la structure de base de l'acteur et la BITMAP de collision.*/
int ConditionObstacleGauche(t_commun * acteur, BITMAP * collision)
{
    int y = 0, x = 0 ;
    for( y = acteur->y + 1 ; y < acteur->y + acteur->image[0]->h - 1 ; y++)
    {
        x = 0 ;
        while(getpixel(acteur->image[0], x, y - acteur->y ) == makecol(255,0,255)  && x < acteur->image[0]->w  )
            x++;
        if(getpixel(collision, acteur->x + x - 1, y ) == makecol(255,255,255) && getpixel(acteur->image[0], x, y - acteur->y ) != makecol(255,0,255) )
            return 0 ;
    }
    return 1 ;
}
/**Verifie s'il y a un obstacle de type décors en haut.
Retourne 0 si il y a collision, 1 sinon.
Reçoit la structure de base de l'acteur et la BITMAP de collision.*/
int ConditionObstacleHaut(t_commun * acteur, BITMAP * collision)
{
    int x = 0, y = 0 ;
    for( x = acteur->x + 1 ; x < acteur->x + acteur->image[0]->w - 1 ; x++)
    {
        y = 0 ;
        while(getpixel(acteur->image[0], x - acteur->x, y ) == makecol(255,0,255)  && y < acteur->image[0]->h )
            y++;
        if(getpixel(collision, x, acteur->y + y - 1 ) == makecol(255,255,255) && getpixel(acteur->image[0], x - acteur->x, y ) != makecol(255,0,255) )
            return 0 ;
    }
    return 1 ;
}
/**Verifie s'il y a un obstacle de type décors en bas.
Retourne 0 si il y a collision, 1 sinon.
Reçoit la structure de base de l'acteur et la BITMAP de collision.*/
int ConditionObstacleBas(t_commun * acteur, BITMAP * collision)
{
    int x = 0, y = 0 ;
    for( x = acteur->x + 1 ; x < acteur->x + acteur->image[0]->w - 1 ; x++)
    {
        y = acteur->image[0]->h ;
        while(getpixel(acteur->image[0], x - acteur->x, y ) == makecol(255,0,255)  && y >= 0 )
            y--;
        if(getpixel(collision, x, acteur->y + y + 1) == makecol(255,255,255) && getpixel(acteur->image[0], x - acteur->x, y ) != makecol(255,0,255) )
            return 0 ;
    }
    return 1 ;
}
int acteur_collision( t_commun* acteur, BITMAP* collision )
{
    int x = 0, y = 0, couleur = 0 ;
    for( y = acteur->y /*-1*/ ; y <= acteur->y + acteur->image[0]->h /*+ 1 */; y++)
    {
        for( x = acteur->x/* - 1*/ ; x <= acteur->x + acteur->image[0]->w /*+ 1*/ ; x++)
        {
            couleur = getpixel(collision, x, y );
            if( (( couleur == makecol(255,0,0) && acteur->c == makecol(0,0,255) ) || ( couleur == makecol(0,0,255) && acteur->c == makecol(255,0,0) )) && getpixel(acteur->image[0], x - acteur->x, y - acteur->y) != makecol(255,0,255) )
            {
                return 0 ;
            }
        }
    }
    return 1 ;
}
/**Permet de détecter la collision entre le décor et les missiles.
Reçoit la Bitmap de collision, ses données de scroller correspondante et l'ancre de la liste chaînée des tir.*/
void tir_collision_avec_decor( BITMAP * collision, t_collection * ancre, t_scroller* fond )
{
    t_munition * courant = ancre->premier ;
    //On parcours toute la liste chaînée
    while( courant != NULL )
    {
        //les tirs se déplacant toujours vers la gauche on ne contrôle que les collisions de ce côté-ci
        if( ConditionObstacleGauche( &courant->base, collision ) == 0 || ConditionObstacleDroite( &courant->base, collision ) == 0 || acteur_collision( &courant->base, collision ) == 0 )
        {
            //if(acteur_collision( &courant->base, collision ) == 0 )
               // Ajout_explosion( courant->explo, -fond->dx, fond->dy, &courant->base );
            //Si l'objet entre en contact avec quelque chose le tir se supprime
            SuppressionTir( courant, ancre );
        }
        //si le tir sors de l'écran on l'éllimine aussi
        else if( courant->base.x + courant->base.image[0]->w < 0  ||  courant->base.x > SCREEN_W || acteur_collision( &courant->base, collision ) == 0 )
            SuppressionTir( courant, ancre );
        courant = courant->suivant ;
    }
}
/**Permet de détecter la collision entre les acteurs et les missiles et d'infliger par la même les dégats.
Reçoit la structure commune de l'acteur et l'ancre de la liste chaînée de missile.
Retourne le score de progression.*/
int collision_acteur_supp_missile ( t_commun* acteur, t_collection* ancre )
{
    t_munition* courant = ancre->premier ;
    t_munition* supp = NULL ;
    int verif = 1, x = 0, y = 0, score = acteur->vie ;
    //on parcours toute la liste chaînée de missile
    while( courant != NULL )
    {
        verif = 1 ;
        //on parcours tous les pixels de notre image pour savoir si il y a collision
        //si il y a collision on met verif à 0
        for( x = acteur->x ; x <= acteur->x + acteur->image[0]->w ; x++ )
            for( y = acteur->y ; y <= acteur->y + acteur->image[0]->h ; y++ )
                if( courant->base.x <= x && courant->base.x + courant->base.image[0]->w >= x && courant->base.y <= y && courant->base.y + courant->base.image[0]->h >= y )
                    verif = 0 ;
        if( verif == 0 )
        {
            //l'acteur perd alors des points de vies en fonction de la vie que possédait le tir
            acteur->vie -= courant->base.vie ;
            supp = courant ;
            //si l'acteur est un ennemis on fait apparaitre une explosion
            if(acteur->c == makecol(255,0,0))
                Ajout_explosion( courant->explo, -acteur->dx, acteur->dy, acteur );
            else
                Ajout_explosion( courant->explo,0,0, acteur );
            //on passe au maillon suivant avant de supprimer le maillon
            courant = courant->suivant ;
            SuppressionTir( supp, ancre );
        }
        else
            courant = courant->suivant;
    }
    if( acteur->vie < 0 )
        acteur->vie = 0 ;
    return score - acteur->vie ;
}
/**Permet de détecter la collision entre les joueurs et les ennemis.
A leur collision le joueur permet une vie et recommence.
Reçoit le premier et le dernier maillon d ela liste chaînée ennemi et la structure acteur.
Retourne la quantité à ajouter au score de progression.
1 s'il n'y a pas eu de contact, 0 sinon.*/
int collision_joueur_supp_ennemi( t_ennemi** premier, t_ennemi** dernier, t_perso* acteur )
{
    t_ennemi* courant = *premier ;
    t_ennemi* supp = NULL ;
    int verif = 1, y = 0, x = 0 ;
    while( courant != NULL )
    {
        verif = 1 ;
        //on parcours tous les pixels de notre image pour savoir si il y a collision
        //si il y a collision on met verif à 0
        for( x = acteur->base.x ; x <= acteur->base.x + acteur->base.image[0]->w ; x++ )
            for( y = acteur->base.y ; y <= acteur->base.y + acteur->base.image[0]->h ; y++ )
                if( courant->base.x <= x && courant->base.x + courant->base.image[0]->w >= x && courant->base.y <= y && courant->base.y + courant->base.image[0]->h >= y)
                    verif = 0 ;
        if( verif == 0 )
        {
            //l'acteur perd automatique un point de vie
            acteur->base.vie = -1 ;
            supp = courant ;
            //on ajoute une explosion au point de contact
            Ajout_explosion( courant->explo, 0, 0, &courant->base );
            courant = courant->suivant ;
            suppression_ennemi( premier, dernier, supp );
            //le joueur perd sa capacité spéciale
            free( acteur->pod );
            acteur->pod = NULL ;
            return 0 ;
        }
        else
            courant = courant->suivant;
    }
    return 1 ;
}
/**Permet de contrôler les collisions entre le décors et les ennemis.
Reçoit le premier maillon de la liste chaînée d'ennemis et la BITMAP de collision.
Retourne le score de progression total.*/
int ennemi_collision( t_ennemi* acteur, BITMAP* collision )
{
    int score = 0 ;
    while( acteur != NULL )
    {
        if( acteur_collision( &acteur->base, collision ) == 0 )
            score += collision_acteur_supp_missile( &acteur->base, acteur->ancre );
        acteur = acteur->suivant ;
    }
    return score ;
}
/**Pilier de la gestion des sous programme de collision avec le joueur.
Reçoit la structure de l'acteur, la BITMAP de collision,
l'ancre de la liste chaînée ennemis, la BITMAP es bonus, les données du scroller.
*/
void joueur_collision( t_perso* acteur, BITMAP* collision, t_ennemis* ancre, BITMAP* bonus, t_scroller* obstacles, int k )
{
    int couleur = 0, vie = 0 ;
    if( acteur_collision( &acteur->base, collision ) == 0 )
    {
        //En cas de contact on s'arrête dans notre recherche de contact
        if(acteur->pod != NULL && acteur->pod->force == 0 )
        {
            vie = acteur->base.vie ;
            if(collision_acteur_supp_missile( &acteur->base, acteur->ancre ) != 0 )
            {
                acteur->base.vie = vie ;
                return;
            }
        }
        else if(collision_acteur_supp_missile( &acteur->base, acteur->ancre ) != 0 )
            return ;
        //on test la collision avec les ennemis fixes
        if(!collision_joueur_supp_ennemi( &ancre->premier_fixe, &ancre->dernier_fixe, acteur ))
            return;
        //on test la collision avec le boss
        if(!collision_joueur_supp_ennemi( &ancre->premier_boss, &ancre->dernier_boss, acteur ))
            return;
        //on test la collision avec les ennemis mouvants
        if(!collision_joueur_supp_ennemi( &ancre->premier_mouvant, &ancre->dernier_mouvant, acteur ))
            return;
    }
    //on recupère la couleur d el'objet de collision
    couleur = objet_collision( acteur, bonus ) ;
    //si la couleur est différente de NULL, il s'agit d'un bonus
    if( 0 != couleur )
    {
        //le joueur récupère le bonus en question
        recup_bonus( couleur, acteur, obstacles, k );
        //On appelle la collision entre acteur et objet pour supprimer l'objet en question de l'affichage
        collision_acteur_objet( &acteur->base, acteur->objet ) ;
    }
}
/**Determine l'objet à effacer de l'affichage.
Reçoit la partie commune de l'acteur et l'ancre de la liste chaînée objet.
*/
void collision_acteur_objet ( t_commun* acteur, t_objets* ancre )
{
    t_objet* courant = ancre->premier ;
    t_objet* supp = NULL ;
    int verif = 1, x = 0, y = 0 ;
    //On pacours tous les objets de la liste chaînée
    while( courant != NULL )
    {
        verif = 1 ;
        //on regarde chaque pixel de l'acteur à la recherched'une collision
        for( x = acteur->x ; x <= acteur->x + acteur->image[0]->w ; x++ )
            for( y = acteur->y ; y <= acteur->y + acteur->image[0]->h ; y++ )
                if( courant->base.x <= x && courant->base.x + courant->base.image[0]->w >= x && courant->base.y <= y && courant->base.y + courant->base.image[0]->h >= y )
                    verif = 0 ;
        //Si il a contact
        if( verif == 0 )
        {
            //on supprime l'objet de l'affichage
            supp = courant ;
            courant = courant->suivant ;
            SuppressionObjet( supp, ancre );
        }
        //si il n'y a pas de contact on passe au maillon suivant
        else
            courant = courant->suivant;
    }
}
/**Balayage de la position du perso pour trouver un changement de couleur sur la BITMAP.
Reçoit la structure du perso et la BITMAP objet.
Retourne la couleur du pixel si celui-ci est différent de noir.
Sinon il renvoie 0.
*/
int objet_collision( t_perso* acteur, BITMAP* objet )
{
    int x = 0, y = 0 ;
    //on parcours toutes les positions de l'image sur la BITMAP objet à la recherche d'un changement de couleur
    for( x = acteur->base.x ; x <= acteur->base.x + acteur->base.image[0]->w ; x++ )
        for( y = acteur->base.y ; y <= acteur->base.y + acteur->base.image[0]->h ; y++ )
            if(getpixel(objet,x + 100,y) != makecol(0,0,0) )
                return getpixel( objet, x + 100, y ) ;
    //on retourne 0 si la position de l'acteur sur la BITMAP objet est "vide"
    return 0 ;
}
