#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir(dir) _mkdir(dir)
#endif

int create_dir(const char *path);

// TODO: test this and turn it into an init function instead of a giant mess
// inside main, also add subdirectory creation, gonna have to look up how git
// does it.
int main(int argc, char **argv) {
  if (strcmp(argv[1], "init") == 0) {
    switch (create_dir(".git")) {
    case 0: {
      printf("Successfully created .git/\n");
      break;
    }
    case 1: {
      fprintf(stderr, "Failed to create .git/\n.git/ already exists\n");
      break;
    }
    case -2: {
      fprintf(stderr, "Failed to create .git/\n.git/ already exists, but it is "
                      "not a directory\n");
      break;
    }
    default: {
      fprintf(stderr, "Failed to create .git/\n");
      break;
    }
    }
  }

  return EXIT_SUCCESS;
}

int create_dir(const char *path) {
  struct stat st;
  if (stat(path, &st) == 0) {
    if (st.st_mode & S_IFDIR) {
      return 1; // directory exists
    } else {
      return -2; // existsts, but not a directory
    }
  }

#ifdef _WIN32
  if (mkdir(path) == 0) {
    return 0; // success
  }
#else
  if (mkdir(path, 0755) == 0) {
    return 0; // success
  }
#endif

  return -1; // failure
}
