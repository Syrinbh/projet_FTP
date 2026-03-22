# Projet FTP Server 

Projet (Matière Systeme et Reseaux) L3S6 - Université Grenoble Alpes (IM2AG) 

## Description 

## Compilation 
make pour compiler 
./ftpserver (dans un terminal) , sert à lancer le seveur 

./ftpclient localhost fichierTest.txt (dans un autre terminal) pour lancer le client 

## Remarques questions 
# Etape 1 Question 4 

Signal(SIGINT, sigint_handler)
 est installé avant le Fork() — sinon les fils hériteraient du handler personnalisé et tueraient leurs frères.

Signal(SIGINT, SIG_DFL) dans le fils , remet le comportement par défaut (terminaison simple) pour que le fils s'arrête proprement quand il reçoit le signal du père.

Signal(SIGINT, SIG_DFL);  //le fils mourra quand le père lui envoie SIGINT

Waitpid() dans le handler — évite les processus zombies (fils terminés mais non récupérés par le père).

fils[] global (tableau de pid_t) nécessaire car le handler de signal n'a pas accès aux variables locales de main().

# Etape 1 question 5
L'enjeu est que client et serveur tournent potentiellement sur la même machine — ils ne peuvent donc pas utiliser le même dossier, sinon le fichier reçu écraserait le fichier source.

# Etape 1 question 6 
## Partie Reception d'une requete 
le serveur doit identifier le type de requête et effectuer le traitement en conséquence.
Si le type de requête est invalide, le serveur doit renvoyer une réponse indiquant une erreur au client 