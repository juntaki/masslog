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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SHM_FILE "/masslog.shm"
#define SHM_SIZE 10
struct shmmng* shmem_get(){
  int alloc_size = SHM_SIZE * sizeof(struct syslogmes) + sizeof(struct shmmng);
  int fd = shm_open(SHM_FILE, O_RDWR, S_IRUSR | S_IWUSR);

  struct shmmng* shmem;
  shmem = (struct shmmng *)mmap(NULL, alloc_size, PROT_WRITE|PROT_READ, MAP_SHARED,fd, 0 );

  return shmem;
}

struct shmmng* shmem_alloc(){
  shm_unlink(SHM_FILE);
  int alloc_size = SHM_SIZE * sizeof(struct syslogmes) + sizeof(struct shmmng);
  int fd = shm_open(SHM_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  ftruncate(fd, alloc_size);

  struct shmmng* shmem;
  shmem = (struct shmmng *)mmap(NULL, alloc_size, PROT_WRITE|PROT_READ, MAP_SHARED,fd, 0 );

  shmem->reader_offset = 0;
  shmem->first_writer_offset = 0;
  shmem->last_writer_offset = 0;
  shmem->size = SHM_SIZE;

  memset(shmem->mes, 0, alloc_size - sizeof(struct shmmng));

  return shmem;
}

int shmem_free(){
  shm_unlink(SHM_FILE);
  return 0;
}
