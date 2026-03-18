/*
 * ftpserver.c - A concurrent FTP server using a process pool
 */

#include "csapp.h"
#include <string.h>

#define FTP_PORT     2121
#define NB_PROC      4
#define FILENAME_MAX_LEN 256

/* Traitant de signal SIGINT pour le père */
void sigint_handler(int sig) {
    /* Retransmet SIGINT à chaque fils */
    for (int i = 0; i < NB_PROC; i++) {
        if (fils[i] > 0) {
            Kill(fils[i], SIGINT);
        }
    }

    /* Attend que tous les fils se terminent */
    for (int i = 0; i < NB_PROC; i++) {
        Waitpid(fils[i], NULL, 0);
    }

    printf("Serveur arrêté proprement.\n");
    exit(0);
}

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

    /* Création du pool de fils */
    for (int i = 0; i < NB_PROC; i++) {
        fils[i] = Fork();
        if (fils[i] == 0) {
            /* --- Code du fils --- */
            /* Le fils remet le handler par défaut pour SIGINT */
            Signal(SIGINT, SIG_DFL);
            while (1) {
                clientlen = sizeof(clientaddr);
                connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
                handle_client(connfd);
                Close(connfd);
            }
            exit(0);
        }
    }

    /* --- Code du père : attend un signal --- */
    while (1) {
        pause(); /* bloqué jusqu'à réception d'un signal */
    }

    exit(0);
}
