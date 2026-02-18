#include "serveur.h"

#define PORT_FREESCORD 1234
#define TAILLE_LIST 20

//permet de fixe le nombre maximum de personne sur le serveur
sem_t client_actif;
// evite toute modification de la liste par deux personne en même temp
pthread_mutex_t modif_verrou;

int main()
{
	/* Création d'une socket udp ipv4 */
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(2);
	}

	/* Création de la sockaddr locale */
	struct sockaddr_in sa = {
		.sin_family = AF_INET, /* IPv4 */
		.sin_port = htons(PORT_FREESCORD),
		.sin_addr.s_addr = htonl(INADDR_ANY) };

	/* Attacher la socket à la sockaddr locale */
	if (bind(sock, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		perror("bind");
		exit(3);
	}

	pthread_t t[TAILLE_LIST];
	for(int i=0;i<TAILLE_LIST;i++){
		t[i]=0;
	}

	sem_init(&client_actif, 0, TAILLE_LIST);
	pthread_mutex_init(&modif_verrou, NULL);

	struct list* list_client=list_create();
	// le while(1) c'est pour dire que le serveur ne doit pas s'arrêter
	while(1){
		if (listen(sock, 128) < 0){
			perror("Echec listen()\n");
			exit(EXIT_FAILURE);
		}
		struct paquet* paq=malloc(sizeof(struct paquet));
		paq->sock=sock;
		paq->li=list_client;
		// bloque si le maximum de client est atteind
		sem_wait(&client_actif);
		int i=0;
		while(t[i]!=0){
			i++;
		}
		pthread_create(&(t[i]),NULL,gestion_individu,paq);

	}
	pthread_mutex_destroy(&modif_verrou);
	close(sock);
	return 0;
}

// permet la gestion d'un client
void* gestion_individu(void* arg){
	struct paquet* paq=arg;
	struct list* list=paq->li;
	int sock= paq->sock;

	socklen_t sl = sizeof(struct sockaddr_in);
	struct sockaddr_in client_addr;
	// attente de connection client
	int client_sock =
		accept(sock,(struct sockaddr *)(&client_addr), &sl);
	if (client_sock < 0){
		perror("Echec Accept()\n");
		exit(EXIT_FAILURE);
	}
	// client_sock est connecté ! On l'annonce
	char client_name[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, 
				client_name,sizeof(client_name)) != NULL)
		printf("Client %s/%d est connecte \n", client_name,
				ntohs(client_addr.sin_port));
	else
		perror("Impossible de récupérer l'adresse du client\n");

	// création de la personne
	struct personne *p=malloc(sizeof(struct personne));
	p->sock_envoi=client_sock;
	p->nom="plus tard"; // nom par défaut
	p->taile_nom=10;
	
	//la réception du nom en question
	p->nom=reception_message(&(p->taile_nom),client_sock);
	p->taile_nom+=1;

	// ajjoute le nom dans la liste chainé
	pthread_mutex_lock(&modif_verrou);
	list_add(list,p);
	pthread_mutex_unlock(&modif_verrou);
	
	//c'est bloquant
	int taille;
	char* str=reception_message(&taille,client_sock);
	
	// tent que le client parle on l'écoute
	while(str!=NULL){
		char* str_sp= ajjout_speudo(p->nom,p->taile_nom,str);
		envoie_msg_list(list,str_sp);
		free(str);
		//c'est bloquant
		str=reception_message(&taille,client_sock);

	}
	// on oublie pas de free la personne et de libéré une place pour une 
	// nouvelle connexion
	free(paq);
	sem_post(&client_actif);
	return NULL;
}

// fonction simple qui envoie un message à tous le monde
void envoie_msg_list(struct list* li,char* str){
	size_t len=list_length(li);
	pthread_mutex_lock(&modif_verrou);
	for(size_t i=0;i<len;i++){
		struct personne* p=list_get(li,i);
		envoie_msg(str,p->sock_envoi);
	}
	pthread_mutex_unlock(&modif_verrou);
	free(str);
}
