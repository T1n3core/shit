#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir(dir) _mkdir(dir)
#endif

int create_dir(const char *path);

int main(int argc, char **argv) {
  if (argv[1].strcmp("init") == 0) {
    switch (create_dir(".git")) {
      case 0: {
        printf("Successfully created .git/\n");
        break;
      }
      case 1: {
        printf("Failed to create .git/\n.git/ already exists\n");
        break;
      }
      case -2: {
        printf("Failed to create .git/\n.git/ already exists, but it is not a directory\n");
        break;
      }
      default: {
        printf("Failed to create .git/\n");
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
      return 0; //success
    }
  #else
    if (mkdir(path, 0755) == 0) {
      return 0; //success
    }
  #endif

  return -1; //failure
}
