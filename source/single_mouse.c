#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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
                    printf("\nX=%d, Y=%d, C=%c", j, i, map->pos[j][i]);
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

void map_navigate(struct map *map) {
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
    int ch = 0;
    int pos_x = map->start.x;
    int pos_y = map->start.y + 1;
    int pos_x_new = pos_x;
    int pos_y_new = pos_y;
    char pos_type;
    printf( "\033[%d;%df", pos_y, pos_x);
    system ("/bin/stty raw");
    while ( ( ch = getchar ()) != 'q') {
        printf( "\033[%d;%df ", pos_y, pos_x);
        if (ch == 'w') {
        pos_y_new --;
        // printf( "\033[A");
        }
        if (ch == 'a') {
        pos_x_new --;
        // printf( "\033[2D");
        }
        if (ch == 's') {
        pos_y_new ++;
        // printf( "\033[B");
        }
        if (ch == 'd') {
        pos_x_new ++;
        // printf( "\033[0C");
        }
        // printf( "\033[s");
        pos_type= map->pos[pos_y_new - 1][pos_x_new];
        if (pos_type == ' ' || pos_type=='S') {
            pos_x = pos_x_new;
            pos_y = pos_y_new;
        } else {
            pos_x_new = pos_x;
            pos_y_new = pos_y;
        }
        printf( "\033[1;11f");
        printf( "%d, %d, %c   ", pos_x, pos_y, map->pos[pos_y][pos_x]);
        printf( "\033[%d;%df", pos_y, pos_x);
        // printf( "\033[u");
    }
    printf( "\033[%d;%dfX", pos_y, pos_x);
    printf( "\033[%d;%df", 36, 1);
    system ("/bin/stty cooked");
    return;
}

int main() {
    struct map *map = malloc(sizeof(map)) ;
    map_init(map, MAP_X, MAP_Y);
    char *filename = malloc(120);
    filename = "/home/rudi/Projects/UAc/uac-so-micromouse/maps/88.txt";
    map_read(map, filename);
    map_print(map);
    map_navigate(map);
    return 0;
}