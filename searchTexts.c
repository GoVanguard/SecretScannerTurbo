#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pcre.h>

#define configFile "regexes.conf"
#define BUF_SIZE 1024

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s ./searchTexts /my/path \n", argv[0]);
    return 1;
  }

  FILE *configFileOb = fopen(configFile, "r");
  if (configFileOb == NULL) {
    fprintf(stderr, "%sMissing or invalid regexes.conf.%s\n    -> File should contain a new line seperated list of PCRE regular expressions.\n    -> Expressions have a limit of 256 characters maximum.\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    return 1;
  }

  char regexStr[256];

  while (fgets(regexStr, sizeof(regexStr), configFileOb) != NULL) {
    const char *error;
    int errorOffset;

    pcre *regex = pcre_compile(regexStr, PCRE_MULTILINE|PCRE_EXTENDED, &error, &errorOffset, NULL);

    if (regex == NULL) {
      fprintf(stderr, "%sGoing to need you to RTFM on PCRE. Can't compile expression:%s %s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, regexStr);
      continue;
    }

    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
      fprintf(stderr, "%sGet real. %s%s%s doesn't exist or isn't a directory.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, (argv[1]), ANSI_COLOR_RED, ANSI_COLOR_RESET);
      return 1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_type != DT_REG) {
        continue;
      }

      char relativeFilePath[PATH_MAX];
      snprintf(relativeFilePath, sizeof(relativeFilePath), "%s/%s", argv[1], entry->d_name);

      FILE *file = fopen(relativeFilePath, "r");

      if (file == NULL) {
        fprintf(stderr, "%sError opening or reading file:%s %s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, relativeFilePath);
        continue;
      }

      char buf[BUF_SIZE];
      size_t n;
      char *block = NULL;
      size_t blockSize = 0;

      while ((n = fread(buf, 1, sizeof(buf), file)) > 0) {
        block = realloc(block, blockSize + n + 1);
        if (block == NULL) {
          fprintf(stderr, "%sCannot allocate enough memory. %s %s %smight to too big.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, relativeFilePath, ANSI_COLOR_RED, ANSI_COLOR_RESET);
          break;
        }

        memcpy(block + blockSize, buf, n);
        blockSize += n;
        block[blockSize] = '\0';
      }

      char *line = strtok(block, "\n");
      int line_number = 1;

      while (line != NULL) {
        int ovector[30];
        int match = pcre_exec(regex, 0, line, strlen(line), 0, 0, ovector, 30);

        if (match >= 0) {
          printf("%s%s%s:%s%d%s:%s%d%s:%s ", ANSI_COLOR_BLUE, relativeFilePath, ANSI_COLOR_CYAN, ANSI_COLOR_GREEN, line_number, ANSI_COLOR_CYAN, ANSI_COLOR_GREEN, ovector[0], ANSI_COLOR_CYAN, ANSI_COLOR_RESET);

          int start = ovector[0];
          int end = ovector[1];

          for (int i = 0; i < strlen(line); i++) {
            if (i == start) {
              printf(ANSI_COLOR_RED);
            }
            if (i == end) {
              printf(ANSI_COLOR_RESET);
            }
            printf("%c", line[i]);
          }
          printf("\n");
        }

        line = strtok(NULL, "\n");
        line_number++;
      }

      free(block);
      fclose(file);
    }

    closedir(dir);
    pcre_free(regex);
  }

  fclose(configFileOb);

  return 0;
}
