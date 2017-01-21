/*
 * masslog daemon
 *
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/common.h"

void get_log(struct shmmng* mng){
  struct syslogmes* readpos = mng->mes;

  for(;;){
    if(readpos->flag == MAGIC_END){
      printf("pos: %p, ", readpos);
      printf("logq: %p\n", mng);
      readpos = (struct syslogmes*)mng->mes;
    }
    if(readpos->flag != MAGIC_WRITE){
      continue;
    }
    printf("Message:%s\n", readpos->message);
    readpos->flag = MAGIC_READ;
    __atomic_fetch_sub(&mng->distance, readpos->size, __ATOMIC_RELAXED);
    readpos = (struct syslogmes*)((char*)readpos + readpos->size);

    if ((char*)readpos > (char*)mng + mng->size - sizeof(struct syslogmes) ||
        readpos->flag == MAGIC_END){
      printf("pos: %p, ", readpos);
      printf("logq: %p\n", mng);
      readpos = (struct syslogmes*)mng->mes;
    }
  }
}

#define PIDFILE "../masslog.pid"

/* pid file is  */
void make_pidfile(){
  FILE *fp;
  char pid[16];

  if ((fp = fopen(PIDFILE, "w")) == NULL) {
    perror("fopen pid");
    exit(EXIT_FAILURE);
  }
  sprintf(pid, "%d", getpid());
  fputs(pid, fp);
  fclose(fp);
}

#define SHMEM_SIZE (4096 * 2)

int main(){
  printf("masslogd started\n");
  make_pidfile();

  struct shmmng* logq  = shmem_alloc(SHMEM_SIZE, PIDFILE);

  /* Main loop */
  get_log(logq);
  shmem_free(logq);

  return 0;
}
