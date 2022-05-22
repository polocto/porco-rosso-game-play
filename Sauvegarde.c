#include "biblio.h"
/**Sauvegarde la partie en cours dans un fichier texte au choix.
Reçoit le nom du fichier texte, le nombre d ejoueur, le nom du fichier texte du niveau,
les données du scroller, les données du perso. */
void sauvegarder(char nom[16], int nombre_joueur, char niveau[40], t_scroller * collision, t_perso * acteur )
{
    FILE * save ;
    int i = 0, nv = 0 ;
    if(strcmp(niveau, "niveau1.txt") == 0)
        nv = 1 ;
    if(strcmp(niveau, "niveau2.txt") == 0)
        nv = 2 ;
    if(strcmp(niveau, "niveau3.txt") == 0)
        nv = 3 ;
    save = fopen(nom, "w");
    if(save != NULL)
    {
        //Niveau
        fprintf(save, "%d\n", nv);
        //Nombre de joueur
        fprintf(save, "%d\n", nombre_joueur);
        //Avancement dans la BITMAP
        //fprintf(save, "%d\n", collision->x);
        //Donées joueur :
        for(i = 0 ; i<nombre_joueur; i++)
        {
            //position
            fprintf(save, "%d\n%d\n", acteur[i].check[0] , acteur[i].check[1]) ;//acteur[i].base.x, acteur[i].base.y);
            //Vie
            fprintf(save, "%d\n", acteur[i].nb_de_vie);
            //fprintf(save, "%d\n", acteur[i].base.vie);
            //Point
            fprintf(save, "%d\n", acteur[i].check[2]);
        }
    }
    else
        probleme_de_memoire();
    fclose(save);
}
/**Lecture des fichiers textes déstiné au menu.
Reçoit le nom du fichier à lire, un pointeur sur niveau et un pointeur sur le nombre de joueur. */
void lectureMenu(char nom[16], int * niveau, int * nb_joueur)
{
    FILE * save ;
    save = fopen(nom, "r");
    if(save != NULL)
    {
        //Niveau
        fscanf(save, "%d\n", niveau);
        //Nombre de joueur
        fscanf(save, "%d\n", nb_joueur);
    }
    else
        probleme_de_memoire();
    fclose(save);
}
/**Lecture des fichiers de sauvegarde déstiné au démarrage du jeu.
Reçoit le nom du fichier texte, le nombre de joueur, le nom du niveau,
la structure du scroller fond et du scroller collision, les données du perso. */
void lectureJeu(char nom[20], int nombre_joueur, char niveau[40], t_scroller * fond, t_scroller * collision, t_perso * acteur)
{
    FILE * save ;
    int i = 0, buf = 0 ;
    save = fopen(nom, "r");
    if(save != NULL)
    {
        fscanf(save, "%d\n%d\n", &buf, &buf);
        //Avancement dans la BITMAP
       // fscanf(save, "%d\n", &fond[1].x);
        collision->x = fond[1].x;
        //Donées joueur :
        for(i = 0 ; i<nombre_joueur ; i++)
        {
            //position
            fscanf(save, "%d\n", &acteur[i].check[0]);
            fond[0].x = acteur[i].check[0] ;
            fond[1].x = acteur[i].check[0] ;
            collision->x = acteur[i].check[0] ;
            fscanf(save, "%d\n", &acteur[i].check[1]);
            acteur[i].base.y = acteur[i].check[1] ;
            //Vie
            fscanf(save, "%d\n", &acteur[i].nb_de_vie);
            //fscanf(save, "%d\n", &acteur[i].base.vie) ;
            //Point
            fscanf(save, "%d\n", &acteur[i].check[2]);
            acteur[i].score = acteur[i].check[2] ;
        }
    }
    else
        probleme_de_memoire();
    fclose(save);
}
