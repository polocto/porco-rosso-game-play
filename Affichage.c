#include "biblio.h"
/**reçoit en paramètre les infos sur l'acteur et l'affiche sur la bitmap collision.
Reçoit la partie commune de l'acteur et la BITMAP de collision. */
void affichage_bitmap_collision( t_commun* acteur, BITMAP* collision)
{
    int x = 0, y = 0 ;
    for( y = acteur->y ; y < acteur->y + acteur->image[0]->h ; y ++ )
        for( x = acteur->x ; x < acteur->x + acteur->image[0]->w ; x ++ )
            if( getpixel(acteur->image[0], x - acteur->x, y - acteur->y) != makecol( 255, 0,255 ))
                putpixel( collision, x, y, acteur->c );
}
/**Permet l'affichage dynamique des oiseau en mouvement
Reçoit la structure de l'oiseau à afficher, la BITMAP du buffer et la BITMAP de collision.*/
void affichage_oiseau( t_ennemi* mechant, BITMAP* buffer, BITMAP* collision )
{
    int i = 0 ;
    while( mechant != NULL )
    {
        mechant->affichage+=40;
        switch( mechant->affichage )
        {
        case 0 ... 80 :
            i = 0 ;
            break ;
        case 81 ... 160 :
            i = 1 ;
            break ;
        case 161 ... 240 :
            i = 2 ;
            break ;
        case 241 ... 320 :
            i = 3 ;
            break ;
        case 321 ... 400 :
            i = 4 ;
            break ;
        case 401 ... 480 :
            i = 5 ;
            break ;
        case 481 ... 560 :
            i = 6 ;
            break ;
        case 561 ... 640 :
            i = 7 ;
            break ;
        default :
            mechant->affichage = 0 ;
            i = 0 ;
            break ;
        }
        //affichage sur bitmap collision
        affichage_bitmap_collision(&mechant->base,collision);
        //affichage sur le buffer interactif avec le joueur
        draw_sprite( buffer, mechant->base.image[i], mechant->base.x, mechant->base.y ) ;
        mechant = mechant->suivant ;
    }
}
/**Permet l'affichage des ennemis sur le scroller.
Reçoit le premier maillon des ennemis, la BIMAP buffer et la BITMAP de collision.*/
void affichage_ennemis(t_ennemi* courant, BITMAP * buffer, BITMAP* collision )
{
    while(courant != NULL)
    {
        //affichage sur le buffer interactif avec le joueur
        draw_sprite(buffer,courant->base.image[0], courant->base.x, courant->base.y);
        //affichage sur bitmap collision
        affichage_bitmap_collision(&courant->base,collision);
        courant = courant->suivant ;
    }
}
/**Affichage des fonds avec mouvement du décors.
Reçoit le tableau de scrolleur, la BITMAP buffer.*/
void scrolling( t_scroller* fond, BITMAP* buffer)
{
    for(int i = 0 ; i< 2 ; i++)
    {
        //deplacement jusqu'à fin du niveau
        if(fond[1].fin - fond[1].x >= buffer->w + fond[1].dx )
            fond[i].x += fond[i].dx;
        //affichage fond
        masked_blit( fond[i].image, buffer, (fond[i].x%(fond[i].image->w+1)), fond[i].y, 0, 0, SCREEN_W, SCREEN_H );
        //complete l'image du scroller quand elle arrive au bout
        if( fond[i].image->w - fond[i].x < SCREEN_W )
            masked_blit( fond[i].image, buffer, 0, 0, fond[i].image->w - (fond[i].x%(fond[i].image->w+1)), fond[i].y, SCREEN_W, SCREEN_H );
    }
}
/**Scrolling du fond de collision.
Reçoit la structure scroller de la BITMAP de collision, et la BITMAP buffer collision.*/
void scrolling_collision( t_scroller* collision, BITMAP* buffer_collision)
{
    //deplacement jusqu'à fin du niveau
    if(collision->fin - collision->x >= buffer_collision->w + collision->dx )
        collision->x += collision->dx ;
    //affichage fond
    blit( collision->image, buffer_collision, (collision->x%(collision->image->w+1)), collision->y, 0, 0, SCREEN_W, SCREEN_H );
    //complete l'image du scroller quand elle arrive au bout
    if( collision->image->w - collision->x < SCREEN_W )
        blit( collision->image, buffer_collision, 0, 0, collision->image->w - (collision->x%(collision->image->w+1)), collision->y, SCREEN_W, SCREEN_H );
}
/**Affichage des tirs.
Reçoit l'ancre de la liste chaînée des tirs, la BITMAP buffer et la BITMAP collision.*/
void AffichageTirs(t_collection * ancre, BITMAP*buffer, BITMAP * collision)
{
    t_munition * courant = ancre->premier ;
    float dx = 0, dy = 0, phase = 0 ;
    while(courant != NULL)
    {
        phase = 0 ;
        if( courant->base.dy != 0 )
        {
            dx = courant->base.dx * 1.0000000 ;
            dy = courant->base.dy * 1.0000000 ;
            //calcul de l'angle entre les abscisses et les ordonnées du tir
            phase = atan(dy/dx);
        }
        //sens de l'image du tir selon le tireur
        //affichage orientation du tir
        if( courant->base.c != makecol(0,0,255))
            rotate_sprite(buffer, courant->base.image[0], courant->base.x, courant->base.y, ftofix((phase*128.00)/M_PI));
        else
            rotate_sprite(buffer, courant->base.image[0], courant->base.x, courant->base.y, ftofix(((phase + M_PI)*128.00)/M_PI));
        //affichage sur bitmap collision
        affichage_bitmap_collision(&courant->base,collision);
        courant = courant->suivant ;
    }
}
/**Affichage des explosions.
Reçoit l'ancre de la liste chaînée des explosions et la BITMAP buffer. */
void AffichageExplosions( t_explosions* ancre, BITMAP* buffer )
{
    t_explo* courant = ancre->premier ;
    while(courant != NULL )
    {
        //si temps d'explosion < 300 ms
        //afficher
        //sinon
        //suppression du missiles
        /*if( courant->time+300 > clock() )
        {
            draw_sprite( buffer, courant->base.image[0], courant->base.x, courant->base.y );
        }
        else
        {
            suppression_explosion( ancre, courant ) ;
        }
        */if(courant->time<=23)
            draw_sprite( buffer, courant->base.image[(courant->time/3)], courant->base.x, courant->base.y );
        else
            suppression_explosion( ancre, courant ) ;
        courant->time ++ ;
        courant = courant->suivant;
    }
}
/**Affichage du type de bonus dans la barre du bas.
Reçoit la BITMAP de la barre du bas, les coordonées x, y du positionnement et le type de bonus à afficher.*/
void AffichageTypePod(BITMAP * barrebas, int x, int y, int type)
{
    switch(type)
    {
    case 0 :
        textprintf_ex(barrebas, font, x, y, makecol(255,255,255), -1, "Bouclier");
        break ;
    case 1 :
        textprintf_ex(barrebas, font, x, y, makecol(255,255,255), -1, "Tri-tir");
        break ;
    case 2 :
        textprintf_ex(barrebas, font, x, y, makecol(255,255,255), -1, "Tir puissant");
        break ;
    case 3 :
        textprintf_ex(barrebas, font, x, y, makecol(255,255,255), -1, "Tir rafale");
        break ;
    }
}
/**Affichage barre du bas de l'etat du perso.
Reçoit la structure du perso, la BITMAP barre du bas, la BITMAP du buffer, le nombre de perso,
le temps, la structure de scroller et le score.
On aura pris en compte les paramètres en const pour ne pas avoir de modification possible. */
void AffichageBarreBas( t_perso * acteur, BITMAP * barrebas, BITMAP * buffer,const int k,const int nombreperso, long int timer, const t_scroller * scrol, const int score )
{
    long int minute = timer/60000, seconde = (timer%60000)/1000, milliseconde = timer % 1000 ;
    float screenx = scrol->x, fin = scrol->fin - SCREEN_W ;
    //on nettoie la bitmap de barrebas
    clear_bitmap(barrebas);
    //s'il n'y a qu'un perso
    if(nombreperso == 1)
    {
        //aficchage des différentes données du joueur
        textprintf_centre_ex(barrebas, font, barrebas->w/2, 5, makecol(255,255,255),-1, "Joueur 1 :");
        textprintf_ex(barrebas, font, 50, 15, makecol(255,255,255),-1, "Score : %d", score);
        textprintf_ex(barrebas, font, 50, 30, makecol(255,255,255),-1, "Vies : %d", acteur[0].nb_de_vie);
        //affichage de la jauge de vie en barre
        textprintf_ex(barrebas, font, 300, 25, makecol(255,255,255), -1, "Jauge de vie");
        rect(barrebas, 450, 25, 510, 30, makecol(0,255,0));
        rectfill(barrebas, 450, 25, 450 + 3 * (acteur[0].base.vie), 30, makecol(0,255,0));
        //affichage du check point actif
        if(acteur[0].check[0] > 0)
            textprintf_ex(barrebas, font, 50, 45, makecol(255,255,255),-1, "Check Point actif");
        //affichage du type de bonus
        if(acteur[0].pod != 0)
        {
            AffichageTypePod(barrebas, 300, 40, acteur[0].pod->force);
            rect(barrebas, 450, 40, 550, 45, makecol(0,255,0));
            rectfill(barrebas, 450, 40, 550-100*(clock() - acteur[0].pod->temps)/10000,45,makecol(0,255,0));
        }
        //affichage du temps
        textprintf_centre_ex(barrebas,font,barrebas->w/2, 70,makecol(255,255,255),-1,"Timer : %ldmin %lds %ld", minute, seconde, milliseconde );
    }
    //si il y a deux joueurs
    else
    {
        if(acteur[0].nb_de_vie != 0)
        {
            //affichage des données du joueur
            textprintf_centre_ex(barrebas, font, barrebas->w/4, 5, makecol(255,255,255),-1, "Joueur 1 :");
            textprintf_ex(barrebas, font, 50, 15, makecol(255,255,255),-1, "Score : %d", score);
            textprintf_ex(barrebas, font, 50, 25, makecol(255,255,255),-1, "Vies : %d", acteur[0].nb_de_vie);
            //affichage de la jauge de vie en barre
            textprintf_ex(barrebas, font, 50, 35, makecol(255,255,255), -1, "Jauge de vie");
            rect(barrebas, 150, 35, 210, 40, makecol(0,255,0));
            rectfill(barrebas, 150, 35, 150 + 3 * (acteur[0].base.vie), 40, makecol(0,255,0));
            //affichage du checkpoint actif
            if(acteur[0].check[0] > 0)
                textprintf_ex(barrebas, font, 50, 45, makecol(255,255,255),-1, "Check Point actif");
            //affichage du type de bonus
            if(acteur[0].pod != 0 && acteur[0].pod->force != 0)
            {
                AffichageTypePod(barrebas, 50, 55, acteur[0].pod->force);
                rect(barrebas, 150, 55, 250, 60, makecol(0,255,0));
                rectfill(barrebas, 150, 55, 250-100*(clock() - acteur[0].pod->temps)/10000,60,makecol(0,255,0));
            }
        }
        //si le joueur 1 est mort
        else
            textprintf_centre_ex(barrebas, font, barrebas->w/4, 5, makecol(255,255,255),-1, "Game Over Joueur 1");
        if(acteur[1].nb_de_vie != 0)
        {
            //affichage des données du joueur
            textprintf_centre_ex(barrebas, font, 3 * (barrebas->w)/4, 5, makecol(255,255,255),-1, "Joueur 2 :");
            textprintf_ex(barrebas, font, 400, 15, makecol(255,255,255),-1, "Score : %d", score );
            textprintf_ex(barrebas, font, 400, 25, makecol(255,255,255),-1, "Vies : %d", acteur[1].nb_de_vie);
            //affichage de la vie en barre
            textprintf_ex(barrebas, font, 400, 35, makecol(255,255,255), -1, "Jauge de vie");
            rect(barrebas, 500, 35, 560, 40, makecol(0,255,0));
            rectfill(barrebas, 500, 35, 500 + 3 * (acteur[1].base.vie), 40, makecol(0,255,0));
            //affichage du check point actif
            if(acteur[1].check[0] > 0)
                textprintf_ex(barrebas, font, 400, 45, makecol(255,255,255),-1, "Check Point actif");
            //affichage du bonus
            if(acteur[1].pod != 0 && acteur[1].pod->force != 0)
            {
                rect(barrebas, 500, 55, 600, 60, makecol(0,255,0));
                rectfill(barrebas, 500, 55, 600-100*(clock() - acteur[1].pod->temps)/10000,60,makecol(0,255,0));
            }
        }
        //si le deuxième joueur est mort
        else
            textprintf_centre_ex(barrebas, font, 3 * (barrebas->w)/4, 5, makecol(255,255,255),-1, "Game Over joueur 2");
        //affichage du timer à la position deux joueurs
        textprintf_centre_ex(barrebas,font,barrebas->w/2, 70,makecol(255,255,255),-1,"Timer : %ldmin %lds %ld", minute, seconde, milliseconde );
    }
    //barre de progression du niveau
    rect(barrebas, 10, 90, (SCREEN_W - 10 ), 95, makecol(100,100,255));
    rectfill(barrebas, 10, 90, 10 + (SCREEN_W - 20 )*(screenx/fin), 95,makecol(100,100,255));
    //on affiche la barre du bas sur le buffer
    blit(barrebas, buffer, 0, 0, 0, 600, barrebas->w, barrebas->h);
}
/**Affichage des bonus
Reçoit l'ancre de la liste chaînée objet, la BITMAP buffer, la BITMAP bonus*/
void AffichageObjet( t_objet* courant, BITMAP* buffer, BITMAP* bonus )
{
    //affichage interactif
    draw_sprite(buffer, courant->base.image[0], courant->base.x, courant->base.y);
    //affichage pour collision
    rectfill( bonus, courant->base.x + 100, courant->base.y, courant->base.x + courant->base.image[0]->w + 100, courant->base.y + courant->base.image[0]->h, courant->base.c );
}
/**Menu s'affichant lors de la mise en pause du jeu
Reçoit le paramètre de pause, le nombre de joueur, le nom du fichier texte du niveau, les paramètres du scroller
les paramètre du perso.
Ce sous programme changera la valeur du booléen en fonction de l'action voulue : quitter ou continuer.*/
void MenuPause(int * booleen, int nombrejoueur, char niveau[40], t_scroller * collision, t_perso * acteur)
{
    *booleen = 0;
    BITMAP * bpause = NULL ;
    bpause = create_bitmap(400,300);
    //tant que l'action reste en pause
    while( *booleen == 0)
    {
        clear_bitmap(bpause);
        //Quitter
        if((mouse_x - 200 > 100 && mouse_x - 200 < 300)&&(mouse_y - 100 > 50 && mouse_y - 100 < 100))
        {
            rectfill(bpause, 100, 50, 300, 100, makecol(255,255,255));
            if(mouse_b&1)
                //si on selectionne l'action le booléen passe à deux, on quittera la partie
                *booleen = 2 ;
        }
        else
            rectfill(bpause, 100, 50, 300, 100, makecol(112,146,190));
        textprintf_centre_ex(bpause, font, bpause->w / 2,73, makecol(0,0,0), -1, "Quitter");
        //sauvegarder
        if((mouse_x - 200 > 100 && mouse_x - 200 < 300)&&(mouse_y - 100 > 125 && mouse_y - 100 < 175))
        {
            rectfill(bpause, 100, 125, 300, 175, makecol(255,255,255));
            if(mouse_b&1)
            {
                //si on selectionne l'action on laisse le choix de l'amplacement de la sauvegarde
                while(mouse_b&1);
                AffichageChoixSauvegarde(nombrejoueur, niveau, collision, acteur, bpause);
                //le booléen passe à 2 pour quitter la partie
                while(mouse_b&1);
                *booleen = 2 ;
            }
        }
        else
            rectfill(bpause, 100, 125, 300, 175, makecol(112,146,190));
        textprintf_centre_ex(bpause, font, bpause->w / 2, 148, makecol(0,0,0), -1, "Quitter et sauvegarder");
        //reprendre la partie
        if((mouse_x - 200 > 100 && mouse_x - 200 < 300)&&(mouse_y - 100 > 200 && mouse_y - 100 < 250))
        {
            rectfill(bpause, 100, 200, 300, 250, makecol(255,255,255));
            //si on selectionne l'action le booléen passe à 1, on reprend la partie où elle en était
            if(mouse_b&1)
                *booleen = 1 ;
        }
        else
            rectfill(bpause, 100, 200, 300, 250, makecol(112,146,190));
        textprintf_centre_ex(bpause, font, bpause->w / 2, 223, makecol(0,0,0), -1, "Reprendre la partie");
        //Affichage de la souris
        draw_sprite( bpause, mouse_sprite, mouse_x - 200, mouse_y - 100 );
        blit(bpause, screen, 0,0,200, 100, 400, 300);
    }
}
/**Affichage du menu "sauvegarder et quitter"
Reçoit le nombre de joueur, le nom du fichier texte de niveau, les paramètres du scroller de collision,
les paramètres du perso, la BITMAP de pause.*/
void AffichageChoixSauvegarde( int nomrejoueur, char niveau [40], t_scroller * collision, t_perso * acteur, BITMAP * bpause)
{
    //ouvrir les trois
    //si un vide sauvegarder dedans
    //sinon donner le choix à l'utilisateur de raser une partie
    int chargement[3][2];
    FILE * save1, * save2, *save3 ;
    save1 = fopen("sauvegarde1.txt", "r");
    save2 = fopen("sauvegarde2.txt", "r");
    save3 = fopen("sauvegarde3.txt", "r");
    clear_bitmap(bpause);
    if(save1 != NULL && save2 != NULL && save3 != NULL)
    {
        //regarde tous les fichiers à la recherche d'un vide, mémorise niveau et joueur
        lectureMenu("sauvegarde1.txt", &chargement[0][0], &chargement[0][1]);
        if(chargement[0][0] == 0)
        {
            sauvegarder("sauvegarde1.txt", nomrejoueur, niveau, collision, acteur);
            fclose(save1);
            fclose(save2);
            fclose(save3);
            return ;
        }
        lectureMenu("sauvegarde2.txt", &chargement[1][0], &chargement[1][1]);
        if(chargement[1][0] == 0)
        {
            sauvegarder("sauvegarde2.txt", nomrejoueur, niveau, collision, acteur);
            fclose(save1);
            fclose(save2);
            fclose(save3);
            return ;
        }
        lectureMenu("sauvegarde3.txt", &chargement[2][0], &chargement[2][1]);
        if(chargement[2][0] == 0)
        {
            sauvegarder("sauvegarde3.txt", nomrejoueur, niveau, collision, acteur);
            fclose(save1);
            fclose(save2);
            fclose(save3);
            return ;
        }
    }
    else
        probleme_de_memoire();
    fclose(save1);
    fclose(save2);
    fclose(save3);
    //on affiche les 3 avec leur categorie
    //celui sur lequel on clique désigne là où on sauvegarde
    //possiblité également de ne pas sauvegarder
    while( !key[KEY_ESC])
    {
        clear_bitmap(bpause);
        if((mouse_x /*- 200 */> 100 && mouse_x /*- 200*/ < 300)&&(mouse_y /*- 100*/ > 20 && mouse_y /*- 100*/ < 70))
        {
            rectfill(bpause, 100, 20, 300, 70, makecol(255,255,255));
            if(mouse_b&1)
            {
                sauvegarder("sauvegarde1.txt", nomrejoueur, niveau, collision, acteur);
                return;
            }
        }
        else
            rectfill(bpause, 100, 20, 300, 70, makecol(112,146,190));
        textprintf_centre_ex(bpause, font, bpause->w / 2,42, makecol(0,0,0), -1, "Niveau : %d - Joueur : %d", chargement[0][0], chargement[0][1]);
        if((mouse_x /*- 200*/ > 100 && mouse_x /*- 200*/ < 300)&&(mouse_y /*- 100*/ > 90 && mouse_y /*- 100 */< 140))
        {
            rectfill(bpause, 100, 90, 300, 140, makecol(255,255,255));
            if(mouse_b&1)
            {
                sauvegarder("sauvegarde2.txt", nomrejoueur, niveau, collision, acteur);
                return;
            }
        }
        else
            rectfill(bpause, 100, 90, 300, 140, makecol(112,146,190));
        textprintf_centre_ex(bpause, font, bpause->w / 2,112, makecol(0,0,0), -1, "Niveau : %d - Joueur : %d", chargement[1][0], chargement[1][1]);
        if((mouse_x /*- 200*/ > 100 && mouse_x /*- 200 */< 300)&&(mouse_y /*- 100 */> 160 && mouse_y /*- 100*/ < 210))
        {
            rectfill(bpause, 100, 160, 300, 210, makecol(255,255,255));
            if(mouse_b&1)
            {
                sauvegarder("sauvegarde3.txt", nomrejoueur, niveau, collision, acteur);
                return;
            }
        }
        else
            rectfill(bpause, 100, 160, 300, 210, makecol(112,146,190));
        textprintf_centre_ex(bpause, font, bpause->w / 2,182, makecol(0,0,0), -1, "Niveau : %d - Joueur : %d", chargement[2][0], chargement[2][1]);
        if((mouse_x /*- 200*/ > 100 && mouse_x /*- 200 */< 300)&&(mouse_y /*- 100*/ > 230 && mouse_y /*- 100*/ < 280))
        {
            rectfill(bpause, 100, 230, 300, 280, makecol(255,255,255));
            if(mouse_b&1)
                return;
        }
        else
            rectfill(bpause, 100, 230, 300, 280, makecol(112,146,190));
        textprintf_centre_ex(bpause, font, bpause->w / 2,252, makecol(0,0,0), -1, "Ne pas sauvegarder");

        draw_sprite( bpause, mouse_sprite, mouse_x, mouse_y );
        blit(bpause, screen, 0,0,200, 100, 400, 300);
    }
}
/**Sous programme d'affichage du message de fin
avec un fondu sur l'écran entre la denrière scène du jeu et le message.
Reçoit la BITMAP avant fondu et le nom de la BITMAP voulue après fondu. */
void FonduFin( BITMAP * debut, const char nom[40])
{
    BITMAP * buffer = create_bitmap(800,600);
    BITMAP * fin = load_bitmap(nom, NULL);
    int x = 0, y = 0, pfin, pdebut, rfin, bfin, gfin, rdebut, bdebut, gdebut, rdest, bdest, gdest ;
    float varfondu = 0 ;
    //on limite le fondu à 1 pour ne pas aller dans des couleurs non voulues
    while(varfondu <= 1.05)
    {
        //on fait varier le fondu pour chaque pixel
        for(y = 0 ; y < fin->h ; y++)
        {
            for(x = 0 ; x < fin->w ; x++)
            {
                pfin = getpixel(fin, x, y);
                pdebut = getpixel(debut, x, y);
                //recupération des données après fondu
                rfin =getr(pfin);
                bfin =getb(pfin);
                gfin =getg(pfin);
                //récupération des données avant fondues
                rdebut = getr(pdebut);
                bdebut = getb(pdebut);
                gdebut = getg(pdebut);
                //on détermine les nouvelles données
                rdest = (1.0-varfondu)*rdebut + varfondu*rfin ;
                gdest = (1.0-varfondu)*gdebut + varfondu*gfin ;
                bdest = (1.0-varfondu)*bdebut + varfondu*bfin ;
                //on applique la couleur du pixel voulu
                putpixel(buffer, x, y, makecol(rdest, gdest, bdest));
            }
        }
        //une fois chaque pixel remplacé dans le buffer on l'affiche
        blit(buffer, screen, 0,0,0,0,800,600);
        varfondu += 0.05;
    }
}
/**ce programme permet la réinitialisation du menu principale*/
void MiseAZeroMenu(int tabchoix[7], int *joueur, int * niveau, int * charge, int chargement[3][2])
{
    tabchoix[0] = 1 ;
    tabchoix[1] = 1 ;
    tabchoix[2] = 0 ;
    tabchoix[3] = 1 ;
    tabchoix[4] = 0 ;
    tabchoix[5] = 1 ;
    *joueur = 0 ;
    *niveau = 0 ;
    *charge  = 0 ;
    lectureMenu("sauvegarde1.txt", &chargement[0][0], &chargement[0][1]);
    lectureMenu("sauvegarde2.txt", &chargement[1][0], &chargement[1][1]);
    lectureMenu("sauvegarde3.txt", &chargement[2][0], &chargement[2][1]);
}
/**Ce sous programme permet en fonction de la position et que si l'on
fait un clique droit ou non
de choisir l'action adéquate*/
int Affichage(BITMAP * buffer, int tabchoix[7], int xg, int xd, int yh, int yb, int cas)
{
    if((mouse_x > xg && mouse_x < xd)&&(mouse_y > yh && mouse_y < yb))
    {
        rectfill(buffer, xg, yh, xd,yb, makecol(0,100,0)/2);
        if(cas == 4)
            rectfill(buffer, xg, yh, xd,yb, makecol(255,255,255));
        if(mouse_b&1)
        {
            while(mouse_b&1);
            switch(cas)
            {
            case 0 :
                tabchoix[2] = 1 ;
                tabchoix[0] = 2 ;
                tabchoix[1] = 0 ;
                tabchoix[3] = 0 ;
                tabchoix[5] = 0 ;
                break ;
            case 1 :
                tabchoix[0] = 0 ;
                tabchoix[1] = 2 ;
                tabchoix[3] = 0 ;
                tabchoix[5] = 0 ;
                return 1 ;
                break ;
            case 2 :
                tabchoix[4] = 1 ;
                tabchoix[2] = 2 ;
                return 1 ;
                break ;
            case 3 :
                tabchoix[0] = 0 ;
                tabchoix[1] = 0 ;
                tabchoix[3] = 2 ;
                tabchoix[5] = 0 ;
                break ;
            case 4 :
                return 1 ;
                break ;
            case 7 :
                return 1 ;
                break ;
            }
        }

    }
    else
        rectfill(buffer, xg, yh, xd, yb,makecol(0,100,0));
    return 0 ;
}
/**sous programme d'affichage du menu principale : "Commencer une nouvelle partie"*/
void AffichageNouvPart(BITMAP * buffer, int tabchoix[7])
{
    if(tabchoix[0] == 1 )
    {
        Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)+200, 150, 200, 0);
        textprintf_centre_ex(buffer, font, buffer->w/2, 172, makecol(255,255,255),-1, "Commencer une nouvelle partie" );
    }
    if(tabchoix[0] == 2)
    {
        rectfill(buffer, (buffer->w/2)-200, 150, (buffer->w/2)+200, 200, makecol(13,50,60));
        textprintf_centre_ex(buffer, font, buffer->w/2, 172, makecol(255,255,255),-1, "Commencer une nouvelle partie" );
    }
}
/**sous programme d'affichage du menu principale : "Charger une sauvegarde"*/
void AffichageChargerSauv(BITMAP * buffer, int tabchoix[7])
{
    if(tabchoix[3] == 1)
    {
        Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)+200, 220, 270, 3);
        textprintf_centre_ex(buffer, font, buffer->w/2, 242, makecol(255,255,255),-1, "Charger une sauvegarde" );
    }
    if(tabchoix[3] == 2)
    {
        rectfill(buffer, (buffer->w/2)-200, 150, (buffer->w/2)+200, 200, makecol(13,50,60));
        textprintf_centre_ex(buffer, font, buffer->w/2, 172, makecol(255,255,255),-1, "Charger une sauvegarde" );
    }
}
/**sous programme d'affichage du menu principale choix nombre de joueur 1 ou 2*/
void AffichageNbJoueur(BITMAP * buffer, int tabchoix[7], int * joueur)
{
    int retour = 1 ;
    if(tabchoix[2] == 1)
    {
        retour = Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)-10, 220,270, 2);
        textprintf_centre_ex(buffer, font, (buffer->w/2)-100, 242, makecol(255,255,255),-1, "1 joueur" );
        if(tabchoix[2] == 2 && retour == 1)
            *joueur = 1 ;

        retour = Affichage(buffer, tabchoix, (buffer->w/2)+10, (buffer->w/2)+200, 220,270, 2);
        textprintf_centre_ex(buffer, font, (buffer->w/2)+100, 242, makecol(255,255,255),-1, "2 joueurs" );
        if(tabchoix[2] == 2 && retour == 1)
            *joueur = 2 ;
    }
    if(tabchoix[2] == 2)
    {
        rectfill(buffer, (buffer->w/2)-200, 220, (buffer->w/2)-10, 270, makecol(13,50,60));
        textprintf_centre_ex(buffer, font, (buffer->w/2)-100, 242, makecol(255,255,255),-1, "1 joueur" );
        rectfill(buffer, (buffer->w/2)+10, 220, (buffer->w/2)+200, 270, makecol(13,50,60));
        textprintf_centre_ex(buffer, font, (buffer->w/2)+100, 242, makecol(255,255,255),-1, "2 joueurs" );
    }
}
/**sous programme d'affichage du menu principale : "Options"*/
void AffichageOptions(BITMAP * buffer, int tabchoix[7], int * charge)
{
    int retour = 0 ;
    if(tabchoix[1] == 1)
    {
        retour = Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)+200, 290, 340,1);
        textprintf_centre_ex(buffer, font, buffer->w/2, 312, makecol(255,255,255),-1, "Options" );
        if(retour == 1)
            *charge = 4 ;
    }
    if(tabchoix[1] == 2)
    {
        rectfill(buffer, (buffer->w/2)-200, 150, (buffer->w/2)+200, 200,makecol(13,50,60));
        textprintf_centre_ex(buffer, font, buffer->w/2, 172, makecol(255,255,255),-1, "Options" );
    }
}
/**sous programme d'affichage des différents niveau*/
void AffichageNiveau(BITMAP * buffer, int tabchoix[7], int * niveau, BITMAP * fond[3] )
{
    int retour = 0 ;
    if(tabchoix[4] == 1)
    {
        retour = Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)+200, 290,340, 4);
        stretch_blit(fond[0], buffer, 0,0, fond[0]->w, fond[0]->h, (buffer->w/2)-195, 295, 390, 40);
        textprintf_centre_ex(buffer, font, (buffer->w/2), 312, makecol(255,255,255),-1, "Niveau 1" );
        if(tabchoix[4] == 1 && retour == 1)
            *niveau = 1 ;

        retour = Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)+200, 360,410, 4);
        stretch_blit(fond[1], buffer, 0,0, fond[1]->w, fond[1]->h, (buffer->w/2)-195, 365, 390, 40);
        textprintf_centre_ex(buffer, font, (buffer->w/2), 382, makecol(255,255,255),-1, "Niveau 2" );
        if(tabchoix[4] == 1 && retour == 1)
            *niveau = 2 ;

        retour = Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)+200, 430,480, 4);
        stretch_blit(fond[2], buffer, 0,0, fond[2]->w, fond[2]->h, (buffer->w/2)-195, 435, 390, 40);
        textprintf_centre_ex(buffer, font, (buffer->w/2), 452, makecol(255,255,255),-1, "Niveau 3" );
        if(tabchoix[4] == 1 && retour == 1)
            *niveau = 3 ;
    }
}
/**sous programme d'affichage du menu principale : "Quitter"*/
void AffichageQuitter(BITMAP * buffer, int tabchoix[7], int * sortie)
{
    if(tabchoix[5] == 1)
    {
        if((mouse_x > (buffer->w/2)-200 && mouse_x < (buffer->w/2)+200) && (mouse_y > 360 && mouse_y <410))
        {
            rectfill( buffer, (buffer->w/2)-200, 360, (buffer->w/2)+200, 410, makecol(0,100,0)/2 );
            if(mouse_b&1)
                *sortie = 1 ;
        }
        else
            rectfill(buffer, (buffer->w/2)-200, 360, (buffer->w/2)+200, 410, makecol(0,100,0));

        textprintf_centre_ex(buffer, font, buffer->w/2, 382, makecol(255,255,255),-1, "Quitter" );
    }
}
/**sous programme d'affichage du menu "Charger une sauvegarde"
en proposant différentes sauvegardes aux choix*/
void AffichageSauvegarde(BITMAP * buffer, int tabchoix[7], int * charge, int chargement[3][2])
{
    int retour = 0 ;
    if(tabchoix[3] == 2)
    {
        if(chargement[0][0] != 0)
        {
            retour = Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)+200, 220,270, 7);
            textprintf_centre_ex(buffer, font, (buffer->w/2), 242, makecol(255,255,255),-1, "Niveau %d - %d Joueur", chargement[0][0], chargement[0][1]);
            if(retour == 1)
                *charge = 1 ;
        }
        if(chargement[1][0] != 0)
        {
            retour = Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)+200, 290,340, 7);
            textprintf_centre_ex(buffer, font, (buffer->w/2), 312, makecol(255,255,255),-1, "Niveau %d - %d Joueur", chargement[1][0], chargement[1][1]);
            if(retour == 1)
                *charge = 2 ;
        }
        if(chargement[2][0] != 0)
        {
            retour = Affichage(buffer, tabchoix, (buffer->w/2)-200, (buffer->w/2)+200, 360,410, 7);
            textprintf_centre_ex(buffer, font, (buffer->w/2), 382, makecol(255,255,255),-1, "Niveau %d - %d Joueur", chargement[2][0], chargement[2][1]);
            if(retour == 1)
                *charge = 3 ;
        }
    }
}
