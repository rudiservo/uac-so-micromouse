#include <stdio.h>
#include <unistd.h>
#include <termios.h>

struct termios oldtc;
struct termios newtc;

void cursor(int m){
  tcgetattr(STDIN_FILENO, &oldtc);
  newtc = oldtc;
  newtc.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
  switch(m){
    case 1://"up"
      printf("\033[%dC",1);
      break;
    case 2://"right"
      printf("\033[%dA",1);
      break;
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
  fflush(stdout);
}


void clearscreen(){
  printf("\033[2J");
}


int main(int argc, char **argv) {
   int ch;
   clearscreen();
   cursor(2);
   sleep(1);
   cursor(2);
   sleep(1);
    cursor(1);
    sleep(1);
    cursor(1);
    sleep(1);
    cursor(2);
    sleep(1);
    cursor(2);
    sleep(1);
     cursor(1);
     sleep(1);
     cursor(1);
     sleep(1);
     cursor(2);
     sleep(1);
     cursor(2);
     sleep(1);
      cursor(1);
      sleep(1);
      cursor(1);
      sleep(1);

}
