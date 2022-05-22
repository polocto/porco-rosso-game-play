#include "biblio.h"
/**initialisation des image ennemis
Reçoit la partie commune de l'ennemi, la BITAMP de l'image complète, le nombre d'image voulue.*/
void init_oiseau(t_commun*mechant, BITMAP*image, int nb_image)
{
    int y = 0, i =0, k = 0, x1 = 0, x2 = 0, x = -1;
    for( i = 0 ; i < nb_image ; i++ )
    {
        //permet de découper l'image peut importe l'écart entre les images
        do
        {
            x++ ;
            if( x > image->w )
                x = 0 ;
            for( y = 0 ; y < image->h ; y++ )
                if( getpixel(image, x, y ) != makecol(255,0,255) )
                    k = 1 ;
        }
        while( k == 0 ) ;
        x1 = x ;
        do
        {
            x++ ;
            k = 0 ;
            for( y = 0 ; y < image->h ; y++ )
                if( getpixel( image, x, y ) != makecol(255,0,255) )
                    k = 1 ;
        }
        while( k != 0 && x <= image->w ) ;
        x2 = x ;
        mechant->image[i] = create_bitmap( x2 - x1 + 1, image->h ) ;
        clear_to_color( mechant->image[i], makecol(255,0,255));
        blit( image, mechant->image[i], x1, 0, 0, 0, x2 - x1, image->h );
    }
}
///initialisation des scroller image de decor de fond/collision
/*Initialisation de toutes les caractéristiques de l'arrière plan*/
///Reçoit la structure du scroller, la taille, la vitesse, le pixel de fin, le nom de l'image
void chargement_fond(t_scroller* fond, int taille_x, int taille_y, int dx, int dy, int fin, char* nom )
{
    ///fond->image=create_bitmap(taille_x,taille_y);/*définir la taille du bitmap*/
    /*réccupérer l'image de type .bmp dans le bitmap en question*/
    fond->image=load_bitmap(nom,NULL);
    /*vérification du bon chargement de l'image*/
    if(!fond->image)
    {
        /*message allegro*/
        allegro_message("Probleme de chargement de l'image fond !");
        allegro_exit();
        /*fermeture du programme en cas de bug*/
        exit(1);
    }
    fond->dx = dx ;
    fond->dy = dy ;
    fond->x = 0 ;
    fond->y = 0 ;
    fond->fin = fin ;
    fond->nom = NULL ;
    fond->type = '0' ;
    fond->taillex = 0 ;
    fond->tailley = 0 ;
}
/**Initialisation des ennemis.
Reçoit le ointeur de l'ennemis à initialiser, l'ancre de la liste chaînée missile,
l'ancre des explosions, les coordonées x, y, la vie, la vitesse dy,
le nom de l'image à charger, le nombre d'image total.
Retourne 0 en cas de problème, 1 si le chargement a été correctement effectué. */
int chargement_ennemi(t_ennemi*mechant, t_collection* missiles, t_explosions* feu, int x, int y, int vie, int dy, char image[30], int nb_image )
{
    int i = 0 ;
    BITMAP* buffer = load_bitmap(image,NULL);
    if(!buffer)
        return 0 ;
    mechant->suivant = NULL ;
    //reservation de n espace en cas de perso animé
    mechant->base.image = (BITMAP**) malloc( nb_image * sizeof(BITMAP*)) ;
    //verification du bon fonctionnement du malloc
    if( mechant->base.image == NULL )
        return 0 ;
    //chargement image
    init_oiseau( &mechant->base, buffer, nb_image ) ;
    for( i = 0 ; i < nb_image ; i++ )
        //vérification du bon chargement de l'image
        if( ! mechant->base.image[i] )
            return 0 ;
    mechant->base.c = makecol(255,0,0) ;
    mechant->base.dx = 2 ;
    mechant->base.dy = dy ;
    mechant->base.x = x ;
    mechant->base.y = y ;
    mechant->base.vie = vie  ;
    mechant->ancre = missiles ;
    mechant->nouveau_tir = 1500 ;
    mechant->tir = clock() - mechant->nouveau_tir/2 ;
    mechant->explo = feu ;
    mechant->affichage = 0 ;
    return 1 ;
}
/**Initialisation de toutes les caractéristiques du personnages.
Reçoit le pointeur de la structure, l'ancre de la liste chaînée missile,
le nombre de vie, l'ancre de la liste chaînée des explosions, l'ancre de la liste d'objets,
le pointeur vers le tableau du nombre de perso, le score, le nom du niveau, l'ancre vers les ennemis.*/
void chargement_perso(t_perso* acteur, t_collection* missiles, int nombre_vie, t_explosions* feu, int k, t_objets* objet, int* nb_de_perso, int i, int score, char * niveau, t_ennemis*mechant )
{
    int A = 0 ;
    //determine le décallage du départ du perso sur l'axe y
    if(strcmp(niveau, "niveau1.txt") == 0)
        A = 0 ;
    else if(strcmp(niveau, "niveau2.txt") == 0)
        A = -250 ;
    else if(strcmp(niveau, "niveau3.txt") == 0)
        A = 150;
    if(acteur->base.image != NULL )
    {
        free(acteur->base.image[0]);
        free(acteur->base.image);
    }
    //reservation de n espace en cas de perso animé
    acteur->base.image = (BITMAP**) malloc(sizeof(BITMAP*)) ;
    //verification du bon fonctionnement du malloc
    if( acteur->base.image == NULL )
    {
        //message allegro
        allegro_message("Probleme d'allocation de memoire !") ;
        allegro_exit();
        //fermeture du programme en cas de bug
        exit(EXIT_FAILURE) ;
    }
    switch(i)
    {
    case 0 :
        //réccupérer l'image de type .bmp dans le bitmap en question
        acteur->base.image[0] = load_bitmap("vaisseau1.bmp",NULL) ;
        break;
    case 1 :
        //réccupérer l'image de type .bmp dans le bitmap en question
        acteur->base.image[0] = load_bitmap("vaisseau2.bmp",NULL) ;
        break;
    }
    //vérification du bon chargement de l'image
    if( ! acteur->base.image[0] )
    {
        //message allegro
        allegro_message("Probleme de chargement de l'image spaceship !") ;
        allegro_exit();
        //fermeture du programme en cas de bug
        exit(EXIT_FAILURE);
    }
    //on initialise chacune des variables de la structure
    acteur->base.c = makecol(0,0,255) ;
    acteur->base.dx = 8 ;
    acteur->base.dy = 6 ;
    acteur->base.vie = 20 ;
    acteur->base.x = 45 ;
    acteur->base.y =  SCREEN_H/2 - (acteur->base.image[0]->h/2) + ((nb_de_perso[i]-1)*20) - 20 + A ;
    acteur->chargement = clock() ;
    acteur->check[2] = 0 ;
    acteur->munition_reserve = 30 ;
    acteur->nb_de_vie = nombre_vie ;
    acteur->pod = NULL ;
    acteur->score = score ;
    acteur->ancre = missiles ;
    acteur->explo = feu ;
    acteur->objet = objet ;
    acteur->opposants = mechant ;
}
