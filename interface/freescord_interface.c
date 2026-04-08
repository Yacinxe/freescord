/* HADIOUI Yacine 12204279
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */
#include <gtk/gtk.h>
#include <stdio.h>
#include "freescord_interface.h"
#include "../communication/communication.h"

#define MAX_H_TEXT 60 // (1136-50)/18 // ces choix son purement personnelle
#define MAX_W_TEXT 104 // (1920-31)/18

//ce semaphore sert à dire que notre connexion est prête pour la lecture
sem_t* semaphore_sock;

// fusionne 2 string 
void fusion_str(Inter* inter,const char* str){
	size_t i=0;

	int h_str=0;
	int h=0;
	// on récupère la taille de la fenêtre
	gtk_window_get_size(inter->window,NULL,&h);

	int h_max=(h-50)/18;
	// on regarde la hauteur du message
	for(;str[i];i++){
		if(str[i]=='\n'){
			h_str+=1;
		}
	}
	// si le message déjà existant et l'ajjout du message ne vas pas changé
	// la taille de la fenêtre
	if(inter->hauteur_str+h_str<h_max){
		for(i=0;inter->str_text[i];i++);
		size_t j=0;
		// on ajoute tous simplement à la fin
		for(;str[j];j++){
			inter->str_text[i+j]=str[j];
		}
		// le \n pour faire jolie
		inter->str_text[i+j]='\n';
		inter->str_text[i+j+1]=0;
		inter->hauteur_str+=1;
		return;
	}
	// sinon on retire la hauteur de trop
	int v=(inter->hauteur_str+h_str)-h_max;
	inter->hauteur_str=h_max;
	for(i=0;v>0 && inter->str_text[i];i++){
		if(inter->str_text[i]=='\n'){
			v-=1;
		}
	}
	size_t j=0;
	for(;inter->str_text[j];j++){
		inter->str_text[j]=inter->str_text[i+j];
	}
	for(int u=0;str[u];j++){
		inter->str_text[j]=str[u];
		u++;
	}
	inter->str_text[j]='\n';
	j++;
	inter->str_text[j]=0;
}

// cette fonction seras effectuer à chaque fois que l'on vas appuyer sur le 
// bouton pour envoyer un message
void validitation(GtkButton *button, gpointer user_data){
	Inter* inter =user_data;
	if(button!=NULL)
	{
		inter->usless=1;
	}
	// si c'est la première fois il faut voir si on à une socket 
	// sinon on la rentre
	if(inter->sock==-1){
		char* ent=(char*)gtk_entry_get_text(inter->entry);
		int sock=connect_serveur_tcp(ent,1234);
		if (sock < 0) {
			gtk_label_set_text(inter->text,
					"humm tu as du te tromper sur l'adresse\n");
			return;
			
		}
		inter->sock=sock;
		sem_post(semaphore_sock);
		gtk_label_set_text(inter->text,"qu'elle est votre nom?\n");
		gtk_entry_set_text(inter->entry,"");
		return;
	}
	else
	{
		char* ent=(char*)gtk_entry_get_text(inter->entry);
		if(ent[0]!=0){
			envoie_msg(ent,inter->sock);
		}
		gtk_label_set_text (inter->text,inter->str_text);
		gtk_entry_set_text(inter->entry,"");
	}
}

//for the chose buttom

void on_fchose(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	//creat dialog windows
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;
	// create a new dialog windows
	dialog = gtk_file_chooser_dialog_new ("Open File",
			NULL,
			action,
			("_Cancel"),
			GTK_RESPONSE_CANCEL,
			("_Open"),
			GTK_RESPONSE_ACCEPT,
			NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	{
		//this is the filename
		char *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		filename = gtk_file_chooser_get_filename (chooser);
		printf("%s\n",filename);
	}

	gtk_widget_destroy (dialog);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}
// create interface
void init_interface(sem_t *s, Inter* inter)
{                                                                               

	// Initializes GTK.                                                         
	gtk_init(NULL, NULL);                                                       

	// Loads the UI description and builds the UI.                              
	// (Exits if an error occurs.)                                              
	GtkBuilder* builder = gtk_builder_new();                                    
	GError* error = NULL;                                                       
	if (gtk_builder_add_from_file(builder, "./src/interface/freescrod.glade", &error) == 0)         
	{                                                                           
		g_printerr("Error loading file: %s\n", error->message);                 
		g_clear_error(&error);                                               
	}                                                                           
	// Gets the widgets.                                                        
	GtkWindow* window = 
		GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.inter"));

	GtkButton* valid_btn = GTK_BUTTON(gtk_builder_get_object(builder, "valid_btn"));
	GtkButton* fichier_btn = GTK_BUTTON(gtk_builder_get_object(builder, "btn_fichier"));

	GtkLabel* text= GTK_LABEL(gtk_builder_get_object(builder, "affichage"));

	GtkEntry* entry= GTK_ENTRY(gtk_builder_get_object(builder, "entre_text"));

	char* str=malloc(sizeof(char)*MAX_H_TEXT*MAX_W_TEXT);
	str[0]=0;
	//set a part of inter
	inter->window=window;
	inter->valid_btn=valid_btn;
	inter->usless=1;
	inter->builder=builder;
	inter->entry=entry;
	inter->text=text;
	inter->str_text=str;
	inter->hauteur_str=0;
	inter->sock=-1;
	inter->btn_fichier=fichier_btn;
	// set button target

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),inter);
	g_signal_connect(valid_btn, "clicked", 
			G_CALLBACK(validitation), inter);
	g_signal_connect(fichier_btn, "clicked", G_CALLBACK(on_fchose), &inter);
	semaphore_sock=s;
	gtk_label_set_text(inter->text,"bienvenue sur freescord donne moi une adresse pour te connecter");
	gtk_main();         	
} 
