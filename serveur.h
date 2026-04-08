/* DUQUEYROIX Saul 12313228
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */
#ifndef SERVEUR_H
#define SERVEUR_H value
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include <semaphore.h>
#include "communication/communication.h"

// personne désigne un client 
struct personne {
	int sock_envoi; //socket pour l'envoie de message
	char* nom;	// nom que le client c'est donné par defaut c'est plus tard
	int taile_nom;	// taille du nom en question
};

// c'est utlile juste pour envoyer toutes les information à 
// gestion_individu qui est une fonction utilisé pour les pthread
struct paquet{
	struct list* li;  	// liste de toutes les personnes
	int sock;		// la socket du client
};

void* gestion_individu(void* arg);
void envoie_msg_list(struct list* li,char* str);
#endif 
