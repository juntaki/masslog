/*
 * masslog common library
 *
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <pthread.h>
#include "common.h"

struct shmmng* shmem_get(int size, const char* keyfile){
  key_t   key;
  int id;
  struct shmmng* shmem;

  if ((key = ftok(keyfile, 'R')) == -1) {
    perror("ftok");
    return NULL;
  }

  if ((id = shmget(key, size, 0666)) < 0) {
    perror("shmget");
    return NULL;
  }

  shmem = (struct shmmng *)shmat(id, (void *)0, 0);
  if (shmem == (struct shmmng *)-1) {
    perror("shmat");
    return NULL;
  }

  return shmem;
}

struct shmmng* shmem_alloc(int size, const char* keyfile){
  key_t   key;
  int id;
  struct shmmng* shmem;

  if ((key = ftok(keyfile, 'R')) == -1) {
    perror("ftok");
    return NULL;
  }

  if ((id = shmget(key, size, IPC_CREAT|0666)) < 0) {
    perror("shmget");
    return NULL;
  }

  shmem = (struct shmmng *)shmat(id, (void *)0, 0);
  if (shmem == (struct shmmng *)-1) {
    perror("shmat");
    return NULL;
  }

  shmem->id = id;
  shmem->size = size;
  shmem->distance = 0;
  shmem->pos = 0;

  memset(shmem->mes, 0, size - sizeof(struct shmmng));

  return shmem;
}

int shmem_free(struct shmmng* shmem){
  /* dettach the segment to data space */
  if (shmdt(shmem) == -1){
    perror("shmdt");
    return 1;
  }
  /* Destroy the segment */
  if (shmctl(shmem->id, IPC_RMID, 0) == -1){
    perror("shmctl");
    return 1;
  }
  return 0;
}
