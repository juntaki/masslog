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
#include "../lib/masslog.h"

int main(){
  printf("masslogd started\n");

  struct shmmng* logq  = shmem_get();
  shmem_free(logq);
  logq  = shmem_alloc();

  /* Main loop */
  get_log(logq);

  return 0;
}
