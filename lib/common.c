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
#include "common.h"

int masslog_debug = 0;

void hex_dmp(const void *buf, int size)
{
  int i,j;
  unsigned char *p = (unsigned char *)buf, tmp[20];

  printf("+0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F|  -- ASCII --\r\n");
  printf("--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+----------------\r\n");
  for (i=0; p-(unsigned char *)buf<size; i++) {
    printf("%5d  ", i*16);
    for (j=0; j<16; j++) {
      tmp[j] = (unsigned char)((*p<0x20||*p>=0x7f)? '.': *p);
      printf("%02X ", (int)*p);
      if (++p-(unsigned char *)buf>=size) {
        tmp[++j] = '\0';
        for (;j<16;j++) {
          printf("   ");
        }
        break;
      }
    }
    tmp[16] = '\0';
    printf("%s\r\n", tmp);
    if (p-(unsigned char *)buf>=size) {
      break;
    }
  }
  fflush(stdout);
}

void* shmem_alloc(int size, const char* keyfile){
  key_t   key;
  int id;
  void* shmem;
  
  if ((key = ftok(keyfile, 'R')) == -1) {
    perror("ftok");
    return NULL;
  }

  if ((id = shmget(key, size + MNGSIZE, IPC_CREAT | 0666)) < 0) {
    perror("shmget");
    return NULL;
  }

  shmem = (void *)shmat(id, (void *)0, 0);
  if (shmem == (char *)-1) {
    perror("shmat");
    return NULL;
  }

  ((struct shmmng *)shmem)->id = id;
  ((struct shmmng *)shmem)->size = size;

  return SHM2DATA(shmem);
}

int shmem_free(void* data){
  char* shmem;
  int id;

  shmem = DATA2SHM(data);
  id = ((struct shmmng*)shmem)->id;

  /* dettach the segment to data space */
  if (shmdt(shmem) == -1){
    perror("shmdt");
    return 1;
  }
  /* Destroy the segment */
  if (shmctl(id, IPC_RMID, 0) == -1){
    perror("shmctl");
    return 1;
  }
  return 0;
}

int _free_lock(void* data){
  int* lock = &((struct shmmng*)DATA2SHM(data))->lock;
  if(*lock){
    DEBUG_PRINT("[freelock]");
    return (*lock = 0);
  }
  else{
    DEBUG_PRINT("Lock already free\n");
    return 1;
  }
}

int _get_lock(void* data){
  int* lock = &((struct shmmng*)DATA2SHM(data))->lock;
  //DEBUG_PRINT("[getlock]");
  return __atomic_test_and_set(lock, __ATOMIC_RELAXED);
}
