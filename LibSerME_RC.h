#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"

void AProposServeurHV(char *Version,char *Nom1,char* Nom2);

int RechercheME_RC(char *NomFichier, int Reference, struct VehiculeHV *UnRecord);

int AchatME_RC(char *NomFichier, int Reference, int Quantite);

int FacturationME_RC(char *NomFichier, char NomClient[40], int quantite, int reference);