#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include "../header/fonc_cli.h"

// Fonction principale pour obtenir des informations en fonction du choix de l'utilisateur
void getinfos(int choix ,char *res){
    char *tempstart=(char *)malloc(sizeof(char)*100);
    char *tempstop=(char *)malloc(sizeof(char)*100);
    switch (choix)
    {
    case 1:
    // Obtenir l'heure de départ la plus tôt
        gettempstart(tempstart,1);
        strcat(res,tempstart);
        strcat(res,";");
        /* code */
        break;
    case 2:
    // Obtenir l'heure de départ la plus tôt et la plus tard
        gettempstart(tempstart,1);
        gettempstart(tempstop,2);
        strcat(res,tempstart);
        strcat(res,";");
        strcat(res,tempstop);
        strcat(res,";");
        break;
    case 3:
        break;
    }
    char choix2='\0';    
    // Si le choix n'est pas 1, demander à l'utilisateur de spécifier le critère de recherche
    if(choix!=1){
        printf("Votre résultat :\n");
        printf("1: Le trajet au meilleur prix\n");
        printf("2: Le trajet de dureé optimum\n");
        // Demander à l'utilisateur de saisir son choix
        do{
            printf("Le numéro de votre choix :");
            char choixs2[100]="";
            scanf("%s",choixs2);            
            // Vérifier si l'entrée est un seul caractère
            if(strlen(choixs2)==1){
                    choix2=choixs2[0];
            }
            //scanf("%c",&choix2);
            //printf("%d",choix);
        }while(choix2!='1'&&choix2!='2');
    }else{        
        // Si le choix est 1, choisir automatiquement l'option 1
        choix2='1';
    }    
    // Ajouter le choix de l'utilisateur à la réponse
    sprintf(res+strlen(res),"%c",choix2);
    //strcat(res,choix);
    //q->modet=choix;
    free(tempstart);
    free(tempstop);
}

void gettempstart(char *tempstart,int m){
        int heure=-1;
        int minutes=-1;
        if(m==1){
            printf("A quelle heure prévoyez-vous de partir au plus tot:\n");
            printf("Indiquer l'heure de départ plus tot:\n");
        }else{
            printf("A quelle heure prévoyez-vous de partir au plus tard\n");
            printf("Indiquer l'heure de départ plus tard:\n");
        }    
        // Saisir l'heure et vérifier la validité de l'entrée
        scanf("%d", &heure);
        while(heure<0||heure>23){
            if(m==1){
                printf("Indiquer l'heure de départ plus tot:\n");
            }else{
                printf("Indiquer l'heure de départ plus tard:\n");
            }        
            // Gérer les entrées incorrectes
            int c;
            while((c=getchar())!='\n'&&c!=EOF){

            }
            scanf("%d", &heure);
        }   
        // Saisir les minutes et vérifier la validité de l'entrée
        if(m==1){
            printf("Indiquer les minutes de départ plus tot:\n");
        }else{
            printf("Indiquer les minutes de départ plus tard:\n");
        }
        scanf("%d", &minutes);
        while(minutes<0||minutes>59){
            if(m==1){
                printf("Indiquer les minutes de départ plus tot:\n");
            }else{
                printf("Indiquer les minutes de départ plus tard:\n");
            }
            int c;
            while((c=getchar())!='\n'&&c!=EOF){
                
            }
            scanf("%d", &minutes);
        }
        sprintf(tempstart,"%d:%d",heure,minutes);
        //printf("\n123%s",tempstart);
}