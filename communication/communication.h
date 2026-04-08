/* DUQUEYROIX Saul 12313228 
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */
#ifndef BUFFER_H
#define BUFFER_H
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include "../interface/freescord_interface.h"


int connect_serveur_tcp(char *adresse, uint16_t port);
void envoie_msg(char* msg,int sock);
void transform_int_char(const int i,char* str);
int transform_char_int(const char* str);
char* reception_message(int* i, int sock);
char* ajjout_speudo(const char* speudo,int taille_speudo,const char* str);
void envoie_fichier(char* nom,int sock,int f);

#endif
