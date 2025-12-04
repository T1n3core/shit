#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * create_blob(FILE *file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file_size < 0) {
        fprintf(stderr, "ERROR: could not determine file size!\n");
        return NULL;
    }

    char *buffer = malloc(file_size);
    if (!buffer) {
        fprintf(stderr, "ERROR: could not allocate memory to buffer!\n");
        return NULL;
    }

    if (fread(buffer, 1, file_size, file)) {
        free(buffer);
        return NULL;
    }

    char header[64];
    int header_len = snprintf(header, sizeof(header), "blob %zu", file_size) + 1;

    char *blob = malloc(header_len + file_size);
    memcpy(blob, header, header_len);
    memcpy(blob + header_len, buffer, file_size);
    
    free(buffer);
    return blob;
}
