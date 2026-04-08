/* HADIOUI Yacine 12204279
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */
#include"communication.h"

#define PORT_FREESCORD 4321
#define ADDRESSE_B "127.0.0.1"

// Cette fonction ajjoute un speudo au début d'un message
// retrun les string fini attention il est créer esur le tas
char* ajjout_speudo(const char* speudo,int taille_speudo,const char* str){
	int i=0;
	taille_speudo-=1;
	for(;str[i];i++);
	char* str_final=malloc(sizeof(char)*(i+taille_speudo+4));
	for(int j=0;j<taille_speudo;j++){
		str_final[j]=speudo[j];
	}
	// c'est pour la beauté du message
	str_final[taille_speudo]=' ';
	str_final[taille_speudo+1]=':';
	str_final[taille_speudo+2]=' ';
	int j=0;
	for(;j<i;j++){
		str_final[taille_speudo+j+3]=str[j];
	}
	str_final[taille_speudo+j+3]='\n';
	return str_final;
}

//se connecte à un serveur en TCP et return la socket 
int connect_serveur_tcp(char *adresse, uint16_t port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(2);
	}
	struct sockaddr_in sa = { .sin_family = AF_INET, /* IPv4 */
		.sin_port = htons(port)};

	/* "Internet Presentation to Numeric" convertit une représentation
	 * d'adresse IP (v4 ou v6) en numérique (gros-boutiste) */
	if (inet_pton(AF_INET, adresse, &sa.sin_addr) != 1) {
		fprintf(stderr, "adresse ipv4 non valable\n");
		exit(1);
	}

	/* À cause d'IPv4 versus IPv6 il faut souvent envoyer ou recevoir
	 * la taille de la sockaddr */
	socklen_t sl = sizeof(struct sockaddr_in);

	/* 4. Demande de connexion TCP */
	/* Remarque : c'est la première fois que le socket est utilisé,
	 * c'est à ce moment que le noyau choisit un port éphémère
	 * et le lie à ce socket. */
	if (connect(sock, (struct sockaddr *) &sa, sl) < 0) {
		perror("connect");
		exit(1);
	}
	return sock;
}

// transforme un int en char par exemple 123 ->"123"
// le deuxième paramètre doit etre assez grand
void transform_int_char(const int i,char* str){
	int l=1;
	int t=10;
	while(t<=i){
		t*=10;
		l+=1;
	}
	l=0;
	while(t>1){
		t/=10;
		str[l]='0'+(i/t)%10;
		l++;
	}
	str[l]=0;
}

// transforme un char en int par exemple "123"->123
int transform_char_int(const char* str){
	int v=0;
	int i=0;
	while(str[i]){
		v*=10;
		v+=str[i]-'0';
		i++;
	}
	return v;
}

//resoit un string et l'envoie sur la socket en question
void envoie_msg(char* msg,int sock){
	int t=0;
	for(;msg[t] && msg[t]!='\n';t++);
	int l=1;
	int ti=10;
	while(ti<=t){
		ti*=10;
		l+=1;
	}
	char str[l+3+t];
	// la première lettre est un m pour dire que c'est un message
	str[0]='m';
	transform_int_char(t,str+1);
	l++;
	str[l]='\n';
	l++;
	int i=0;
	for(;msg[i] && msg[i]!='\n';i++){
		str[i+l]=msg[i];
	}
	str[i+l]='\n';
	write(sock,str,l+1+t);	
}

//envoit un fichier sur la socket attention cette fonction ne fonctionne pas 
//ou en tout cas elle n'a pas ete testé 
void envoie_fichier(char* nom,int sock,int f){
	int t=0;
	for(;nom[t] && nom[t]!='\n';t++);
	int l=1;
	int ti=10;
	while(ti<=t){
		ti*=10;
		l+=1;
	}
	char str[l+3+t];
	//la première lettre est un f pour dire que c'est un fichier
	str[0]='f';
	transform_int_char(t,str+1);
	l++;
	str[l]='\n';
	l++;
	int i=0;
	for(;nom[i] && nom[i]!='\n';i++){
		str[i+l]=nom[i];
	}
	str[i+l]='\n';
	write(sock,str,l+t+1);
	ssize_t n;
	char* buff[1024];
	while((n=read(f,buff,1024))>0){
		write(sock,buff,n);
	}
	close(sock);
}

// lit dans la socket change le i ressu en argument par la longueur du string
// et retourn le string reçu sur le tas.
// il y a un read bloquant
char* reception_message(int* i, int sock){
	char str[2];
	int n=read(sock,str,1);
	if(n==0){
		printf("je part\n");
		return NULL;
	}
	int v=0;
	// le m permet de dire que c'est un message
	if(str[0]=='m'){
		n=read(sock,str,1);
		// recuppere la longueur du message
		while(str[0]!='\n'){
			if(str[0]>='0' && str[0]<='9'){
				v*=10;
				v+=str[0]-'0';
			}
			n=read(sock,str,1);
			if(n==0){
				printf("je part\n");
				return NULL;
			}
		}
		char* str_final=malloc(sizeof(char)*v+1);
		if(str_final==NULL){
			perror("ERREUR MALLOC: BUFFER\n");
			return NULL;
		}
		int t=0;
		// on lit tous le message 
		while(t<v){
			int temp=read(sock,str_final+t,v-t);
			if(temp==0){
				printf("je part\n");
				free(str_final);
				return NULL;
			}
			t+=temp;
		}
		// ceci est pour être sûr d'avoir tout lu comme nos message ce
		// fini par \n
		n=read(sock,str,1);
		if(n==0 || str[0]!='\n'){
			free(str_final);
			perror("ERREUR DANS LA RECEPTION\n");
			return NULL;
		}
		*i=v;
		str_final[v]=0;
		return str_final;
	}
	// ceci concerne les fichier il est exactement fait comme les message
	if(str[0]=='f'){
		n=read(sock,str,1);
		while(str[0]!='\n'){
			if(str[0]>='0' && str[0]<='9'){
				v*=10;
				v+=str[0]-'0';
			}
			n=read(sock,str,1);
			if(n==0){
				printf("je part\n");
				return NULL;
			}
		}
		char* str_final=malloc(sizeof(char)*v+1);
		if(str_final==NULL){
			perror("ERREUR MALLOC: BUFFER\n");
			return NULL;
		}
		int t=0;
		while(t<v){
			int temp=read(sock,str_final+t,v-t);
			if(temp==0){
				printf("je part\n");
				free(str_final);
				return NULL;
			}
			t+=temp;
		}
		str_final[v]=0;
		int fd=open(str_final, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if(fd<0){
			perror("create file :(\n");
			return NULL;
		}
		char* buff[1024];
		while(read(sock,buff,1024)>0){
			write(fd,buff,n);
		}
		return NULL;
	}
	return NULL;
}
