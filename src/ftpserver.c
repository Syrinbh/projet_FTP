/*
 * ftpserver.c - A concurrent FTP server using a process pool
 */

#include "csapp.h"
#include <string.h>

#define FTP_PORT     2121
#define NB_PROC      4
#define FILENAME_MAX_LEN 256

/* Types de requêtes */
typedef enum {
    GET,
    PUT,
    LS
} typereq_t;

/* Structure d'une requête client */
typedef struct {
    typereq_t type;
    char filename[FILENAME_MAX_LEN];
} request_t;

/* Traite une connexion client : lit la requête et répond */
void handle_client(int connfd) {
    request_t req;

    /* Lire la requête envoyée par le client */
    if (Rio_readn(connfd, &req, sizeof(request_t)) <= 0)
        return;

    if (req.type == GET) {
        /* TODO : ouvrir le fichier req.filename et envoyer son contenu */
    }
    /* TODO : gérer PUT, LS */
}

int main(void)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;

    listenfd = Open_listenfd(FTP_PORT);
    printf("FTP server listening on port %d\n", FTP_PORT);

    /* Création du pool de NB_PROC processus fils */
    for (int i = 0; i < NB_PROC; i++) {
        pid_t pid = Fork();
        if (pid == 0) {
            /* --- Code du fils --- */
            while (1) {
                clientlen = sizeof(clientaddr);
                connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
                handle_client(connfd);
                Close(connfd);
            }
            exit(0); /* jamais atteint */
        }
        /* Le père continue la boucle pour créer les autres fils */
    }

    /* --- Code du père : attend un signal d'arrêt --- */
    /* TODO : gérer SIGTERM / SIGINT pour tuer les fils proprement */
    pause();

    Close(listenfd);
    exit(0);
}