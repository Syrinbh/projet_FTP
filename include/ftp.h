#include "./include/csapp.h"
/*********************************************
 * Reentrant protocol-independent client/server helpers
 *********************************************/

#define FILENAME_MAX_LEN 256

typedef enum
{
    GET = 0,
    PUT = 1,
    LS = 2
} typereq_t;

/* Question 2 : Structure de données pour les requêtes client */

typedef struct {
    typereq_t  type;                    /* Type de la requête (GET, PUT, LS)  */
    char       filename[FILENAME_MAX_LEN]; /* Nom du fichier concerné         */
} request_t;

