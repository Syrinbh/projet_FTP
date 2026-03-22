/*
 * ftpserver.c - A concurrent FTP server using a process pool
 */

#include "../include/csapp.h"
#include "../include/ftp.h"
#include <string.h>

pid_t fils[NB_PROC]; // tableau pour stocker les PID des fils

/* Traitant de signal SIGINT pour le père */
void sigint_handler(int sig) {
    for (int i = 0; i < NB_PROC; i++) {
        if (fils[i] > 0) {
            Kill(fils[i], SIGINT); //Retransmet SIGINT à chaque fils
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
    response_t response; 

    char filepath[FILENAME_MAX_LEN + sizeof(SERVER_DIR)];
    int filefd;        
    char buf[MAXLINE]; 
    ssize_t n;    

    if (Rio_readn(connfd, &req, sizeof(request_t)) <= 0)
        return;

    /* Traitement GET */
    if (req.type == GET) {
        /* construit le chemin complet */
        snprintf(filepath, sizeof(filepath), "%s%s", SERVER_DIR, req.filename);

        if ((filefd = open(filepath, O_RDONLY)) < 0) {
            //erreur
            response.status = ERROR; 
            snprintf(response.message, sizeof(response.message),
                     "ERROR: file '%s' is not found\n", req.filename);
            Rio_writen(connfd, &response, sizeof(response_t));
            return;
        }

        if (fstat(filefd, &st) < 0) {
            response.status = ERROR;
            snprintf(response.message, sizeof(response.message),
                     "ERROR: impossible de lire les métadonnées\n");
            Rio_writen(connfd, &response, sizeof(response_t));
            close(filefd);
            return;
        }

        /* charge fichier */
        filebuf = malloc(st.st_size);
        if (filebuf == NULL) {
            response.status = ERROR;
            snprintf(response.message, sizeof(response.message),
                     "ERROR: mémoire insuffisante\n");
            Rio_writen(connfd, &response, sizeof(response_t));
            close(filefd);
            return;
        }

        if ((n = Rio_readn(filefd, filebuf, st.st_size)) < 0) {
            response.status = ERROR;
            snprintf(response.message, sizeof(response.message),
                     "ERROR: échec de lecture du fichier\n");
            Rio_writen(connfd, &response, sizeof(response_t));
            free(filebuf);
            close(filefd);
            return;
        }
        close(filefd);


        response.status = OK; 
        response.filesize = st.st_size; 
        snprintf(response.message, sizeof(response.message), "OK\n");
        Rio_writen(connfd, filebuf, st.st_size); //on transmet le contenu 

        free(filebuf);

        /* Envoie le contenu du fichier au client par blocs */
        while ((n = read(filefd, buf, MAXLINE)) > 0)
            Rio_writen(connfd, buf, n);
        close(filefd);

        else{ //quand il y'a type invalide
            response.status = ERROR;
            snprintf(response.message, sizeof(response.message),
                    "ERROR: invalid request type %d\n", req.type);
            Rio_writen(connfd, &response, sizeof(response_t));
        }
    }
}
int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    
    /* Installation du traitant SIGINT AVANT la création des fils */
    Signal(SIGINT, sigint_handler); //on installe avant fork 
    
    listenfd = Open_listenfd(FTP_PORT);
    printf("FTP server listening on port %s\n", FTP_PORT);

    /* Création du pool de fils */
    for (int i = 0; i < NB_PROC; i++) {
        fils[i] = Fork();
        if (fils[i] == 0) {
            /* Code du fils */
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

    /* Code du père : attend un signal  */
    while (1) {
        pause(); 
    }

    exit(0);
}

