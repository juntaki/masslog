/*
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"
#include "masslog.h"

static inline struct syslogmes* get_pointer(struct shmmng* shmem, size_t offset){
  return (struct syslogmes*)((char*) shmem->mes + offset);
}

static size_t get_pos(struct shmmng* shmem, size_t size){
  size_t offset;

 retry:
  offset = __atomic_fetch_add(&shmem->pos, size, __ATOMIC_RELAXED);

  if (offset + size > shmem->size){
    if (!(offset + sizeof(struct syslogmes) > shmem->size)){
      struct syslogmes* end = get_pointer(shmem, offset);
      end->flag = MAGIC_END;
    }

    shmem->pos = 0;             /* should be atomic? */
    goto retry;
  }

  __atomic_fetch_add(&shmem->distance, size, __ATOMIC_RELAXED);

  while(shmem->distance > shmem->size){
    printf("%d, %d\n", shmem->distance, shmem->size);
    //pause();
  }; /* wait for read */

  return offset;
}

int add_log(int facility, int priority, const char* message, struct shmmng* shmem){
  size_t size = strlen(message) + sizeof(struct syslogmes);
  size_t offset = get_pos(shmem, size);
  struct syslogmes * data = get_pointer(shmem, offset);

  /* set parameter */
  data->facility = facility;
  data->priority = priority;
  data->size = size;
  memcpy(data->message, message, strlen(message) + 1);
  data->flag = MAGIC_WRITE;

  return 0;
}

