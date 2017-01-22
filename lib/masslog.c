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

void get_log(struct shmmng* shmem){
  for(;;){
    struct syslogmes* readpos = &shmem->mes[shmem->reader_offset % shmem->size];
    if(readpos->flag != MAGIC_WRITE){
      continue;
    }
    printf("Message:%s\n", readpos->message);
    readpos->flag = MAGIC_READ;
    __atomic_fetch_add(&shmem->reader_offset, 1, __ATOMIC_RELAXED);
  }
}

int add_log(int facility, int priority, const char* message, struct shmmng* shmem){
  unsigned long long offset = __atomic_fetch_add(&shmem->first_writer_offset, 1, __ATOMIC_RELAXED);
  struct syslogmes * data = &shmem->mes[offset % shmem->size];

  /* set parameter */
  data->facility = facility;
  data->priority = priority;
  strncpy(data->message, message, MESSAGE_MAX);
  data->flag = MAGIC_WRITE;

  return 0;
}

