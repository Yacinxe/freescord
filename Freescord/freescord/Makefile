# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -g $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = $(shell pkg-config --libs gtk+-3.0) -lpthread

# Dossiers
INT_DIR = interface
COM_DIR = communication
LST_DIR = list

# Sources et Objets
# On sépare les objets communs, ceux du client et ceux du serveur
COMMON_SRCS = $(COM_DIR)/communication.c
LIST_SRCS = $(LST_DIR)/list.c
INT_SRCS = $(INT_DIR)/freescord_interface.c

CLIENT_SRCS = client.c $(COMMON_SRCS) $(INT_SRCS)
SERVEUR_SRCS = serveur.c $(COMMON_SRCS) $(LIST_SRCS)

CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
SERVEUR_OBJS = $(SERVEUR_SRCS:.c=.o)

# Cibles principales
all: client serveur

client: $(CLIENT_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

serveur: $(SERVEUR_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Règle générique pour les fichiers .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f *.o $(INT_DIR)/*.o $(COM_DIR)/*.o $(LST_DIR)/*.o client serveur

.PHONY: all clean