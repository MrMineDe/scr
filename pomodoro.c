#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TIME_DEF 60*25
//TODO dont hardcode home dir

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("wo input\n");
        return EXIT_FAILURE;
    }
    if(argc == 2 && !strcmp("--start", argv[1])){
        FILE *f = fopen("/home/mrmine/.config/pomodoro/time", "r");
        int len = TIME_DEF;
        if(f != NULL){
            fscanf(f, "%d", &len);
            fclose(f);
        }
        f = fopen("/tmp/pomodoro.tmp", "w");
        time_t t = time(0);
        fprintf(f, "%ld", t+len);
        fclose(f);
        //run notify-send at the end
        char str[500];
        sprintf(str,"nohup sh -c 'sleep %d; notify-send \"Pomodoro is over! Rest a bit!\"' &", len);
        system(str);
    } else if (argc == 2 && !strcmp("--status", argv[1])){
        FILE *f = fopen("/home/mrmine/.config/pomodoro/time", "r");
        FILE *p = fopen("/tmp/pomodoro.tmp", "r");
        int len = TIME_DEF;
        if(f != NULL){
            fscanf(f, "%d", &len);
            fclose(f);
        }
        if(p == NULL){
            printf("%02d:%02d\n", len/60, len%60);
        } else {
            time_t end;
            fscanf(p, "%ld", &end);
            time_t t = time(0);
            t = end - t;
            if(t < 0 && t > -(5*60))
                printf("PAUSE\n");
            else if (t < -(5*60))
                printf("%02d:%02d\n", len/60, len%60);
            else {
                struct tm *tm = localtime(&t);
                printf("%02d:%02d\n", tm->tm_min, tm->tm_sec);
            }
        }
    } else if (argc == 3 && !strcmp("--new-time", argv[1])){
        FILE *f = fopen("/home/mrmine/.config/pomodoro/time", "w");
        if(f == NULL){
            printf("couldnt open ~/.config/pomodoro/time, aborting\n");
            return EXIT_FAILURE;
        }
        int n = strtol(argv[2], NULL, 10);
        fprintf(f, "%d", n);
    }
    return 0;
}
