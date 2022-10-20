#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FPS 30.0 // Frames per second
#define WIDTH 12 // Width of the video
#define HEIGHT 4 // Height of the video

int main(int argc, char *argv[]) {
    // Open file
    FILE *out = fopen("out.xba", "w");

    // Write the file header to the buffer, then write the buffer to the file
    char header[BUFSIZ];
    snprintf(header, BUFSIZ, "dim=%d %d\nfps=%.2f\n\n", WIDTH, HEIGHT, FPS);

    for (int i = 0; header[i]; i++) {
        fwrite(header + i, 1, 1, out);
    }

    // Test bytes
    char lmao[] = {0xff};

    for (int j = 0; j < 256; j++) {
        lmao[0] = (char) j;
        fwrite(lmao, 1, 1, out);
    }

    fclose(out);

    printf("Test write complete\n");

    return 0;
}