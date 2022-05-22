#include "biblio.h"
/** Permet de supprimer un maillon de l'explosion une fois celui-ci effectu�.
Re�oit l'ancre et le maillon � supprimer par param�tre.  */
void suppression_explosion( t_explosions* ancre, t_explo* suppression )
{
    t_explo* courant = ancre->premier ;
    t_explo* precedent = NULL ;
    //on parcours toute la liste cha�n�e jusqu'au maillon � supprimer,
    //on obitent ainsi l'adresse du maillon pr�c�dent
    while(courant != suppression)
    {
        precedent = courant ;
        courant = courant->suivant ;
    }
    //si le maillon � supprimer est le premier de la liste cha�n�e
    if( precedent == NULL )
    {
        ancre->premier = courant->suivant ;
        if( ancre->premier == NULL )
            ancre->dernier = NULL ;
    }
    else
    {
        //si le maillon � supprimer est le dernier de la lite cha�n�e
        if( ancre->dernier == courant )
            ancre->dernier = precedent ;
        precedent->suivant = courant->suivant ;
    }
    //on lib�re l'espace
    free(suppression->base.image[0]);
    free(suppression->base.image);
    free(suppression);
}
/**Condition de suppression des ennemis re�oit le premier et dernier*/
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
/**Suppression des opposants avec en param�tre le premier et dernier maillon de la liste
ainsi que le maillon � supprimer*/
void suppression_ennemi( t_ennemi** premier, t_ennemi** dernier, t_ennemi* suppression )
{
    t_ennemi* courant = *premier ;
    t_ennemi* precedent = NULL ;
    //on parcours toute la liste cha�n�e jusqu'au maillon � supprimer,
    //on obitent ainsi l'adresse du maillon pr�c�dent
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
    //on lib�re l'espace
    free(suppression->base.image[0]);
    free(suppression->base.image);
    free(suppression);
}
/**Supprime et lib�re un maillon de tir.
Re�oit le pointeur du maillon � supprimer, l'ancre de la liste cha�n� des tirs. */
void SuppressionTir(t_munition * suppression, t_collection * ancre)
{
    //on retrouve et on r�organise les liaisons de la liste chain�
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
    //on lib�re l'espace
    free(suppression->base.image[0]);
    free(suppression->base.image);
    free(suppression);
}
/**Permet de supprimer tous les objets de la liste cha�n�e
Re�oit en param�tre l'ancre de celle-ci*/
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
Re�oit en param�tre l'adresse du maillon � supprimer et l'ancre de la liste cha�n�e.*/
void SuppressionObjet(t_objet * suppression, t_objets * ancre)
{
    //on retrouve et on r�organise les liaisons de la liste chain�
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
    //on lib�re l'espace
    free(suppression->base.image[0]);
    free(suppression->base.image);
    free(suppression);
}
///Condition de suppression des ennemis re�oit le premier et dernier*/
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
