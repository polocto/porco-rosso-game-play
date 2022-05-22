#include "biblio.h"
/** Permet de supprimer un maillon de l'explosion une fois celui-ci effectué.
Reçoit l'ancre et le maillon à supprimer par paramètre.  */
void suppression_explosion( t_explosions* ancre, t_explo* suppression )
{
    t_explo* courant = ancre->premier ;
    t_explo* precedent = NULL ;
    //on parcours toute la liste chaînée jusqu'au maillon à supprimer,
    //on obitent ainsi l'adresse du maillon précédent
    while(courant != suppression)
    {
        precedent = courant ;
        courant = courant->suivant ;
    }
    //si le maillon à supprimer est le premier de la liste chaînée
    if( precedent == NULL )
    {
        ancre->premier = courant->suivant ;
        if( ancre->premier == NULL )
            ancre->dernier = NULL ;
    }
    else
    {
        //si le maillon à supprimer est le dernier de la lite chaînée
        if( ancre->dernier == courant )
            ancre->dernier = precedent ;
        precedent->suivant = courant->suivant ;
    }
    //on libère l'espace
    free(suppression->base.image[0]);
    free(suppression->base.image);
    free(suppression);
}
/**Condition de suppression des ennemis reçoit le premier et dernier*/
void condition_suppression_ennemi(  t_ennemi** premier, t_ennemi** dernier )
{
    t_ennemi* courant = *premier ;
    t_ennemi* supp = NULL ;
    while( courant != NULL)
    {
        if( courant->base.vie <= 0 || courant->base.x + courant->base.image[0]->w < 0 || courant->base.y > SCREEN_H || courant->base.y + courant->base.image[0]->h < 0 )
        {
            supp = courant ;
            courant = courant->suivant ;
            suppression_ennemi( premier, dernier, supp ) ;
        }
        else
            courant = courant->suivant ;
    }
}
/**Suppression des opposants avec en paramètre le premier et dernier maillon de la liste
ainsi que le maillon à supprimer*/
void suppression_ennemi( t_ennemi** premier, t_ennemi** dernier, t_ennemi* suppression )
{
    t_ennemi* courant = *premier ;
    t_ennemi* precedent = NULL ;
    //on parcours toute la liste chaînée jusqu'au maillon à supprimer,
    //on obitent ainsi l'adresse du maillon précédent
    while(courant != suppression)
    {
        precedent = courant ;
        courant = courant->suivant ;
    }
    if( precedent == NULL )
    {
        *premier = courant->suivant ;
        if( *premier == NULL )
            *dernier = NULL ;
    }
    else
    {
        if( *dernier == courant )
            *dernier = precedent ;
        precedent->suivant = courant->suivant ;
    }
    //on libère l'espace
    free(suppression->base.image[0]);
    free(suppression->base.image);
    free(suppression);
}
/**Supprime et libère un maillon de tir.
Reçoit le pointeur du maillon à supprimer, l'ancre de la liste chaîné des tirs. */
void SuppressionTir(t_munition * suppression, t_collection * ancre)
{
    //on retrouve et on réorganise les liaisons de la liste chainé
    t_munition * precedent = NULL, * courant  = ancre->premier ;
    while(courant != suppression)
    {
        precedent = courant ;
        courant = courant->suivant ;
    }
    if( precedent == NULL )
    {
        ancre->premier = courant->suivant ;
        if( ancre->premier == NULL )
            ancre->dernier = NULL ;
    }
    else
    {
        if( ancre->dernier == courant )
            ancre->dernier = precedent ;
        precedent->suivant = courant->suivant ;
    }
    //on libère l'espace
    free(suppression->base.image[0]);
    free(suppression->base.image);
    free(suppression);
}
/**Permet de supprimer tous les objets de la liste chaînée
Reçoit en paramètre l'ancre de celle-ci*/
void suppression_de_tout_les_objets( t_objets* ancre )
{
    t_objet* courant = ancre->premier, *supp = NULL ;
    while( courant!= NULL )
    {
        supp = courant ;
        courant = courant->suivant ;
        SuppressionObjet( supp, ancre );
    }
}
/**Permet de supprimer le maillon objet que l'on souhaite.
Reçoit en paramètre l'adresse du maillon à supprimer et l'ancre de la liste chaînée.*/
void SuppressionObjet(t_objet * suppression, t_objets * ancre)
{
    //on retrouve et on réorganise les liaisons de la liste chainé
    t_objet * precedent = NULL, * courant  = ancre->premier ;
    while(courant != suppression)
    {
        precedent = courant ;
        courant = courant->suivant ;
    }
    if( precedent == NULL )
    {
        ancre->premier = courant->suivant ;
        if( ancre->premier == NULL )
            ancre->dernier = NULL ;
    }
    else
    {
        if( ancre->dernier == courant )
            ancre->dernier = precedent ;
        precedent->suivant = courant->suivant ;
    }
    //on libère l'espace
    free(suppression->base.image[0]);
    free(suppression->base.image);
    free(suppression);
}
///Condition de suppression des ennemis reçoit le premier et dernier*/
void condition_suppression_objet( t_objet** courant, t_objets* ancre )
{
    t_objet* supp = NULL ;
    if( (*courant)->base.x + (*courant)->base.image[0]->w < 0 || (*courant)->base.y > SCREEN_H || (*courant)->base.y + (*courant)->base.image[0]->h < 0 )
    {
        supp = *courant ;
        *courant = (*courant)->suivant ;
        SuppressionObjet( supp, ancre ) ;
    }
    else
        *courant = (*courant)->suivant ;
}
