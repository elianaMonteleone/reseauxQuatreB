/*--------------------------------------
Herman Vanstapel

ex02\ser.c 

Un serveur recevant une structure 
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "../udplib/udplib.h"
//#include "structure.h"
#include "requeteme_rc.h"
#include "LibSerME_RC.h"
#include "data.h"


void die(char *s){
   perror(s);
   exit(1);
}

int main(int argc,char *argv[]){
   int rc;
   int Desc;
   struct sockaddr_in sthis; /* this ce programme */
   struct sockaddr_in sos; /* s = serveur */
   struct sockaddr_in sor; /* r = remote */

   u_long  IpSocket;
   u_short PortSocket;

   int tm;
   struct RequeteME_RC UneRequete;
   struct VehiculeHV V;

   memset(&sthis,0,sizeof(struct sockaddr_in));
   memset(&sos,0,sizeof(struct sockaddr_in)); 
   memset(&sor,0,sizeof(struct sockaddr_in)); 

   printf("Ceci est le serveur\n");
   if ( argc!=3){
      printf("ser ser port cli\n");
      exit(1);
   }

   /* Récupération IP & port   */
   IpSocket= inet_addr(argv[1]);
   PortSocket = atoi(argv[2]);

   // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4]));
   Desc=creer_socket(SOCK_DGRAM,&IpSocket,PortSocket,&sthis);

   if ( Desc == -1 )
      die("CreateSockets:");
   else
      printf("CreateSockets %d\n", Desc);

   while(1){
   tm = sizeof(struct RequeteME_RC);
   rc = ReceiveDatagram(Desc, &UneRequete ,tm, &sor);
   if ( rc == -1 )
      die("ReceiveDatagram");
   else{
      printf("bytes Lus %d Reference:%d\n", rc, UneRequete.Type);
      AfficheRequeteME_RC(stderr, UneRequete);
   }
   
   printf("Type recu %d\n", UneRequete.Type);
   printf("Received packet from %s:%d\n", inet_ntoa(sor.sin_addr), ntohs(sor.sin_port));
  
   /* attention l'enum peut être codé en short */
   /* reponse avec psos */

   //Recherche du Véhicule avec la ref reçue dans la requête du client
   fprintf(stderr, "RechercheME_RC>\n");
   int result_recherche = RechercheME_RC("VehiculesHV", UneRequete.Reference, &V);
   fprintf(stderr, "RechercheME_RC<\n");
   //Set UneRequete avec la réponse à la requête du client

   switch(UneRequete.Type){
      case Question: 
            result_recherche = RechercheME_RC("VehiculesHV", UneRequete.Reference, &V); 
            fprintf(stderr,"res :%d Reference:%d %s\n",result_recherche, UneRequete.Puissance, UneRequete.Modele) ;
            memset(&UneRequete, 0, sizeof(struct RequeteME_RC));
             if(result_recherche == 1){
               UneRequete.Type = OK;
               UneRequete.Reference = V.Reference;
               strcpy(UneRequete.Constructeur, V.Constructeur);
               strcpy(UneRequete.Modele, V.Modele);
               UneRequete.Puissance =V.Puissance;
               }
             else
               UneRequete.Type = Fail;
         break;
      case Achat: 
            result_recherche = RechercheME_RC("VehiculesHV", UneRequete.Reference, &V); 
            if(UneRequete.Reference > 0 && result_recherche == 1){
               fprintf("Trouve: %c Quantite: %d" ,UneRequete.Modele, UneRequete.Quantite);
               AchatME_RC("VehiculesHV", UneRequete.Reference, UneRequete.Quantite);
               if(UneRequete.Quantite > 0 && FacturationME_RC("FactureHV", UneRequete.NomClient, UneRequete.Quantite, UneRequete.Reference) > 1){
                  rc = SendDatagram(Desc, &UneRequete, sizeof(struct RequeteME_RC), &sor);
               }
               else {
                  printf("aucun véhicule");
               }
         }
         break;

   }
  
   //Send réponse
   rc = SendDatagram(Desc, &UneRequete, sizeof(struct RequeteME_RC), &sor);
   if ( rc == -1 )
      die("SendDatagram:");
   else
      fprintf(stderr, "bytes ecrits:%d\n", rc);
   
   fprintf(stderr, "test\n");
   }

   close(Desc);
}


