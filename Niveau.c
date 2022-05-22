#include "biblio.h"
/**Gère le chargement des niveaux à partir d'un fichier texte, avec ses données.
Reçoit le nom du niveau et sa taille. Retourne un tableau de structure avec toutes les données du niveau.*/
t_scroller * chargement_niveau( char* niveau, int* taille )
{
    FILE* fichier = fopen(niveau, "r");
    int i = 0;
    t_scroller * tabAttente ;
    if(fichier == NULL)
    {
        allegro_message("Problème au chargement du niveau");
        fclose( fichier ) ;
        allegro_exit();
        exit(1) ;
    }
    fscanf(fichier, "%d\n", taille);
    tabAttente = (t_scroller*)malloc( (*taille) * sizeof(t_scroller) ) ;
    for(i = 0 ; i < *taille ; i++)
    {
        tabAttente[i].image = NULL ;
        fscanf(fichier, "%d\n", &tabAttente[i].x);
        fscanf(fichier, "%d\n", &tabAttente[i].y);
        fscanf(fichier, "%d\n", &tabAttente[i].taillex);
        fscanf(fichier, "%d\n", &tabAttente[i].tailley);
        fscanf(fichier, "%d\n", &tabAttente[i].dx);
        tabAttente[i].nom = (char*)malloc(tabAttente[i].dx*sizeof(char));
        fscanf(fichier, "%s\n", tabAttente[i].nom);
        fscanf(fichier, "%c\n", &tabAttente[i].type);
        tabAttente[i].dx = 0 ;
    }
    fclose( fichier ) ;
    return tabAttente ;
}
/**Determine où ajouter l'élément du tableau d'attente en fonction de son type.
Reçoit l'ancre des ennemis, l'ancre des bonus, l'ancre des missiles, l'ancre des explosions,
le tableau des scroller, le tableau des perso, le nombre de perso. */
void choix_type( t_ennemis* ennemis_collec, t_objets* bonus, t_collection* missiles, t_explosions* feu, t_scroller* tabAttente, t_scroller* fond, t_perso* acteur, int n_perso, int k )
{
    switch(tabAttente->type)
    {
    case 'D' :
        chargement_fond( fond, tabAttente->taillex, tabAttente->tailley, tabAttente->y, 0, tabAttente->x, tabAttente->nom ) ;
        break;
    case 'F' :
        Ajout_ennemi( &ennemis_collec->premier_fixe, &ennemis_collec->dernier_fixe, missiles, feu, tabAttente->x - fond[1].x, tabAttente->y, tabAttente->taillex, 0, tabAttente->nom, 1 );
        break;
    case 'M' :
        Ajout_ennemi( &ennemis_collec->premier_mouvant, &ennemis_collec->dernier_mouvant, missiles, feu, tabAttente->x - fond[1].x, tabAttente->y, tabAttente->taillex, tabAttente->tailley, tabAttente->nom, 8 );
        break;
    case 'B' :
        Ajout_ennemi( &ennemis_collec->premier_boss, &ennemis_collec->dernier_boss, missiles, feu, tabAttente->x - fond[1].x, tabAttente->y, tabAttente->taillex, tabAttente->tailley, tabAttente->nom, 1 );
        break;
    case 'V' :
        Ajout_objet( bonus, makecol(0,255,0), tabAttente->nom, tabAttente->x - fond[1].x, tabAttente->y, 5, /*vitesse scroller*/ fond[1].dx, 0 );
        break;
    ///Trois tirs
    case 'T' :
        Ajout_objet( bonus, makecol( 0, 0, 255 ), tabAttente->nom, tabAttente->x - fond[1].x, tabAttente->y, 0, /*vitesse scroller*/ fond[1].dx, 0) ;
        break;
    case 'R' :
        Ajout_objet( bonus, makecol(0,255,255), tabAttente->nom, tabAttente->x - fond[1].x, tabAttente->y, 0, /*vitesse scroller*/ fond[1].dx, 0 );
        break;
    case 'P' :
        Ajout_objet( bonus, makecol( 255, 255, 0 ), tabAttente->nom, tabAttente->x - fond[1].x, tabAttente->y, 0, /*vitesse scroller*/ fond[1].dx, 0) ;
        break;
    case 'S' :
        Ajout_objet( bonus, makecol( 255, 0, 0 ), tabAttente->nom, tabAttente->x - fond[1].x, tabAttente->y, 0, /*vitesse scroller*/ fond[1].dx, 0) ;
        break;
    case 'C' :
        Ajout_objet( bonus, makecol( 255, 255, 255 ), tabAttente->nom, tabAttente->x - fond[1].x, tabAttente->y, 0, /*vitesse scroller*/ fond[1].dx, 0) ;
        break ;
    }
}
