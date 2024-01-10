typedef struct quest{
    int modec;
    int modet;
    char villedepart[100];
    char villearrive[100];
    char tempstot[100];
    char tempstard[100];
}Quest;//la demande

typedef struct train{
    char villedepart[100];
    char villearrive[100];
    char tempstot[100];
    char tempstard[100];
    double prix;
    int etat;
    int num;
}Train;//les infos d'un train

typedef struct trains{
    Train trains[100];
    int len;
}Trains;//un groupe de train

void fils(int socketServer,int sockerClient);//creer des fils pour répondre au client
void initrain(Train *t);//initialiser le train
void getinfostrain(Trains *t);//stocker les infos des trains d'après train.txt dans structure Trains
void getlisttrain(Trains *t,Quest q,int sockerClient);//analyser les infos et obtenir les trains disponible et les envoyer
int compareTime(char *t1,char *t2);
void getResultat(Train t,char *res);//reécrire le résultat en un format lisible
void iniquest(Quest *q);//initialiser la demande
void getinfosquest(Quest *q,char *dem);//stocker les infos viennent du client dans structure Quest
void end_child();//tuer des processus zombie
