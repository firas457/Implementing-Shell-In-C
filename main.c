#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include "client.h"



#define PORT_NUMBER 55555
#define BUFFER_LEN 1024






//  this function is used in system impl
//  this function creates a child proccess and it uses the child proccess
//  (pid==0) to execute the execl function
//  execl  takes "/bin/sh" as it's first arguemnt
//  "/bin/sh" refers to the system shell which have all the system functions 
//  implementions like ls,CD...
//  execl  takes "sh" as it's second arguemnt
//  "sh" refers to the cmd that we get the commands to be executed from
//   execl  takes "-c" as it's third arguemnt which is used to compile the command
//   execl  takes line as it's fourth arguemnt which is used to store the command
//   written in the cmd
//   wait(NULL) will block parent process until any of its children has finished

int system_imple(const char *line)
{
    
    pid_t pid;

    pid = fork();

    if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", line, (char *)NULL);
        exit(1);
    }

    else
    {
        wait(NULL);
    }

    return 0;
}





int main(int argc, char **argv)
{
   

    char line[BUFFER_LEN];

    char cwd[256];

    int sock = -1;

    int std_output = dup(1);

//infinite loop unitll EXIT command recived

    while (1)
    {

        // Yes Master loop

        printf("Yes Master ?? ");

        if (!fgets(line, BUFFER_LEN, stdin))
        {
            break;
        }




        // ECHO 

        char *pos;
        pos = strstr(line, "ECHO");

        if (pos)
        {
            int i = 0;
            int j = 0;
            char arr[4];
            arr[0] = 'E';
            arr[1] = 'C';
            arr[2] = 'H';
            arr[3] = 'O';
            while (line[i] == arr[j])
            {
                i++;
                j++;
            }
            char temp[1024];
            int k = 0;
            for (j; j < BUFFER_LEN; j++)
            {
                temp[k] = line[j];
                k++;
            }
            printf("%s\n", temp);
        }



// getcwd

        //the yes master keeps appearing, if we type getcwd and we typed after it
        //some string then strcmp will be equal to 0 and enter the if statement
        //inside the if statement , getcwd function whic is a function in c
        //will return the cuurent dirictory
        //if null it reurns error
        //after finshing this steps , the loop of yes master returns


        if (strcmp(line, "getcwd\n") == 0)
        {

            if (getcwd(cwd, sizeof(cwd)) == NULL)
                perror("error");
            else
                printf("current directory is: %s\n", cwd);
        }



        //DIR

        if (strcmp(line, "DIR\n") == 0)
        {
            struct stat buf;
            int exists;
            DIR *d;

            struct dirent *de;

            d = opendir(".");

            if (d == NULL)
            {
                exit(1);
            }

            for (de = readdir(d); de != NULL; de = readdir(d))
            {
                exists = stat(de->d_name, &buf);

                if (exists < 0)
                {
                    fprintf(stderr, "%s not found\n", de->d_name);
                }

                else
                {
                    printf("%s     ", de->d_name);
                }
              }

            printf("%s\n ", "  ");
            closedir(d);
        }



    //CD

    //The chdir command is a system function (system call) which is
    //used to change the current working directory


        if (strcmp(line, "CD\n") == 0 || strcmp(line, "CD\n") == 0)
        {
            char buffer[256] = {0};

            if (chdir(strcpy(buffer, line + 3)) == -1)
            {
                perror("unvalid argument");
            }

        }





        if (strcmp(line, "TCP PORT\n") == 0)
        {

            sock = open_socket(PORT_NUMBER);
        }




//// this is system implemention using system() c libary function
/// you can try it , it works
// int system(const char *command) is a C library function
// code start

         if (strcmp(line, "EXIT\n") != 0 && strcmp(line, "TCP PORT_NUMBER\n") != 0 &&
         strcmp(line, "DIR\n") != 0 && strcmp(line, "get\n") != 0){
         printf("using system()\n");

        system(line);

         }
// code end






// system implemntion using exec , wait ,fork without system()
// explanation written inside the system imple function

        if (strcmp(line, "EXIT\n") != 0 && strcmp(line, "TCP PORT_NUMBER\n") != 0 &&
            strcmp(line, "DIR\n") != 0 && strcmp(line, "get\n") != 0)
        {
    // explanation written inside the system imple function
            printf("using exec , wait ,fork without system()\n");
            system_imple(line);
        }





// LOCAL  is used for TCP PORT command...
        
        if (strcmp(line, "LOCAL\n") == 0)
        {
            if (sock == -1)
            {
                printf("socket is not open");
            }
            else
            {
                local_socket(sock, std_output);
            }

        }



    //DIR  : to return the current dirictory FILES

        if (strcmp(line, "DIR\n"))
        {
            DIR *dir_p;
            struct dirent *dirent_var;

            dir_p = opendir("/Desktop");

            if (dir_p == NULL)
            {
                printf("\n\n");
            }
            else
            {
                for (;;)
                {
                    dirent_var = readdir(dir_p);

                    if (dirent_var == NULL)
                        break;

                    printf("%s\n", dirent_var->d_name);
                }

                closedir(dir_p);
            }
        }








//COPY ---->  FILE FROM SRC TO DST

    //fopen() is a C libary function
    //fputc() is a C libary function
    //fgetc() is a C libary function

        char *pos3;
        pos3 = strstr(line, "COPY");
        int tmp = 0;

        if (pos3)
        {
            FILE *sourceFile;
            FILE *destFile;

            char ch;
            char sourcePath[100];
            char destPath[100];
            for (int i = 4; i < strlen(line); i++)
            {
                if (isalpha(line[i]) != 0)
                {
                    tmp = i;
                    break;
                }
            }

            int x = tmp;
            int i = 0;
            while (isalpha(line[x]) != 0)
            {

                sourcePath[i] = line[x];
                i++;
                x++;
            }

            int tmp2 = 0;

            for (int j = x; j < strlen(line); j++)
            {
                if (isalpha(line[j]) != 0)
                {
                    tmp2 = j;
                    break;
                }
            }

            int k = tmp2;
            int a = 0;
            while (isalpha(line[k]) != 0)
            {

                destPath[a] = line[k];
                k++;
                a++;
            }

            sourceFile = fopen(sourcePath, "r");
            destFile = fopen(destPath, "w");

            if (sourceFile == NULL || destFile == NULL)
            {
                printf("\nUnable to open file.\n");
                exit(EXIT_FAILURE);
            }

            ch = fgetc(sourceFile);

            while (ch != EOF)
            {

                fputc(ch, destFile);

                ch = fgetc(sourceFile);
            }

            printf("\ncopied successfully.\n");

            fclose(sourceFile);
            fclose(destFile);
        }



    //DELETE FILE USING unlink


        //unlink() function is a linux system call
        //strstr() is a C libary function

        line[strlen(line) - 1] = '\0';
        char *posa;
        posa = strstr(line, " ") + 1;
        unlink(posa);


        //EXIT
        if (strcmp(line, "EXIT\n") == 0)
        {
            break;
        }
    }


}