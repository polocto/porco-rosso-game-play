#include "biblio.h"
/**Sous programme qui permet la création d'un maillon de la liste des explosion.
Reçoit l'ancre de la liste chaînée explosion, la vitesse dx, dy et la partie commune de l'acteur touché.*/
void Ajout_explosion ( t_explosions* ancre, int dx, int dy, t_commun* base )
{
    int nombre_image = 8 ;
    BITMAP* buffer = load_bitmap("explosion.bmp",NULL ) ;
    t_explo* nouveau = malloc(sizeof(t_explo));
    if( nouveau == NULL )
        return;
    //selon si ennemis ou allié : position de l'explosion
    if( base->c == makecol(255,0,0) )
    {
        nouveau->base.x = base->x ;
    }
    else
    {
        nouveau->base.x = base->x + base->image[0]->w/2 ;
    }
    nouveau->base.dx = dx ;
    nouveau->base.dy = dy ;
    nouveau->suivant = NULL ;
    nouveau->time = 0;//clock() ;
    nouveau->base.image = (BITMAP**) malloc( nombre_image * sizeof( BITMAP* ) ) ;
    init_oiseau( &nouveau->base , buffer , nombre_image);
    //nouveau->base.image[0] = create_bitmap(20,20);
    //clear_to_color(nouveau->base.image[0],makecol(255,0,255));
    //circlefill(nouveau->base.image[0],10,10,10,makecol(255,255,0));
    nouveau->base.y = base->y + base->image[0]->h/2 - nouveau->base.image[0]->h/2 ;
    //ajout du nouveau maillon à la liste
    if( ancre->premier == NULL )
    {
        ancre->premier = nouveau ;
        ancre->dernier = nouveau ;
    }
    else
    {
        ancre->dernier->suivant = nouveau ;
        ancre->dernier = nouveau ;
    }
}
/**sous programme qui permet la création d'un maillon de la liste des missiles*/
/**A revoir ajout à la liste apres load_bitmap*/
void tirAjout(t_collection*collec, t_commun * tireur, int puissance, t_explosions* feu, int dx, int dy, int i, char image[30] )
{
    t_munition * nouveau = malloc(sizeof(t_munition));
    if( nouveau == NULL)
        return;
    if( collec->premier == NULL )
    {
        collec->premier = nouveau ;
        collec->dernier = nouveau ;
    }
    else
    {
        collec->dernier->suivant = nouveau ;
        collec->dernier = nouveau ;
    }
    nouveau->suivant = NULL ;
    nouveau->base.c = tireur->c ;
    nouveau->base.vie = puissance ;
    //pour se donner une idée rouge pour méchant
    if(tireur->c == makecol(255,0,0))
    {
        nouveau->base.x = tireur->x ;
        nouveau->base.dx = - dx ;
        nouveau->base.dy = dy ;
        nouveau->base.y = tireur->y + (tireur->image[0]->h)*0.5 ;
    }

    else
    {
        nouveau->base.x = tireur->x + tireur->image[0]->w ;
        nouveau->base.dx = dx ;
        nouveau->base.dy = dy ;
        if( i == 0 )
            nouveau->base.y = tireur->y + (tireur->image[0]->h)*0.7 ;
        else
            nouveau->base.y = tireur->y + (tireur->image[0]->h)*0.3 ;
    }
    nouveau->base.image = (BITMAP**) malloc(sizeof(BITMAP*));
    //nouveau->base.image[0] = create_bitmap(10, 5);
    nouveau->base.image[0] = load_bitmap(image,NULL);
    //rectfill(nouveau->base.image[0], 0,0,10,5, tireur->c );
    nouveau->explo = feu ;
}
/**Sous programme qui permet la création d'un maillon de la liste des ennemis.
Reçoit l'adresse du premier et dernier maillon de la chaîne ennemi, l'ancre de la chaîne missile,
le premier maillon de la chaine explosion, les coordonnées de création x, y, la vitesse dx, dy,
l'image souhaitée, le nombre d'image. */
void Ajout_ennemi(t_ennemi** premier, t_ennemi** dernier, t_collection* missiles,t_explosions* feu, int x, int y, int vie, int dy, char* image, int nb_image )
{
    t_ennemi* nouveau = NULL ;
    nouveau = (t_ennemi*) malloc(sizeof(t_ennemi));
    if( nouveau == NULL )
        return;
    if(!chargement_ennemi(nouveau, missiles, feu, x, y, vie, dy, image, nb_image ))
    {
        free(nouveau) ;
        return;
    }
    if( *premier == NULL)
    {
        *premier = nouveau ;
        *dernier = nouveau ;
    }
    else
    {
        (*dernier)->suivant = nouveau ;
        *dernier = nouveau ;
    }
}
/**Sous programme qui permet la création d'un maillon de la liste des bonus.
Reçoit l'ancre de la liste chaînée des bonus, la couleur du bonus,
le nom de l'image du bonus, ses coordonées x, y, son nombre de vie, sa vitesse dx, dy.*/
void Ajout_objet( t_objets* bonus, int couleur, char image[20], int x, int y, int vie, int dx, int dy )
{
    t_objet* nouveau = (t_objet*) malloc(sizeof(t_objet));
    if( nouveau == NULL )
        return;
    nouveau->base.image = (BITMAP**) malloc(sizeof(BITMAP*)) ;
    if( nouveau->base.image == NULL )
    {
        free(nouveau);
        return;
    }
    //nouveau->base.image[0] = create_bitmap(20,20);
    //rectfill(nouveau->base.image[0],0,0,20,20,couleur);
    nouveau->base.image[0] = load_bitmap( image , NULL ) ;
    if( !nouveau->base.image[0] )
    {
        free( nouveau->base.image ) ;
        free( nouveau );
        return ;
    }
    nouveau->base.c = couleur ;
    nouveau->base.x = x ;
    nouveau->base.y = y ;
    nouveau->base.vie = vie ;
    nouveau->base.dx = dx ;
    nouveau->base.dy = dy ;
    nouveau->suivant = NULL ;
    if( bonus->premier == NULL )
    {
        bonus->premier = nouveau ;
        bonus->dernier = nouveau ;
    }
    else
    {
        bonus->dernier->suivant = nouveau ;
        bonus->dernier = nouveau ;
    }
}
/**Programme qui permet de savoir si l'on ajoute et quand
un nouveau corps dans le jeu
ainsi que le chargement des fonds en début de partie*/
void nouveau_corps_sur_screen( t_ennemis* ennemis_collec, t_objets* bonus, t_scroller* tabAttente, t_collection* missiles, t_explosions* feu, int taille, int*k, t_scroller* fond, t_perso* acteur, int n_perso )
{
    //k variable corresspondant à notre position dans le tableau
    if(*k<3)
    {
        //chargement des fonds
        //avant début de partie
        choix_type( ennemis_collec, bonus, missiles, feu, &tabAttente[*k], fond, acteur, n_perso, *k ) ;
        (*k) ++ ;
    }
    else
    {
        while( *k < taille && tabAttente[*k].x - fond[1].x <= SCREEN_W)
        {
            //chargement en cours de partie
            choix_type( ennemis_collec, bonus, missiles, feu, &tabAttente[*k], fond, acteur, n_perso, *k ) ;
            (*k) ++ ;
        }
    }
}
void degradee( BITMAP* buffer , char nom [40] )
{
    int compteur = 0 , x , y ;
    BITMAP* fin = load_bitmap(nom,NULL);
    while(compteur<=fin->w && !key[KEY_ESC] )
    {
        clear_bitmap( buffer ) ;
        for( y = 0 ; y < fin->h ; y ++ )
            for( x = compteur ; x < fin->w ; x++ )
            putpixel(buffer, x, y, getpixel(fin, x, y) );
        compteur+=2;
        blit(buffer, screen, 0,0,0,0,800,600);
        //rest(5) ;
    }
}
