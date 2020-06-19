#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_DEFAULT "./test.txt"
/*
*/
#define COLUMNS 15
#define ROWS 6

int main(int argc, char* argv[]) {

   FILE *fp;
   char buff[COLUMNS];
   char c;
   char ficheiro[200];
   //Selecting the maze using the commandline
   if (argc > 1){
     strcpy(ficheiro,argv[1]);
   }
   else
   {
     strcpy(ficheiro, FILE_DEFAULT);
   }
   if ( (fp = fopen(ficheiro,"r")) == NULL){
          printf("Error! opening file");

          // Program exits if the file pointer returns NULL.
          exit(1);
   }
   // reads text until newline is encountered
   //Read the first row
   fgets(buff,COLUMNS, (FILE*)fp);
   for (int i=1;i<ROWS;i++){ //(!feof(fp))
     printf("%s", buff);
     fgets(buff,COLUMNS, (FILE*)fp);


   }
   fclose(fp);

   return(1);
}
