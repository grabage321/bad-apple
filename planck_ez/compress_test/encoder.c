#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FPS 30.0 // Frames per second
#define WIDTH 12 // Width of the video
#define HEIGHT 4 // Height of the video
#define FRAMES 6573 // Total number of frames
#define PGMSEEK 12 // Bytes to skip forward when reading the PGM files

int main(int argc, char *argv[]) {
    char buf[BUFSIZ];
    char filename[BUFSIZ];

    // Open file
    FILE *out = fopen("out.xba", "w");

    // Write the file header to the buffer, then write the buffer to the file
    char header[BUFSIZ];
    snprintf(header, BUFSIZ, "dim=%d %d\nfps=%.2f\n\n", WIDTH, HEIGHT, FPS);

    for (int i = 0; header[i]; i++) {
        fwrite(header + i, 1, 1, out);
    }

    printf("Completed header write\n");

    for (int j = 1; j <= FRAMES; j++) {
        snprintf(filename, BUFSIZ, "b/out%d.pgm", j);
        FILE *f = fopen(filename, "r");
        fseek(f, PGMSEEK, SEEK_SET);
        fread(buf, 1, WIDTH * HEIGHT, f);


        fwrite(buf, 1, WIDTH * HEIGHT, out);

        fclose(f);
    }

    fclose(out);

    printf("Test write complete\n");

    return 0;
}