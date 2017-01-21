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
#include "common.h"

int get_log(char* logq){
  struct shmmng *mng = (struct shmmng*)DATA2SHM(logq);
  struct syslogmes* readpos;
  char* local = malloc(mng->size);

  while(!mng->lock);
  memcpy(local, logq, mng->size);
  _free_lock(logq);

  readpos = (struct syslogmes*)local;

  do{
    printf("Message:%s\n", readpos->message);
    readpos = (struct syslogmes*)((char*)readpos + readpos->size);
    if(readpos->flag != MAGIC_WRITE)
      break;
  }while((char*)readpos < local + mng->size);

  return 0;
}

int main(){
  FILE *fp;
  char pid[16];

  /* Make pid file */
  if ((fp = fopen("/var/run/masslog.pid", "w")) == NULL) {
    perror("fopen pid");
    exit(EXIT_FAILURE);
  }
  sprintf(pid, "%d", getpid());
  fputs(pid, fp);

  fclose(fp);

  char* logq  = shmem_alloc(4096*2, "/var/run/masslog.pid");
  shmem_free(logq);

  logq  = shmem_alloc(4096*2, "/var/run/masslog.pid");

  /* Main loop */
  for(;;){
    get_log(logq);
  }

  shmem_free(logq);

  return 0;
}
