#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAP_X 34
#define MAP_Y 65

struct positionXY {
    int x;
    int y;
};

int main(int argc, char* argv[]) {
    int MAXCHAR = 100000;
    struct stat st;

    struct positionXY start;
    char map[MAP_X][MAP_Y];
    FILE *fp;
    char* filename = argv[1];

    stat(filename, &st);
    MAXCHAR = st.st_size;
    
    char str[MAXCHAR];
    
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
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
                    start.x = j;
                    start.y = i;
                    break;
                default:
                    // printf("\nX=%d, Y=%d, C=%c", j, i, str[i]);
                    map[j][i] = str[i];
                    break;
            }
        }
    }
    printf("\nfile size %d bytes\n", MAXCHAR);
    fclose(fp);
    return 0;
}