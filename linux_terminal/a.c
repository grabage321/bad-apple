#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define FPS 30.0 // Frames per second
#define SPF (1.0 / FPS) // Seconds per frame
#define FRAMES 6573 // Total number of frames
#define USPS 1000000 // Microseconds per second
#define NSPS 1000000000 // Nanoseconds per second
#define PGMSEEK 14 // Bytes to skip forward when reading the PGM files
#define WIDTH 104 // Width of the video
#define HEIGHT 40 // Height of the video
#define AREA WIDTH * HEIGHT // Total number of pictures

// Takes a timespec struct and returns the total number of seconds as a double
double ns_double(struct timespec t) {
    return (double) t.tv_sec + ((double) t.tv_nsec / NSPS);
}

int main(int argc, char *argv[]) {
    struct timespec t;
    clock_gettime(0, &t);
    double start = ns_double(t);

    unsigned char buf[BUFSIZ];
    char filename[BUFSIZ];

    system("clear");

    int pid;
    char *bad_apple[] = {"aplay", "./bad_apple.wav", NULL};
    if (pid = fork() == 0) {
        // Close stderr and write it to /dev/null instead
        close(2);
        openat(2, "/dev/null", O_WRONLY);

        // Use aplay to play the Bad Apple audio
        execvp("aplay", bad_apple);
        return 0;
    }

    for (int i = 1; i <= FRAMES; i++) {
        clock_gettime(0, &t);
        double slepe = (start + ((i + 1) / 30) + (SPF * ((i + 1) % 30))) - (ns_double(t));
        usleep((int) (slepe * USPS));

        // Open pgm file
        snprintf(filename, BUFSIZ, "b/out%d.pgm", i);
        FILE *f = fopen(filename, "r");

        // Skip past pgm header information
        fseek(f, PGMSEEK, SEEK_SET);
        fread(buf, AREA, AREA, f);

        // Move cursor to start of terminal
        printf("\033[H");

        for (int j = 0; j < AREA; j++) {
            // Print character depending on pixel value
            printf(
                (buf[j] >= 0xCC) ? "█" :
                (buf[j] >= 0x99) ? "▓" :
                (buf[j] >= 0x66) ? "▒" :
                (buf[j] >= 0x33) ? "░" :
                " "
            );

            // Print a newline every WIDTH characters
            if (j % WIDTH == WIDTH - 1)
                printf("\n");
        }

        // Newline to keep the cursor one line down
        printf("\n");
        
        // Quiet mode (-q) hides this
        if (!(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'q' && !argv[1][2])) {
            // Timing information
            clock_gettime(0, &t);
            printf("frame:  %d\n", i);
            printf("start:  %lf\n", start);
            printf("now:    %lf\n", ns_double(t));
            printf("target: %lf\n", (start + ((i + 1) / 30) + (SPF * ((i + 1) % 30))));
            printf("delta:  %lf\n", slepe * USPS);
        }

        // Close the file
        fclose(f);
    }

    return 0;
}