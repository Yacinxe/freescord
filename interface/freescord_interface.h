/* DUQUEYROIX Saul 12313228
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */
#ifndef INTERFACE_H_
#define INTERFACE_H_
#include <gtk/gtk.h>
#include <semaphore.h>

typedef struct Inter
{
	GtkWindow* window;              // Main window
	GtkButton* valid_btn;      	// le bouton pour validé le message	
	GtkButton* btn_fichier;       	// le bouton pour envoyer un fichier
	GtkBuilder* builder;		// le builder de fenetre
	GtkLabel* text;			// le label
	GtkEntry* entry;		// la où on vas entre le text 
	int usless;			// comme son nom l'indique il est
       					//inutile mais évite les warning
	char* str_text;			// c'est le string qui est afficher
	int hauteur_str;		// c'est la hauteur du string afficher
	int sock;			// c'est la socket pour parlé au serveur
} Inter;

void validitation(GtkButton *button, gpointer user_data);
void init_interface(sem_t *s,Inter* inter);
char* reception_massage(int*,const char*);
void fusion_str(Inter* inter,const char* str);
void on_fchose(GtkButton *button, gpointer user_data);
//void init_interface
#endif
