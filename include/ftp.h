/* ftp.h - types partagés entre client et serveur */

#ifndef FTP_H
#define FTP_H

#define FTP_PORT         2121
#define NB_PROC          4
#define FILENAME_MAX_LEN 256

#define SERVER_DIR  "server_files/"   
#define CLIENT_DIR  "client_files/"   

typedef enum
{
    GET, //telecharge un fichier du serveur
    PUT, //envoie un fichier au serveur
    LS, //liste les fichiers disponibles sur le serveur
} typereq_t;

typedef struct {
    typereq_t  type;    /* Type de la requête  */
    char filename[FILENAME_MAX_LEN]; /* Nom du fichier */
} request_t;


#endif