#include <stdio.h>
#include <pthread.h>
#define MAXBUFFER 3

void *client_thread(void *);
void *server_thread(void *);
void run_client();
pthread_t    tid[4], server;
int is_first = 0;
pthread_mutex_t insert_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_broad = PTHREAD_COND_INITIALIZER;
pthread_cond_t server_sig = PTHREAD_COND_INITIALIZER;
char *brd_msg = "BROADCAST MSG";

void run_client(){
    char next[10];
    printf("insert any char\n");
    scanf("%s", next);
    pthread_cond_signal(&server_sig);
}

int main() {
    int i;
    int id[4];
    
    for(i = 0; i < 4; i++){
        id[i] = i;
        pthread_create(&tid[i],NULL,client_thread,(void*)&id[i]);
    }
    pthread_create(&server,NULL,server_thread,NULL);
    for(i = 0; i < 4 ; i++){
        pthread_join(tid[i], NULL);
    }

    return 0;
}
void *server_thread(void *arg)
{
    pthread_mutex_lock(&server_mutex);	
    pthread_cond_wait(&server_sig, &server_mutex);
    pthread_cond_broadcast(&client_broad);
    pthread_mutex_unlock(&server_mutex);	
}
 
void *client_thread(void *arg){
    if(is_first == 0) {
        is_first = 1;
        run_client();
    }
    else{
        pthread_cond_wait(&client_broad, &client_mutex);

	    printf(" %d thread receive broadcast: %s\n", *((int*)arg), brd_msg);
		pthread_mutex_unlock(&client_mutex);
    }
}
