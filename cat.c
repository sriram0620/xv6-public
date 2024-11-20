#include "types.h"
#include "stat.h"
#include "user.h"
#include <stdlib.h>
#include <string.h>

char buf[512];

void cat(int fd) {
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      printf(1, "cat: write error\n");
      exit(1);  // Exit with an error status
    }
  }
  if(n < 0){
    printf(1, "cat: read error\n");
    exit(1);  // Exit with an error status
  }
}

int main(int argc, char *argv[]) {
  int fd, i;

  if(argc <= 1){
    cat(0);  // Read from standard input
    exit(0); // Exit successfully
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit(1); // Exit with an error status
    }
    cat(fd);
    close(fd);
  }
  exit(0); // Exit successfully
}
