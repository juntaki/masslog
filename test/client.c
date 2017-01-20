/*
 * masslog test client
 *
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
*/

#include "common.h"

int main()
{
  int i;
  char* logq  = shmem_alloc_key(4096*2, "shm0.dat");
  for(i=0; i<200000; i++){
    add_log(LOG_USER, LOG_ERR, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", logq);
  }
  return 0;
}
