#include "biblio.h"
/**Programme principale du jeu
déclaration de toutes les varaible des joueurs/ennemies/scroller
des variables pour le timer ainsi que des BITMAP pour l'affichage
Ce sous-programme va s'occuper des appels de fonction des sous programme concernant une partie déjà commencé*/
void jeu( int n_perso, char niveau[40], int charge )
{
    //Déclaration des variable propre à une partie
    t_scroller* fond = NULL, *collision = NULL, *tabAttente = NULL ;
    t_perso* acteur = NULL ;
    t_collection* missiles = NULL ;
    t_ennemis * ennemis_collec = NULL ;
    t_explosions * feu = NULL ;
    t_objets* bonus = NULL ;
    int booleen = 1, nombre_de_perso[2], i = 0, k = 0, taille = 0 , score = 0 ;
    long int timer = 0, retard = 0, debut = clock(), temps_pause = - 200 ;
    float screenx = 0 , screenfin = 0 ;
    BITMAP* buffer = create_bitmap( SCREEN_W, SCREEN_H ) ;
    BITMAP * buffer_collision = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP * objet = create_bitmap(SCREEN_W+200, SCREEN_H);
    BITMAP * barrebas = create_bitmap(SCREEN_W, 100);
    BITMAP * fin = NULL ;
    //réccupération des données concernant le niveau
    tabAttente = chargement_niveau( niveau, &taille );
    //initialisaton des variables
    initialisation( &fond, &acteur, n_perso, &missiles, &collision, & ennemis_collec, nombre_de_perso, &feu, &bonus, tabAttente, &k, taille, niveau ) ;
    //chargement à partir d'une sauvegarde
    if( charge == 1)
        lectureJeu("sauvegarde1.txt", n_perso, niveau, fond, collision, acteur);
    if( charge == 2)
        lectureJeu("sauvegarde2.txt", n_perso, niveau, fond, collision, acteur);
    if( charge == 3)
        lectureJeu("sauvegarde3.txt", n_perso, niveau, fond, collision, acteur);
    //chargement au boss final
    if(charge == 4)
    {
        fond[1].x = 7100;
        collision->x = 7100;
    }
    //Début boucle jeu
    while( ! key[KEY_ESC] && booleen !=2 &&  nombre_de_perso[1] > nombre_de_perso[0] && tabAttente != NULL && (( nombre_de_perso[0] == 0 && acteur[0].base.x < SCREEN_W ) || ( nombre_de_perso[1] == 2 && acteur[1].base.x < SCREEN_W )))
    {
        //si bouleen == 1 alors nous ne sommes pas en pause
        //effectue les actions concernant le jeu (joueur/ennemis/missiles...
        if( booleen == 1)
        {
            clear(buffer);
            //récupération du timer de la partie
            timer = clock() - debut ;
            //affichage et déplacement du fond
            scrolling( fond, buffer ) ; /*action/affichage du scroller*/
            scrolling_collision(collision, buffer_collision);
            //Action des différents acteurs :
            //deplacement
            //tir
            //collision
            //affichage
            action_tir( missiles, buffer, buffer_collision, collision );
            action_ennemi( ennemis_collec, buffer, fond, buffer_collision, acteur, nombre_de_perso ) ;
            action_objet( bonus, buffer, objet , collision );
            action_joueur( acteur, buffer, nombre_de_perso, buffer_collision, collision, ennemis_collec, objet, k );     /*actions mener par le joueur*/
            action_explosions( feu, buffer, collision );
            //detection si dans la sauvegarde il y a un ajout d'un nouveau corps
            //ennemis
            //objets
            nouveau_corps_sur_screen( ennemis_collec, bonus, tabAttente, missiles, feu, taille, &k, fond, acteur, n_perso ) ;
            screenfin = collision->fin ;
            screenx = collision->x ;
            score = (screenx/screenfin) * 1000 + acteur[0].score * 10 ;
            //affichage de la barre du bas relatives au joueurs et au temps de jeu
            AffichageBarreBas(acteur, barrebas, buffer, k, n_perso, timer - retard, collision , score );
            //affichage de la barre du bas relatives au joueurs et au temps de jeu
            //AffichageBarreBas(acteur, barrebas, buffer, k, n_perso, timer - retard, collision);
            textprintf_ex(buffer,font,600, 650,-1,-1,"x : %d y : %d",acteur->base.x+fond[1].x + (acteur->base.image[0]->w/2), acteur->base.y + acteur->base.image[0]->h);
            // blit(buffer_collision,buffer,0,0,SCREEN_W/2,0,SCREEN_W/2,SCREEN_H);
            blit( buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H ) ;
            //delai en fonction de la clock par rapport au début de la boucle et non indépendent du jeu
            //permet de joueur à la même vitesse sur les différents ordinateurs
            while( timer + debut + 25 > clock() );
        }
        //Début de la pause
        if( key[KEY_P] && temps_pause + 200 < clock() )
        {
            temps_pause = clock() ;
            //Affichage menu pause
            MenuPause(&booleen, n_perso, niveau, collision, acteur);
            //récupère le retard de la pause pour faire attention au timer et ne pas avvoir de décalages
            retard += temps_pause - debut ;
            retard -= timer ;
            //evite que le joueurs ne tir sans l'action du gamer
            for( i = nombre_de_perso[0] ; i < nombre_de_perso[1] ; i++)
                acteur[i].chargement = clock();
        }
        //vérification des condition de l'état du joueur
        //mort
        //bonus
        for( i = nombre_de_perso[0] ; i < nombre_de_perso[1] ; i++)
        {
            if( acteur[i].pod != NULL && acteur[i].pod->force != 0 && acteur[i].pod->force != 1 && acteur[i].pod->force != 2 && acteur[i].pod->force != 3 && acteur[i].pod->force != 4 )
            {
                free( acteur[i].pod ) ;
                acteur[i].pod = NULL ;
            }
            if(acteur[i].base.x + acteur[i].base.image[0]->w / 2 < 0 || acteur[i].base.vie <= 0 )
            {
                if( acteur[i].pod != NULL )
                {
                    free(acteur[i].pod);
                    acteur[i].pod = NULL ;
                }
                if( nombre_de_perso[1] - nombre_de_perso[0] == 1 )
                {
                    fond[0].x = acteur[i].check[0] ;
                    fond[1].x = acteur[i].check[0] ;
                    collision->x = acteur[i].check[0] ;
                    acteur[0].score = acteur[i].check[2] ;
                    k = 3 ;
                    //libère tout l'espace pris par les ennemis, les missiles
                    suppression_de_tout_les_explosions( acteur[i].explo ) ;
                    suppression_de_tout_les_tirs( missiles->premier, missiles ) ;
                    suppression_tous_ennemis( &ennemis_collec->premier_boss, &ennemis_collec->dernier_boss ) ;
                    suppression_tous_ennemis( &ennemis_collec->premier_fixe, &ennemis_collec->dernier_fixe ) ;
                    suppression_tous_ennemis( &ennemis_collec->premier_mouvant, &ennemis_collec->dernier_mouvant  ) ;
                    suppression_de_tout_les_objets( bonus ) ;
                    //réinitialise les paramètres du joueur et retour au checkpoint
                    chargement_perso( &acteur[i], acteur[i].ancre, acteur[i].nb_de_vie-1, acteur->explo, k, bonus, nombre_de_perso, i, acteur[i].score, niveau, ennemis_collec ) ;
                    acteur[i].base.y = acteur[i].check[1] ;
                }
                else
                {
                    //réinitialise les paramètres du joueur
                    //prends les mêmes coordonées que son partenaire pour éviter l'appartion dans un décor
                    chargement_perso( &acteur[i], acteur[i].ancre, acteur[i].nb_de_vie-1, acteur->explo, k, bonus, nombre_de_perso, i, acteur[i].score, niveau, ennemis_collec ) ;
                    if( i == 0 )
                    {
                        acteur[0].base.y = acteur[1].base.y ;
                        acteur[0].base.x = acteur[1].base.x ;
                    }
                    else
                    {
                        acteur[1].base.y = acteur[0].base.y ;
                        acteur[1].base.x = acteur[0].base.x ;
                    }
                }
            }
        }
        //si le joueur n'a plus de vie empêcher ses actions
        //plus d'influences sur le cours du jeu
        if( nombre_de_perso[0] == 0 && acteur[0].nb_de_vie <= 0 )
            nombre_de_perso[0]++ ;
        if( nombre_de_perso[1] == 2 && acteur[1].nb_de_vie <= 0 )
            nombre_de_perso[1]-- ;
    }
    clear_bitmap(barrebas);
    blit(barrebas, screen, 0,0,0,600,800,100);
    ///avant vider memoire, condition game over
    if(booleen == 1 && !key[KEY_ESC])
    {
        if(nombre_de_perso[1] > nombre_de_perso[0])
        {
            if(strcmp(niveau, "niveau3.txt") == 0)
                FonduFin(buffer, "fin.bmp");
            else
                FonduFin(buffer, "win.bmp");
        }
        else
            FonduFin(buffer, "gameover.bmp");
        //rest(800);
        temps_pause = clock() ;
        while(!key[KEY_ESC] && temps_pause + 4000 > clock() );
    }
    //libération des allocation dynamique de la mémoire
    free(fin);
    vider_memoire( ennemis_collec, missiles, acteur, n_perso, fond, collision, buffer, buffer_collision, taille, tabAttente, barrebas ) ;
    while(key[KEY_ESC] || mouse_b&1);
}

/**Programme principal où se trouve toute les initialisations des fonctions allegro
Appelle de fonction d'affichage du menu principale*/
int main()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    srand(time(NULL)) ;
    set_color_depth(desktop_color_depth());
    //déclaration de variable/initialisation
    BITMAP * buffer, *font[3] ;
    int sortie = 0, joueur = 0, niveau = 0, charge = 0;
    int chargement[3][2] ;
    int tabchoix[7] = {1,1,0,1,0,1,0};
    //0 : commencer une partie
    //1 : options220,270
    //2 : nb de joueur
    //3 : reprendre une partie
    //4 : niveau 1-2-3
    //5 : quitter
    //6 : choix de chargement de la partie
    //premier : le numero de la sauvegarde
    //deuxième 0 : niveau, 1 : nombre de perso
    //initialisation fenêtre graphique
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,700,0,0)!=0)
    {
        allegro_message("Probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    buffer = create_bitmap(800, 600);
    font[0] = load_bitmap("fond1.bmp", NULL);
    font[1] = load_bitmap("fond2.bmp", NULL);
    font[2] = load_bitmap("fond3.bmp", NULL);
    //Récupère les données niveau et joueur des sauvegarde
    lectureMenu("sauvegarde1.txt", &chargement[0][0], &chargement[0][1]);
    lectureMenu("sauvegarde2.txt", &chargement[1][0], &chargement[1][1]);
    lectureMenu("sauvegarde3.txt", &chargement[2][0], &chargement[2][1]);
    //affichage menu principal
    while(!key[KEY_ESC] && sortie == 0 )
    {
        clear(buffer);
        //affichage fond
        blit(font[0], buffer, 750,0,0,0,font[0]->w, font[0]->h);
        //affichage cases choix
        AffichageSauvegarde(buffer, tabchoix,&charge, chargement);
        AffichageNouvPart(buffer, tabchoix);
        AffichageChargerSauv(buffer, tabchoix);
        AffichageQuitter(buffer, tabchoix, &sortie);
        AffichageNiveau(buffer, tabchoix, &niveau, font);
        AffichageOptions(buffer, tabchoix, &charge);
        AffichageNbJoueur(buffer, tabchoix, &joueur);
        //Récupération de sauvegarde si "Charger Sauvegarde"
        if(charge != 0 && charge != 4)
        {
            niveau = chargement[charge-1][0];
            joueur = chargement[charge-1][1];
        }
        //Accès direct au boss final
        if(charge == 4)
        {
            niveau = 3 ;
            joueur = 1 ;
        }
        //Si choix de niveau donc de nombre de joueur : commencer partie
        if(niveau != 0)
        {
            if(niveau == 1)
                jeu(joueur, "niveau1.txt", charge);
            if(niveau == 2)
                jeu(joueur, "niveau2.txt", charge);
            if(niveau == 3)
                jeu(joueur, "niveau3.txt", charge);
            MiseAZeroMenu(tabchoix, &joueur, & niveau, & charge, chargement);
            //Fin de parti retour au menu principal avec choix de :
            //Commencer une nouvelle partie
            //Charger une sauvegarde
            //Option
            //Quitter
        }
        //Si selection du soleil allors remise à zero du menu
        if( mouse_x > 740 && mouse_x < 780 && mouse_y > 20 && mouse_y < 60 )
        {
            circlefill(buffer, 760, 40, 20, makecol(255,242,0));
            if(mouse_b&1)
            {
                MiseAZeroMenu(tabchoix, &joueur, &niveau, & charge, chargement);
            }
        }
        else
            circlefill(buffer, 760, 40, 20, makecol(255,201,14));
        //show_mouse(buffer) ;
        draw_sprite( buffer, mouse_sprite, mouse_x, mouse_y );
        blit(buffer, screen, 0,0,0,0, buffer->w,buffer->h);
    }
    return 0;
}
END_OF_MAIN();
