/*---------------------------------------------------------------
   Vanstapel Herman
   ex02\cli.c

 Le client dit bonjour en utilisant un structure  et
 le serveur fait de même
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "../udplib/udplib.h"
#include "requeteme_rc.h"
#include <termios.h>
#include "LibSerME_RC.h"


static struct termios old, new;

void die(char *s){
   perror(s);
   exit(1);
}

void initTermios(int echo) 
{
   tcgetattr(0, &old); //grab old terminal i/o settings
   new = old; //make new settings same as old settings
   new.c_lflag &= ~ICANON; //disable buffered i/o
   new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
   tcsetattr(0, TCSANOW, &new); //apply terminal io settings
}
 
/* Restore old terminal i/o settings */
void resetTermios(void) 
{
   tcsetattr(0, TCSANOW, &old);
}
 

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
   char ch;
   initTermios(echo);
   ch = getchar();
   resetTermios();
   return ch;
}
 
/* 
Read 1 character without echo 
getch() function definition.
*/
char Getch(void) 
{
   return getch_(0);
}
 
/* 
Read 1 character with echo 
getche() function definition.
*/
char GetchE(void) 
{
   return getch_(1);
}

int main(int argc, char *argv[]){
   int rc;
   int Desc;
   int tm; 

   u_long  IpSocket , IpServer;
   u_short PortSocket, PortServer; 

   struct sockaddr_in sthis; /* this ce programme */
   struct sockaddr_in sos; /* s = serveur */
   struct sockaddr_in sor; /* r = remote */
   struct RequeteME_RC UneRequete;
   struct VehiculeHV  UnRecord ;

   memset(&sthis, 0, sizeof(struct sockaddr_in));
   memset(&sos, 0, sizeof(struct sockaddr_in)); 
   memset(&sor, 0, sizeof(struct sockaddr_in));
   memset(&UneRequete, 0, sizeof(struct RequeteME_RC));

   char Choix;
   if (argc!=5){
      printf("cli client portc serveur ports\n");
      exit(1);
   }

   /* Récupération IP & port   */
   IpSocket= inet_addr(argv[1]);
   PortSocket = atoi(argv[2]);

   IpServer = inet_addr(argv[3]);
   PortServer = atoi(argv[4]);

   // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4]));
   Desc=creer_socket(SOCK_DGRAM, &IpSocket, PortSocket, &sthis);

   if (Desc == -1)
      die("CreateSockets:"); 
   else
      printf("CreateSockets %d\n", Desc);

   sos.sin_family = AF_INET;
   sos.sin_addr.s_addr= IpServer;
   sos.sin_port = htons(PortServer);
   while(1){
    printf("-----------------------------   \n");
    printf("1)Demander une reference          \n");
    printf("2)Acheter un Véhicule       \n");
    printf("3)Quitter         \n");
    printf(">>") ; Choix=GetchE() ; printf("\n") ;


   //Gestion entrée utilisateur de la ref du vehicule à rechercher
   char tampon[8];
   char NomClient[40];
   int ref;
   int quantite;

   switch(Choix){
      case '1':
         printf("Sasie reference vehicule:");
         fgets(tampon, sizeof(tampon), stdin);
         UneRequete.Type = Question;
         UneRequete.Reference = atoi(tampon);

         rc = SendDatagram(Desc, &UneRequete, sizeof(struct RequeteME_RC), &sos);

         if (rc == -1)
            die("SendDatagram");
         else
            fprintf(stderr, "Envoi de %d bytes\n", rc);

         memset(&UneRequete, 0, sizeof(struct RequeteME_RC));
         tm = sizeof(struct RequeteME_RC);

         rc = ReceiveDatagram(Desc, &UneRequete, tm, &sor);
         if (rc == -1)
            die("ReceiveDatagram");
         else
            printf("bytes lus:%d\n", rc);
         // Si la requête n'a pas échouée alors affichage du véhicule
         if (UneRequete.Type != Fail)
            printf("Constructeur, Modele, Puissance:%s %s %d\n", UneRequete.Constructeur, UneRequete.Modele, UneRequete.Puissance);
         else
            printf("Echec de la requête\n");

         AfficheRequeteME_RC(stderr, UneRequete);
         break;
      case '2':
         //Set les champs de la requête achat à envoyer au serveur
         UneRequete.Type = Achat;
         UneRequete.Numero = atoi(tampon);
         
         // Demande information pour achat
         printf("\nNomClient:");
         fgets(tampon, sizeof(tampon), stdin);
         strcpy(UneRequete.NomClient, tampon);
         printf("\nReference:");
         fgets(tampon, sizeof(tampon), stdin);
         ref = atoi(tampon);
         printf("\nQuantite:");
         fgets(tampon, sizeof(tampon), stdin);
         quantite = atoi(tampon);
         UneRequete.Reference = ref; 
         UneRequete.Quantite = quantite;

         //Envoi de la requête au serveur
         rc = SendDatagram(Desc, &UneRequete, sizeof(struct RequeteME_RC), &sos);

         if (rc == -1)
            die("SendDatagram");
         else
            printf("Envoi de %d bytes\n", rc);

         memset(&UneRequete, 0, sizeof(struct RequeteME_RC));
         tm = sizeof(struct RequeteME_RC);

         //Reception de la réponse du serveur
         rc = ReceiveDatagram(Desc, &UneRequete, tm, &sor);
         if (rc == -1)
            die("ReceiveDatagram");
         else{
            printf("bytes lus %d\n", rc);
            printf("Achat Reussi Facture: %d\n", UneRequete.NumeroFacture);
         }

         break;
      case '3':
         exit(0);
      break;
   }
}

   close(Desc);
 
}
