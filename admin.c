/******************************************
  Herman Vanstapel
  2017 Basé sur Fichiers
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "LibSerME_RC.h"

// include pour les fonctions entrees sortie
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

static struct termios old, new;
 
//clrscr() function definition
// #include <stdlib.h>
void ClrScr(void)
{
   system("clear");
}

void Gotoxy(int x,int y)    
{
   printf("%c[%d;%df",0x1B,y,x);
}

/* Initialize new terminal i/o settings */
// #include <stdio.h>
// #include <stdlib.h>

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



// permet de supprimer le cr qui placé dans la chaine lors d'un fgets
void DelNewLine(char *Chaine)
{
   Chaine[strlen(Chaine)-1] = 0 ;
}



void MonPrintf(char* tempo, int espace,int taille )
{
   int Count ;
   printf("%s",tempo) ; 
   Count = espace-taille ;
   while ( Count>0 ){
      printf(" ");
      Count-- ;
   }
}

void AfficheEnteteVehiculeHV ()
{
   char Tampon[80] ;
   sprintf(Tampon,"%s","Ref" ) ;MonPrintf(Tampon,4,strlen(Tampon)) ;
   sprintf(Tampon,"%s","Constructeur") ;    MonPrintf(Tampon,25,strlen(Tampon)) ;
   sprintf(Tampon,"%s","Modele") ;    MonPrintf(Tampon,20,strlen(Tampon)) ;
   sprintf(Tampon,"%s","Quantite") ; MonPrintf(Tampon,15,strlen(Tampon)) ;
   sprintf(Tampon,"%s","Puissance") ; MonPrintf(Tampon,6,strlen(Tampon)) ;
   printf("\n") ;
}

void AfficheVehiculeHV (struct VehiculeHV *UnRecord)
{
   char Tampon[80] ;
   sprintf(Tampon,"%d",UnRecord->Reference ) ;  MonPrintf(Tampon,4,strlen(Tampon)) ;
   sprintf(Tampon,"%s",UnRecord->Constructeur ) ;    MonPrintf(Tampon,25,strlen(Tampon)) ;
   sprintf(Tampon,"%s",UnRecord->Modele ) ;    MonPrintf(Tampon,20,strlen(Tampon)) ;
   sprintf(Tampon,"%d",UnRecord->Quantite ) ; MonPrintf(Tampon,15,strlen(Tampon)) ;
   sprintf(Tampon,"%d",UnRecord->Puissance ) ; MonPrintf(Tampon,6,strlen(Tampon)) ;
   printf("\n") ;
}

void SaiSieVehiculeHV (int Reference, struct VehiculeHV  *UnRecord )
{
   char Tampon[80] ;

   printf("Reference :%d \n",Reference) ;
   UnRecord->Reference = Reference ; 
   printf("Saisie Constructeur :") ;
   fgets(UnRecord->Constructeur ,sizeof UnRecord->Constructeur ,stdin ) ;
   printf("Saisie Modele :") ;
   fgets(UnRecord->Modele ,sizeof UnRecord->Modele ,stdin ) ;
   printf("Saisie Quantite :") ;
   fgets(Tampon,sizeof Tampon,stdin ) ;
   UnRecord -> Quantite = atoi(Tampon) ;
   printf("Saisie Puissance :");
   fgets(Tampon, sizeof Tampon, stdin);
   UnRecord->Puissance = atoi(Tampon);

   DelNewLine(UnRecord->Constructeur) ;
   DelNewLine(UnRecord->Modele) ;
   AfficheEnteteVehiculeHV () ;
   AfficheVehiculeHV (UnRecord) ;
   printf("-----------------------\n") ;
   return ;
}

int NombreVehiculesHV (char *NomFichier)
{
   FILE *sortie ;
   sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
   if ( sortie == NULL ){
      fprintf(stderr,"Pas de fichier\n") ;
      return 0 ;
   }
   else
      fprintf(stderr,"Ouverture reussie \n") ;
   fseek(sortie, 0, SEEK_END ) ;
   return (ftell(sortie)/ sizeof(struct VehiculeHV  )) ;
}

void CreationAjoutVehiculeHV (char *NomFichier,struct VehiculeHV  *UnRecord )
{
   FILE *sortie ;
   char Redo ;
   int nbr ;

   sortie = fopen(NomFichier,"a") ; /* Si le fichier existe, on le cree sinon on ajoute */
   if ( sortie == NULL ){
      fprintf(stderr,"Echec Ouverture\n") ;
      exit(0) ;
   }
   else
      fprintf(stderr,"Ouverture reussie \n") ;

   
   printf("Position actuelle dans le fichier %ld\n",ftell(sortie)) ;
   UnRecord-> Supprime = 0 ;
   nbr = fwrite(UnRecord,sizeof(struct VehiculeHV ),1,sortie) ;
   fflush(sortie);
   fclose(sortie);
}



void AfficheFacture(struct FactureHV *UneFacture)
{
   char Tampon[80] ;
   sprintf(Tampon,"%d",UneFacture->NumeroFacturation ) ; MonPrintf(Tampon,4,strlen(Tampon)) ;
   sprintf(Tampon,"%s",UneFacture->Acheteur ) ; MonPrintf(Tampon,20,strlen(Tampon)) ;
   sprintf(Tampon,"%d",UneFacture->Quantite  ) ; MonPrintf(Tampon,4,strlen(Tampon)) ;
   sprintf(Tampon,"%d",UneFacture->Reference) ; MonPrintf(Tampon,4,strlen(Tampon)) ;
   
   struct tm *local_time = localtime(&UneFacture->DateFacturation);
   strftime(Tampon, sizeof(Tampon), "%H:%M", local_time);MonPrintf(Tampon,4,strlen(Tampon)) ;
   printf("\n") ;
}

void ListingVehiculesHV (char *NomFichier)
{
   struct VehiculeHV   UnRecord ;
   FILE *sortie ;
   char Tampon[80] ;
   int  Numero ;
   int  nbr ;

   sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
   if ( sortie == NULL ){
      fprintf(stderr,"Echec Ouverture\n") ;
      exit(0) ;
   }
   else
      fprintf(stderr,"Ouverture reussie \n") ;

   AfficheEnteteVehiculeHV () ;
   nbr = fread(&UnRecord,sizeof(UnRecord),1,sortie) ;
   
   while ( !feof(sortie) ){
      fprintf(stderr,"Record lu %d et Position actuelle dans le fichier %ld\n",nbr,ftell(sortie)) ;
      AfficheVehiculeHV ( &UnRecord) ;
      nbr = fread(&UnRecord,sizeof(UnRecord),1,sortie) ;
   }
   fclose(sortie) ;
}

void ListingFacturesHV(char *NomFichier)
{
   struct FactureHV UneFacture ;
   FILE *sortie ;
   char Tampon[80] ;
   int  Numero ;
   int  nbr ;

   sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
   if ( sortie == NULL ){
      fprintf(stderr,"Echec Ouverture\n") ;
      exit(0) ;
   }
   else
      fprintf(stderr,"Ouverture reussie \n") ;

   
   nbr = fread(&UneFacture,sizeof(struct FactureHV),1,sortie) ;
   
   while ( !feof(sortie) ){
      fprintf(stderr,"Record lu %d et Position actuelle dans le fichier %ld\n",nbr,ftell(sortie)) ;
      AfficheFacture( &UneFacture) ;
      nbr = fread(&UneFacture,sizeof(struct FactureHV ),1,sortie) ;
   }
   fclose(sortie) ;
}

int main(){
   //Affichage noms prénoms
   AProposServeurHV("Step1", "Monteleone Eliana", "Russe Cyril");


   char Choix ;
   char Tampon[80] ;
   int res ;
   struct VehiculeHV  UnRecord ;
   int Numero ;
   while(1){

      printf("-------2022-----------\n");
      printf("1) Ajout              \n");
      printf("2) Vehicule           \n"); 
      printf("4) Recherche          \n");   
      printf("5) Achat              \n");  
      printf("6) Factures           \n");
      printf("7) A propos           \n");
      printf("8) exit               \n"); 
      printf("----------------------\n"); 
      printf(">>") ; Choix=GetchE() ; printf("\n") ;

      switch(Choix){
         case '1': {
            FILE *sortie ;
            char Redo ;
            
            Redo='y' ;
            while ( Redo=='Y' || Redo=='y'){ 
               int Nombre ;
               Nombre= NombreVehiculesHV ("VehiculesHV");
               SaiSieVehiculeHV (Nombre+1, &UnRecord );
               CreationAjoutVehiculeHV ("VehiculesHV",&UnRecord); 
               printf("Encoder un autre (Y/N) ?)");
               printf(">>"); Redo=GetchE(); printf("\n");
            }
      
            break;	
         }
         case '2': ListingVehiculesHV ("VehiculesHV");
            break;
         case '4': {
            int ref, ch;

            // Demande ref du vehicule à rechercher à l'utilisateur
            printf("Veuillez entrer la référence du véhicul à rechercher\n>>");
            fgets(Tampon, sizeof(Tampon), stdin);
            ref = atoi(Tampon);
            
            //Recherche le véhicule en question et l'affiche s'il est trouvé
            if(ref > 0 && RechercheME_RC("VehiculesHV", ref, &UnRecord) == 1)
               AfficheVehiculeHV(&UnRecord);
            else 
               printf("\nVehicule introuvable\n\n");
            break ;
         }
         case '5':{
            char NomClient[40];
            int ref;
            int quantite;
            // Demande information pour achat
            printf("\nNomClient:");
            fgets(Tampon, sizeof(Tampon), stdin);
            strcpy(NomClient, Tampon);
            printf("\nReference:");
            fgets(Tampon, sizeof(Tampon), stdin);
            ref = atoi(Tampon);
            printf("\nQuantite:");
            fgets(Tampon, sizeof(Tampon), stdin);
            quantite = atoi(Tampon);
         
            if(ref > 0 && RechercheME_RC("VehiculesHV", ref, &UnRecord) == 1){
               fprintf(stderr, "Trouve %s, %s Quantité %d\n", UnRecord.Constructeur, UnRecord.Modele, UnRecord.Quantite);
               if(AchatME_RC("VehiculesHV", ref, quantite)){
                  fprintf(stderr, "Echec d'achat\n");
               }
               else{
                  FacturationME_RC("FactureHV", NomClient, quantite, ref);
               }
            }
            break;
         }
         case '6': ListingFacturesHV("FactureHV");
            break;
         case '7': AProposServeurHV("V 1","Monteleone Eliana","Russe Cyril");
            break;
         
         
         case '8': exit(0);
      }
   }
}
