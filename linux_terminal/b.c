#include <stdio.h>
#include <unistd.h>

int main(void) {
    int pid;
    char *bad_apple[] = {"echo", "weewoo", NULL};
    if (pid = fork() == 0) {
        printf("This is the forken strign\n");
        execvp("echo", bad_apple);
        return 0;
    }

    printf("This is strign\n");

    return 0;
}