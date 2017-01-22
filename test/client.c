/*
 * masslog test client
 *
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
*/

#include "../lib/common.h"
#include "../lib/masslog.h"
#include <stdio.h>

int main()
{
  int i;
  printf("started\n");
  struct shmmng* logq  = shmem_get();

  printf("shmem_alloc done\n");
  for(i=0; i<200; i++){
    char message[256];
    sprintf(message, "log id = %d", i);
    add_log(LOG_USER, LOG_ERR, message, logq);
    printf("add log %d\n", i);
  }
  return 0;
}
