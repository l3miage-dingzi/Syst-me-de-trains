#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include "../header/fonc_ser.h"
// Fonction pour gérer la fin des processus fils
void end_child(){//tuer des processus zombie
    wait(NULL);
}

void fils(int socketServer,int sockerClient){//creer des fils pour répondre au client
    switch (fork())
    {
    case -1:
        printf("nmsl7");
        break;
    case 0:
        int end=0;
        while(end!=1){
            Trains *t;
            t=(Trains *)malloc(sizeof(Trains)*100);
            t->len=0;       
            getinfostrain(t);
            char *dem=(char *)malloc(sizeof(char)*100);
            ssize_t sread;
            sread=read(sockerClient,dem,sizeof(char)*100);
            if(sread==0){
                printf("Client mort");
                end=0;
                exit(0);
            };
            Quest demande;
            iniquest(&demande);
            getinfosquest(&demande,dem);
            getlisttrain(t,demande,sockerClient);
            
            free(t);
            free(dem);
            sread=read(sockerClient,&end,sizeof(int));
            if(sread==0){
                printf("Client mort");
                end=0;
                exit(0);
            };
            write(sockerClient,&end,sizeof(int));
        }printf("Fin demande");
        exit(0);
        break;
    default:
        close(sockerClient);
        break;
    }
}

void initrain(Train *t){//initialiser le train
    t->num=0;
    t->etat=0;
    t->prix=0.0;
    strcpy(t->villedepart,"");
    strcpy(t->villearrive,"");
    strcpy(t->tempstot,"");
    strcpy(t->tempstard,"");
}



void getinfostrain(Trains *t){//stocker les infos des trains d'après train.txt dans structure Trains
    //printf("nmsl8");
    char b[2000];
    char* token;
    FILE *file;
    file=fopen("train.txt","r");
    if(file==NULL){
        printf("Pas de fichier train.txt");
        exit(0);
    }

    while(fgets(b,sizeof(b),file)!=NULL){
        token=strtok(b,";");
        //printf("seg:%s\n",token);
        //t.num=atoi(token);
        int i=1;
        initrain(&t->trains[t->len]);
        while(token!=NULL){
            //printf("seg:%s\n",token);
            switch(i){
            case 1:
                int j=atoi(token);
                t->trains[t->len].num=j;
                break;
            case 2:
                strcpy(t->trains[t->len].villedepart,token);
                break;
            case 3:
                strcpy(t->trains[t->len].villearrive,token);
                break;
            case 4:
                strcpy(t->trains[t->len].tempstot,token);
                break;
            case 5:
                strcpy(t->trains[t->len].tempstard,token);
                break;
            case 6:
                char *endpdr;
                t->trains[t->len].prix=strtod(token,&endpdr);
                break;
            case 7:
            //printf(",%d",strcmp(token,"REDUC\n"));
                if(strcmp(token,"REDUC\n")==0){
                    t->trains[t->len].etat=1;
                }else if(strcmp(token,"SUPPL\n")==0){
                    t->trains[t->len].etat=2;
                }
                break;
            }
            i++;
            token=strtok(NULL,";");

        }
        t->len++;
        //printf("num:%d",t->len);
    //t.num=atoi(strtok(b,";"));
    //printf("123%s",strtok(b,";"));
    //strcpy(t.villedepart,strtok(b,";"));
    }
    fclose(file);
    //free(b);
    //printf("vs%s",t->trains[t->len]);
    //printf("vs%d",t->trains[t->len]);
    //printf("456%d",t.num);
}

void getinfosquest(Quest *q,char *dem){//stocker les infos viennent du client dans structure Quest
    char* token;
    token=strtok(dem,";");
    int i=1;
    while(token!=NULL){
        //printf("seg:%s\n",token);
        switch(i){
            case 1:
                int j=atoi(token);
                q->modec=j;
                break;
            case 2:
                strcpy(q->villedepart,token);
                //printf("seg:%s\n",q->villedepart);
                break;
            case 3:
                strcpy(q->villearrive,token);
                //printf("seg:%s\n",q->villearrive);
                break;
            default :
                switch(q->modec){
                    case 1:
                        if(i==4){
                            strcpy(q->tempstot,token);
                        }else if(i==5){
                            int j=atoi(token);
                            q->modet=j;
                        }
                        break;
                    case 2:
                        if(i==4){
                            strcpy(q->tempstot,token);
                        }else if(i==6){
                            int j=atoi(token);
                            q->modet=j;
                        }else if(i==5){
                            strcpy(q->tempstard,token);
                        }
                        break;
                    case 3:
                        if(i==4){
                            int j=atoi(token);
                            q->modet=j;
                        }
                        break;
                    default:
                        break;
                }
                break;
            }
        i++;
        token=strtok(NULL,";");
    }
    //printf("seg:%s\n",q->villedepart);
}

void getlisttrain(Trains *t,Quest q,int sockerClient){//analyser les infos et obtenir les trains disponible et les envoyer
    int list[t->len];
    int lenlist=0;
    for(int i=0;i<t->len;i++){
        //printf("%s,%s\n",q.villedepart,t->trains[t->i].villedepart);
        if(strcmp(q.villedepart,t->trains[i].villedepart)==0&&strcmp(q.villearrive,t->trains[i].villearrive)==0){
            list[lenlist]=i;
            lenlist++;
            
        }
    }
    int listtemp[lenlist];
    double listprix[lenlist];
    int listorder[lenlist];
    int min=0;
    char *res=(char *)malloc(sizeof(char)*2000);
    strcpy(res,"");
    int lentp=0;
    switch (q.modec)
    {
    case 1:
        for(int i=0;i<lenlist;i++){
            listtemp[i]=compareTime(q.tempstot,t->trains[list[i]].tempstot);
            lentp++;
            //printf("%s,%s",q.tempstot,t->trains[list[i]].tempstot);
            //printf("%d",listtemp[i]);
        }
        for(int i=0;i<lenlist;i++){
            if(listtemp[i]>=0){
                if(listtemp[i]<listtemp[min]||listtemp[min]<0){
                   min=i; 
                }
                
            }
        }
        if(listtemp[min]<0){
            lentp=0;
        }
        //printf("%d",min);
        
        //strcpy(res,"Résultat :\n"+"Numéro de train :"+t->trains[i].num)
        break;
    
    case 2:
        for(int i=0;i<lenlist;i++){
            //listtemp[i]=compareTime(q.tempstot,t->trains[list[i]].tempstot);
            //printf("%s,%s",q.tempstot,t->trains[list[i]].tempstot);
            //printf("%d",listtemp[i]);
            int t1=compareTime(q.tempstot,t->trains[list[i]].tempstot);
            int t2=compareTime(q.tempstard,t->trains[list[i]].tempstot);
            int t3=compareTime(t->trains[list[i]].tempstot,t->trains[list[i]].tempstard);
            if(t1>=0&&t2<=0){
                listtemp[lentp]=t3;
                //printf("%f,%d\n",t->trains[list[i]].prix,t->trains[list[i]].etat);
                if(t->trains[list[i]].etat==0){
                    listprix[lentp]=t->trains[list[i]].prix;
                }else if(t->trains[list[i]].etat==1){
                    listprix[lentp]=(t->trains[list[i]].prix)*0.8;
                }else if(t->trains[list[i]].etat==2){
                    listprix[lentp]=(t->trains[list[i]].prix)*1.1;
                }
                listorder[lentp]=list[i];
                lentp++;
            }
        }
            if(q.modet==2){
                for(int i=0;i<lentp;i++){
                    for(int j=0;j<lentp-1;j++){
                        if(listtemp[j]>listtemp[j+1]){
                            int temp=listtemp[j];
                            listtemp[j]=listtemp[j+1];
                            listtemp[j+1]=temp;
                            temp=listorder[j];
                            listorder[j]=listorder[j+1];
                            listorder[j+1]=temp;
                        }
                    }
                }
            
            }else if(q.modet==1){
                for(int i=0;i<lentp;i++){
                    for(int j=0;j<lentp-1;j++){
                        if(listprix[j]>listprix[j+1]){
                            double temp1=listprix[j];
                            listprix[j]=listprix[j+1];
                            listprix[j+1]=temp1;
                            int temp2=listorder[j];
                            listorder[j]=listorder[j+1];
                            listorder[j+1]=temp2;
                        }
                    }
                }
            }
            /*g[0]=lentp;
            for(int i=0;i<lentp;i++){
                printf("%d,",listorder[i]);
                g[i+1]=listorder[i];
            }*/
        
        break;

    case 3:
        for(int i=0;i<lenlist;i++){
            //listtemp[i]=compareTime(q.tempstot,t->trains[list[i]].tempstot);
            //printf("%s,%s",q.tempstot,t->trains[list[i]].tempstot);
            //printf("%d",listtemp[i]);
            //int t1=compareTime("0:0",t->trains[list[i]].tempstot);
            int t1=compareTime(t->trains[list[i]].tempstot,t->trains[list[i]].tempstard);
            //int t2=compareTime(q.tempstard,t->trains[list[i]].tempstot);
            //if(t1>0&&t2<0){F
                listtemp[i]=t1;
                //printf("%f,%d\n",t->trains[list[i]].prix,t->trains[list[i]].etat);
                if(t->trains[list[i]].etat==0){
                    listprix[i]=t->trains[list[i]].prix;
                }else if(t->trains[list[i]].etat==1){
                    listprix[i]=(t->trains[list[i]].prix)*0.8;
                }else if(t->trains[list[i]].etat==2){
                    listprix[i]=(t->trains[list[i]].prix)*1.1;
                }
                //listorder[lentp]=list[i];
                lentp++;
            //}
        }
            if(q.modet==2){
                for(int i=0;i<lenlist;i++){
                    for(int j=0;j<lenlist-1;j++){
                        if(listtemp[j]>listtemp[j+1]){
                            int temp=listtemp[j];
                            listtemp[j]=listtemp[j+1];
                            listtemp[j+1]=temp;
                            temp=list[j];
                            list[j]=list[j+1];
                            list[j+1]=temp;
                        }
                    }
                }
            
            }else if(q.modet==1){
                for(int i=0;i<lenlist;i++){
                    for(int j=0;j<lenlist-1;j++){
                        if(listprix[j]>listprix[j+1]){
                            double temp1=listprix[j];
                            listprix[j]=listprix[j+1];
                            listprix[j+1]=temp1;
                            int temp2=list[j];
                            list[j]=list[j+1];
                            list[j+1]=temp2;
                        }
                    }
                }
            }
            /*g[0]=lenlist;
            for(int i=0;i<lenlist;i++){
                //printf("%d,\n",lenlist);
                g[i+1]=list[i];
                
            }*/
            //lentp=lenlist;
        break;

    default:
        break;
    }
    if(q.modec==1){
        getResultat(t->trains[list[min]],res);
        //printf("%s",res);
        write(sockerClient,&lentp,sizeof(int));
    }else if(q.modec==2){
        for(int i=0;i<lentp;i++){
            getResultat(t->trains[listorder[i]],res);
        }
        //printf("%s",res);
        write(sockerClient,&lentp,sizeof(int));
    }else if(q.modec==3){
        //printf("\nres%d\n",lenlist);
        for(int i=0;i<lentp;i++){
            getResultat(t->trains[list[i]],res);
        }
        //printf("%s",res);
        //printf("%d",lentp);
        write(sockerClient,&lentp,sizeof(int));
    }
    //int ex=0;
    //printf("%d",lenlist);
    //write(sockerClient,strcmp(res,""),sizeof(int));
    if(lentp!=0){
        printf("%s",res);
        write(sockerClient,res,sizeof(char)*2000);
    }
    


    free(res);
    //
    //printf("res%s",res);
}

int compareTime(char *t1,char *t2){
    int h1,m1,h2,m2;
    sscanf(t1,"%d:%d",&h1,&m1);
    sscanf(t2,"%d:%d",&h2,&m2);
    return (h2-h1)*60+(m2-m1);
}

void getResultat(Train t,char *rest){//reécrire le résultat en un format lisible
    char *res=(char *)malloc(sizeof(char)*300);
    char *rese=(char *)malloc(sizeof(char)*300);
    memset(res,0,sizeof(char)*300);
    memset(rese,0,sizeof(char)*300);
    sprintf(res,"Numéro de train :%d\nVille départ :%s\nVille arrive :%s\nTemps départ :%s\nTemps arrive :%s\nPrix :%.2f\n",t.num,t.villedepart,t.villearrive,t.tempstot,t.tempstard,t.prix);
    if(t.etat==1){
        sprintf(rese,"Etat :Réduction\nPrix actuellement :%.2f\n",t.prix*0.8);
    }else if(t.etat==2){
        sprintf(rese,"Etat :Supplément\nPrix actuellement :%.2f\n",t.prix*1.1);
    }
    strcat(res,rese);
    strcat(rest,res);
    strcat(rest,"\n----------------------------\n");
    //printf("%s",res);
    free(rese);
    free(res);
}

void iniquest(Quest *q){//initialiser la demande
    q->modec=0;
    q->modet=0;
    memset(q->tempstard,0,sizeof(q->tempstard));
    memset(q->tempstot,0,sizeof(q->tempstot));
    memset(q->villearrive,0,sizeof(q->villearrive));
    memset(q->villedepart,0,sizeof(q->villedepart));
}

