/*
 * ftpclient.c - A simple FTP client
 */

#include "../include/csapp.h"
#include "../include/ftp.h"
#include <string.h>

#define FTP_PORT     2121
#define FILENAME_MAX_LEN 256

int main(int argc, char **argv)
{
    int clientfd;
    request_t req;
    char buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <filename>\n", argv[0]);
        exit(0);
    }

    /* Connexion au serveur FTP sur le port fixe 2121 */
    clientfd = Open_clientfd(argv[1], FTP_PORT);
    printf("client connected to FTP server\n");

    /* Construction de la requête GET */
    req.type = GET;
    strncpy(req.filename, argv[2], FILENAME_MAX_LEN - 1);
    req.filename[FILENAME_MAX_LEN - 1] = '\0';

    /* Envoi de la requête */
    Rio_writen(clientfd, &req, sizeof(request_t));

    /* Réception et affichage de la réponse (contenu du fichier) */
    Rio_readinitb(&rio, clientfd);
    while (Rio_readlineb(&rio, buf, MAXLINE) > 0) {
        Fputs(buf, stdout);
    }

    Close(clientfd);
    exit(0);
}