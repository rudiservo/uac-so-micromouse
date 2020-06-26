#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <pthread.h> 


#define MAP_X 66
#define MAP_Y 35
#define BUFFER_SIZE 1024


typedef struct positionXY {
    int x;
    int y;
} PositionXY;

typedef struct map {
    char **pos;
    struct positionXY start;
    struct positionXY current;
    struct positionXY new;
} Map;

typedef struct server_socket {
    int sock;
    int length;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in addr;
    struct sockaddr_in client;
} ServerSocket;

typedef struct buffer
{
    char data[BUFFER_SIZE];
    int count;

} Buffer;

pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Buffer buffer;
int count;


int server_start(ServerSocket * server) {
    /* Create socket from which to read. */
    server->sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server->sock == -1) {
        perror("opening datagram socket");
        exit(1);
    }
    
    /* Create servaddr with wildcards. */
    server->addr.sin_family = AF_INET;
    server->addr.sin_addr.s_addr = INADDR_ANY;
    //servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server->addr.sin_port = 0;
    //servaddr.sin_port = htons(8010);
    if (bind(server->sock,(struct sockaddr *) &server->addr, sizeof(server->addr)) == -1) {
        perror("binding datagram socket");
        exit(1);
    }
    /* Find assigned port value and print it out. */
    server->length = sizeof(server->addr);
    if (getsockname(server->sock,(struct sockaddr *) &server->addr, &server->length) == -1) {
        perror("getting socket name");
        exit(1);
    }
    printf("Socket port #%d\n", ntohs( server->addr.sin_port));
}

int server_receive(ServerSocket *server) {
    
    int n;
    int len;
    len = sizeof(server->client);
    n = recvfrom(server->sock, (char *)&buffer.data, BUFFER_SIZE, MSG_WAITALL, ( struct sockaddr *) &server->client, &len);
    // server->buffer[n] = '\0';
    // char answer =  (char) *server->buffer;
    // sendto(server->sock, &answer, sizeof(answer), MSG_CONFIRM, (const struct sockaddr *) &server->client,len);
    return n;
    //printf("-->%s\n", buf);
}

int server_responde(ServerSocket *server, char *answer)
{
    int len = sizeof(server->client);
    int n = sendto(server->sock, &answer, sizeof(answer), MSG_CONFIRM, (const struct sockaddr *) &server->client,len);
    return n;
}

int server_stop(struct server_socket * server) {
    close(server->sock);
}



/**
 * Have to do a map init because double vector char does not seem to work
 */
void map_init(Map *map, int x, int y)
{
    map->pos = malloc(y*sizeof(int *));
    for (int i = 0 ; i < y; i++) {
        map->pos[i] = malloc(x*sizeof(char));
    }
}

int map_read(struct map *map, char *filename)
{
    struct stat st;
    FILE *fp;
   
    
    stat(filename, &st);
    int MAXCHAR = st.st_size;
    
    char str[MAXCHAR];
    
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 0;
    }
    int j = 1;
    while (fgets(str, MAXCHAR, fp) != NULL) {
        for (int i = 0; i < strlen(str); i++){
            switch (str[i])
            {
                case '\n':
                    j++;
                    break;
                case 'S':
                    map->start.x = i+1;
                    map->start.y = j;
                default:
                    map->pos[j][i+1] = str[i];
                    // printf("\nX=%d, Y=%d, C=%c", i+1, j, map->pos[j][i+1]);
                    break;
            }
        }
    }
    map->current.x = map->start.x;
    map->current.y = map->start.y;
    map->new.x = map->start.x;
    map->new.y = map->start.y;
    // printf("\nfile size %d bytes\n", MAXCHAR);
    fclose(fp);
    
    return 1;
}

void map_print(Map *map) {
    printf("\033[2J");
    printf( "\033[0;0f");
    printf ("Position: %d, %d   \n", map->start.x, map->start.y);
    char str[MAP_X];
    for (int j = 1; j < MAP_Y; j++) {
        for(int i = 1; i< MAP_X; i++ ) {
            printf("%c", map->pos[j][i]);
        }
        if (j!= (MAP_Y - 1)) {
            printf("\n");
        }
    }
    // printf("\033[K");
}

void map_navigate(Map * map, int ch) {
    // printf("\033[K");
    char pos_type;
    printf("\033[%d;%dfClient : %c\n", 0,55, *buffer.data);
    switch (ch) {
        case 'w':
            --map->new.y;
            break;
        case 'a':
            --map->new.x;
            break;
        case 's':
            ++map->new.y;
            break;
        case 'd':
            ++map->new.x;
            break;
        default:
            break;
    }
    pos_type = map->pos[(map->new.y - 1)][map->new.x];
    printf ("\033[0;0fPosition: %d, %d   | %d, %d     %c", map->current.x, map->current.y, map->new.x, map->new.y, pos_type );
    if ((map->current.y != map->new.y || map->current.x != map->new.x) && pos_type == ' ') {
        printf("\033[%d;%df ", map->current.y, map->current.x);
        map->current.x = map->new.x;
        map->current.y = map->new.y;
        printf("\033[%d;%dfM", map->current.y, map->current.x);
    } else {
        map->new.x = map->current.x;
        map->new.y = map->current.y;
    }
    return;
}

int Map_GetSurroundings(char *answer, Map *map, int pos_x, int pos_y)
{
    /**
     * another send mutex
     * client also with thread for receiving only without any mutex
     *
     */
    // return snprintf(answer, sizeof(answer), "%s",;
}


void *serverProducer(void *server)
{
    ServerSocket *server1 = server;
    server_start(server1);
    while (1 == 1) {
        pthread_mutex_lock(&mutex);
        while (buffer.count != 0) {
            pthread_cond_wait(&empty, &mutex);
        }
        buffer.count = server_receive(server1);
        pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mutex);
    }
}

void *mapConsumer(void *map)
{
    while (1 == 1) {
        pthread_mutex_lock(&mutex);
        while (buffer.count == 0) {
            pthread_cond_wait(&fill, &mutex);
        }
        map_navigate(map, (int) *buffer.data);
        buffer.count = 0;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }

}

int main(int argc, char * argv[]) {
    ServerSocket * server = malloc(sizeof(server));
    Map * map1 = malloc(sizeof(map1)) ;
    char *filename = malloc(120);
    // create a buffer stack
    filename = argv[1];
    map_init(map1, MAP_X, MAP_Y);
    map_read(map1, filename);
    map_print(map1);
    // server_start(server);
    pthread_t producer;
    pthread_t consumer;
    buffer.count = 0;
    pthread_create(&producer, NULL, serverProducer, server);
    pthread_create(&consumer, NULL, mapConsumer, map1);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    // // server_read(server);

    // map_navigate(map1, server);
    printf("ending");
    exit(0);
}