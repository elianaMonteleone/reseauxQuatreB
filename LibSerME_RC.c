#include "LibSerME_RC.h"

#include <stdlib.h>

void AProposServeurHV(char *Version,char *Nom1,char* Nom2){
   printf("Version : %s \n",Version);
   printf("Nom1 : %s \n",Nom1);
   printf("Nom2 : %s \n",Nom2);
}

int RechercheME_RC(char *NomFichier, int Reference, struct VehiculeHV *UnRecord){
   //Ouverture fichier dans lequel rechercher le vehicule
   FILE *fichier = fopen(NomFichier, "r");
   if(fichier == NULL){
      perror("Erreur lors de l'ouverture du fichier");
      return -1;
   }
   fprintf(stderr, "(fonction recherche)Ouverture reussie VehiculesHV\n");

   int count = 0;

   while(1){
      if((count = fread(UnRecord, sizeof(struct VehiculeHV), 1, fichier)) == 0){
         fclose(fichier);
         return 0;      
      }
   
      if(UnRecord->Reference == Reference)
         break;
   }

   fclose(fichier);
   return 1;
}

int AchatME_RC(char *NomFichier, int Reference, int Quantite){
   //Ouverture fichier dans lequel rechercher le vehicule
   FILE *fichier = fopen(NomFichier, "r+b");
   if(fichier == NULL){
      perror("(fonction achat)Erreur lors de l'ouverture du fichier");
      return -1;
   }
   fprintf(stderr, "(fonction achat)Ouverture reussie VehiculesHV\n");
   
   //Recherche du vehicule voulu et modification de la quantite si trouvé
   int count;
   struct VehiculeHV UnRecord;
   while(1){
      if((count = fread(&UnRecord, sizeof(struct VehiculeHV), 1, fichier)) == 0)
         break;
   
      if(UnRecord.Reference == Reference){
         UnRecord.Quantite -= Quantite;
         fseek(fichier, -sizeof(struct VehiculeHV), SEEK_CUR);
         fwrite(&UnRecord, sizeof(struct VehiculeHV), 1, fichier);
         fflush(fichier);
         fclose(fichier);
         printf("(fonction achat) Mise à jour de la quantite réussie\n");
         return 0;
      }
   }
   
   fclose(fichier);
   return -1;
}

int FacturationME_RC(char *NomFichier, char NomClient[40], int quantite, int reference){
   FILE *fichier;
   struct FactureHV new_facture;

   //Ouverture du fichier de facturation
   if((fichier = fopen(NomFichier, "a")) == NULL){
      fprintf(stderr, "(fonction facturation) Echec ouverture\n");
      exit(EXIT_FAILURE);
   }
   else
      fprintf(stderr,"(fonction facturation) Ouverture du fichier %s reussie \n", NomFichier);
   
   int nombre_facture = ftell(fichier)/sizeof(struct FactureHV);
   fprintf(stderr, "(fonction facturation) Nombre facture déjà présente avant ajout: %d\n", nombre_facture);

   time_t heure_actuelle;
   time(&heure_actuelle);

   new_facture.NumeroFacturation = nombre_facture + 1;
   strcpy(new_facture.Acheteur, NomClient);
   new_facture.Quantite = quantite;
   new_facture.Reference = reference;
   new_facture.DateFacturation = heure_actuelle;


   fwrite(&new_facture, sizeof(struct FactureHV), 1, fichier);
   fflush(fichier);
   fclose(fichier);

   return new_facture.NumeroFacturation;
}