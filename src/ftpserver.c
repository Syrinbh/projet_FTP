/*
 * ftpserver.c - A concurrent FTP server using a process pool
 */

#include "../include/csapp.h"
#include "../include/ftp.h"
#include <string.h>

#define FTP_PORT     2121
#define NB_PROC      4
#define FILENAME_MAX_LEN 256

pid_t fils[NB_PROC]; // tableau pour stocker les PID des fils

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
    char filepath[FILENAME_MAX_LEN + sizeof(SERVER_DIR)]; // pour construire le chemin complet du fichier

    if (Rio_readn(connfd, &req, sizeof(request_t)) <= 0)
        return;

    if (req.type == GET) {
        /* Construit le chemin complet : "server_files/calcul.c" */
        snprintf(filepath, sizeof(filepath), "%s%s", SERVER_DIR, req.filename);

        /* TODO : ouvrir filepath et envoyer son contenu */
    }
}
int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;

    char client_ip_string[INET_ADDRSTRLEN]; //a enlever si pas besoin d'afficher l'IP du client
    char client_hostname[MAX_NAME_LEN];
    
    /* Installation du traitant SIGINT AVANT la création des fils */
    Signal(SIGINT, sigint_handler); //on installe avant fork 
    
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

