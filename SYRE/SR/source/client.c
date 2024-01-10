
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

int main(int argc,char *argv[]){ 
    // Vérification du nombre d'arguments en ligne de commande
    if(argc!=2&&argc!=3){
        printf("La connection a échoué!");
        exit(0);
    }
    // Initialisation des variables
    Client;
    struct sockaddr_in clientAddr;
    int port=atoi(argv[1]);
    struct hostent *server;
    char nom[30] = "localhost"; //par defaut 
    // Modification du nom du serveur si fourni en ligne de commande
    if(argc==3){
        strcpy(nom,argv[2]);
        //printf("%s",argv[2]);
    }
    //printf("%d",argc);

    // Création de la socket client
    sockerClient=socket(AF_INET,SOCK_STREAM,0);
    if(sockerClient==-1){
        printf("La connection a échoué!");
        exit(0);
    }else{
        //printf("114514");
    }

    // Récupération des informations sur le serveur
    server=gethostbyname(nom);
    if(server==NULL){
        printf("La connection a échoué!");
        exit(0);
    }else{
        //printf("114514");
    }
    // Configuration de la structure de l'adresse du client
    clientAddr.sin_family=AF_INET;
    clientAddr.sin_port=htons(port);
    memcpy(&clientAddr.sin_addr.s_addr,server->h_addr,sizeof(struct in_addr));
    // Connexion au serveur
    if(connect(sockerClient,(struct sockaddr*)&clientAddr,sizeof(clientAddr))==-1){
        printf("La connection a échoué!");
        exit(0);
    }else{
        printf("La connection est réussie!");
    }

    int end=0;
    while(end!=1){
        // Affichage du menu de recherche
        printf("\nVous voulez chercher les infos d'après :\n");
        printf("Mode 1: avec le temps precisé\n");
        printf("Mode 2: avec une période du temps\n");
        printf("Mode 3: sans indiquer le temps\n");
        printf("Pour quitter, veuillez taper 4\n");
        char choix='\0';
        do{
            char choixs[100]="";
            printf("Le numéro de votre choix :");
            scanf("%s",choixs);
            //printf("%zu\n",strlen(choixs));
            if(strlen(choixs)==1){
                choix=choixs[0];
            }
            //printf("%d",choix);
        }while(choix!='1'&&choix!='2'&&choix!='3'&&choix!='4');
        if(choix=='4'){
            printf("Au revoir!\n");
            exit(0);
        }
        //Quest demande;
        //iniquest(demande);
        //demande.modec=choix;
        char *villedepart=(char *)malloc(sizeof(char)*100);
        char *villearrive=(char *)malloc(sizeof(char)*100);
        char *dem=(char *)malloc(sizeof(char)*100);
        memset(dem,0,sizeof(char)*100);
        printf("Votre ville de départ:\n");
        scanf(" %99[^\n]",villedepart);
        printf("Votre ville d'arrivée:\n");
        scanf(" %99[^\n]",villearrive);
        // Construction de la demande
        sprintf(dem+strlen(dem),"%c",choix);
        strcat(dem,";");
        strcat(dem,villedepart);
        strcat(dem,";");
        strcat(dem,villearrive);
        strcat(dem,";");
        // Appel à la fonction getinfos
        getinfos(choix-'0',dem);
        write(sockerClient,dem,sizeof(char)*100);

        //printf("Fin de recherche ?\n1: oui:\n0: non\n");
        /*char choix2='\0';
        do{
            char choixs2[100]="";
            printf("La numéro de votre choix :");
            scanf("%s",choixs2);
            if(strlen(choixs2)==1){
                choix2=choixs2[0];
            }
            //printf("%d",choix);
        }while(choix2!='1'&&choix2!='0');
        end=choix2-'0';
        write(sockerClient,&end,sizeof(int));*/
        char *res=(char *)malloc(sizeof(char)*2000);
        int nb=-1;
        ssize_t sread;
        sread=read(sockerClient,&nb,sizeof(int));
        /*if(sread==0){
            printf("Serveur mort");
            end=0;
            exit(0);
        };*/
        //printf("%d",nb);
        if(nb==0){
            printf("\n----------------------------\n");
            printf("Résultat :\nPas de train disponible.");
            printf("\n----------------------------\n");
        }else if(nb==-1){
            printf("Serveur mort");
            end=0;
            exit(0);
        }else{
            printf("\n----------------------------\n");
            read(sockerClient,res,sizeof(char)*2000);
            printf("Résultat :\n%s",res);
        }
        free(res);
        free(villearrive);
        free(villedepart);
        free(dem);

        printf("\nFin de recherche ?\n1: oui:\n0: non\n");
        char choix2='\0';
        do{
            char choixs2[100]="";
            printf("Le numéro de votre choix :");
            scanf("%s",choixs2);
            if(strlen(choixs2)==1){
                choix2=choixs2[0];
            }
            //printf("%d",choix);
        }while(choix2!='1'&&choix2!='0');
        end=choix2-'0';
        // Envoi de la demande de fin au serveur
        write(sockerClient,&end,sizeof(int));
        int i;
        // pour verifier que le seveur exsite encore
        sread=read(sockerClient,&i,sizeof(int));
        if(sread==0){
            printf("Serveur mort");
            end=0;
            exit(0);
        };
    }

    return 0;
}