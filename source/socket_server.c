#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define MAP_X 66
#define MAP_Y 35

struct positionXY {
    int x;
    int y;
};

struct map {
    char **pos;
    struct positionXY start;
};

struct server_socket {
    int sock;
    int length;
    char buffer[1024];
    struct sockaddr_in addr;
    struct sockaddr_in client;
};


int server_start(struct server_socket *server) {
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
    if (bind(server->sock,(struct sockaddr *)&server->addr, sizeof(server->addr)) == -1) {
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

int server_read(struct server_socket *server) {
    
    int n;
    int len;
    len = sizeof(server->client);
    n = recvfrom(server->sock, (char *)server->buffer, 1024,
                    MSG_WAITALL, ( struct sockaddr *) &server->client,
                    &len);
        server->buffer[n] = '\0';
        char answer =  (char) *server->buffer;
        sendto(server->sock, &answer, sizeof(answer),
            MSG_CONFIRM, (const struct sockaddr *) &server->client,
                len);
    

    return 1;
    //printf("-->%s\n", buf);
}

int server_stop(struct server_socket *server) {
    close(server->sock);
}

void map_init(struct map *map, int x, int y)
{
    map->pos = malloc(y*sizeof(int *));
    for (int i = 0 ; i < y; i++) {
        map->pos[i] = malloc(x*sizeof(char));
    }
}


int map_read(struct map *map, char *filename)
{
    int MAXCHAR = 100000;
    struct stat st;
    FILE *fp;
   
    
    stat(filename, &st);
    MAXCHAR = st.st_size;
    
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
                    // printf("\nX=%d, Y=%d, C=%c", j, i, map->pos[j][i]);
                    break;
            }
        }
    }
    printf("\nfile size %d bytes\n", MAXCHAR);
    fclose(fp);
    return 1;
}

void map_print(struct map *map) {
    printf("\033[2J");
    printf( "\033[0;0f");
    printf ("Position: %d, %d   \n", map->start.x, map->start.y);
    char str[MAP_X];
    for (int j = 1; j < MAP_Y; j++) {
        for(int i = 1; i< MAP_X; i++ ) {
            printf("%c", map->pos[j][i]);
        }
        if (j!= (MAP_Y - 2)) {
            printf("\n");
        }
    }
    printf("\033[K");
}

void map_navigate(struct map *map, struct server_socket *server) {
    // printf("\033[K");
    int ch;
    int pos_x = map->start.x;
    int pos_y = map->start.y + 1;
    int pos_x_new = pos_x;
    int pos_y_new = pos_y;
    char pos_type;
    printf( "\033[%d;%dfM", pos_y, pos_x);

    while (server_read(server)) {
        ch = (int)*server->buffer;
        printf("\033[%d;%dfClient : %s\n", 0,55, server->buffer);
        switch (ch) {
            case 'w':
                --pos_y_new;
                break;
            case 'a':
                --pos_x_new;
                break;
            case 's':
                ++pos_y_new;
                break;
            case 'd':
                ++pos_x_new;
                break;
            default:
                break;

        }
        pos_type = map->pos[(pos_y_new - 1)][pos_x_new];
        if ((pos_x != pos_x_new || pos_y_new != pos_y) && (pos_type == ' ' || pos_type == 'S')) {
            printf("\033[%d;%df ",pos_y, pos_x);
            pos_x = pos_x_new;
            pos_y = pos_y_new;
            printf("\033[%d;%dfM", pos_y, pos_x);
            printf ("\033[0;0fPosition: %d, %d   \n", pos_x, pos_y);
        } else {
            pos_x_new = pos_x;
            pos_y_new = pos_y;
        }
        
        // printf( "%d, %d, %c   ", pos_x, pos_y, map->pos[pos_y][pos_x]);
        
        // printf( "\033[u");
    }
    printf( "\033[%d;%df", 36, 1);
    return;
}


int main(int argc, char* argv[]) {
    struct server_socket *server = malloc(sizeof(server));
    struct map *map = malloc(sizeof(map)) ;
    
    map_init(map, MAP_X, MAP_Y);
    char *filename = malloc(120);
    filename = argv[1];
    map_read(map, filename);
    map_print(map);
    server_start(server);
    printf("\033[0;0f%d",ntohs(server->addr.sin_port));
    // server_read(server);
    map_navigate(map, server);
    exit(0);
}