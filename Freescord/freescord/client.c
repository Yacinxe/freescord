#include"client.h"

#define PORT_FREESCORD 4321
#define ADDRESSE_B "127.0.0.1"

//ce semaphore sert à savoir si on peut lire la socket
//on peut la retrouve dans interface
sem_t s;

int main()
{

	sem_init(&s, 0, 0);
	Inter* inter=malloc(sizeof(Inter));
	pthread_t a;
	// un pthread pour pouvoir lire et parlé en même temp
	pthread_create(&a, NULL, ecoute_client, inter);
	init_interface(&s,inter);	
	return 0;
}

//cette fonction écoute le client elle est bloquant et on ne s'arrête que 
//si il y a un problème avec reception_message généralement sera dus au faite 
// que le serveur s'arrête.
void * ecoute_client(void* arg){
	sem_wait(&s);
	Inter* inter=arg;
	int i;
	char* str=reception_message(&i,inter->sock);
	while(str!=NULL){
		fusion_str(inter,str);
		gtk_label_set_text (inter->text,inter->str_text);
		free(str);
		str=reception_message(&i,inter->sock);
	}
	return NULL;
}
