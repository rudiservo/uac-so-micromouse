#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
  int ch = 0;
  int pos_x = 14;
  int pos_y = 9;
  printf("\033[2J");
  printf ("Position: %d, %d   \n", pos_x, pos_y);
  printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
  for (size_t i = 0; i < 15; i++)
  {
    printf("║                                                                                                                                        ║\n");
  }
  printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
  printf( "\033[%d;%df", pos_y, pos_x);
  system ("/bin/stty raw");
  while ( ( ch = getchar ()) != 'q') {
    printf( "\033[%d;%df ", pos_y, pos_x);
    if (ch == 'w') {
      pos_y --;
      // printf( "\033[A");
    }
    if (ch == 'a') {
      pos_x --;
      // printf( "\033[2D");
    }
    if (ch == 's') {
      pos_y ++;
      // printf( "\033[B");
    }
    if (ch == 'd') {
      pos_x ++;
      // printf( "\033[0C");
    }
    // printf( "\033[s");
    printf( "\033[2;11f");
    printf( "%d, %d   ", pos_x, pos_y);
    printf( "\033[%d;%df", pos_y, pos_x);
    // printf( "\033[u");
  }
  printf( "\033[%d;%dfX", pos_y, pos_x);
  printf( "\033[20;0f ");
  system ("/bin/stty cooked");
  return 0;
}
