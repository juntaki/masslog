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
#include <assert.h>
#include <sys/mman.h>

static inline struct syslogmes* get_pointer(struct shmmng* shmem, size_t offset){
  return (struct syslogmes*)((char*) shmem->mes + offset);
}

void get_log(struct shmmng* shmem){
  int current_flag, new_flag;
  for(;;){
    while(shmem->reader_offset >= shmem->last_writer_offset ||
          shmem->reader_offset >= shmem->first_writer_offset); // confirm reader < writer

    printf("get_log: %lld", shmem->reader_offset % shmem->size);
    struct syslogmes* readpos = &shmem->mes[shmem->reader_offset % shmem->size];
    // assert(readpos->flag == STATUS_OK);
    if(readpos->flag != STATUS_OK){ // slow writer
      new_flag = STATUS_INVALID;
      __atomic_exchange(&readpos->flag, &new_flag, &current_flag, __ATOMIC_ACQ_REL);
      if(current_flag != STATUS_OK){ // still slow
        printf("flag = %d\n", current_flag);
        __atomic_fetch_add(&shmem->reader_offset, 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_add(&shmem->last_writer_offset, 1, __ATOMIC_ACQ_REL);
        continue;
      }
    }

    printf("Message:%s\n", readpos->message);
    new_flag = STATUS_INIT;
    __atomic_exchange(&readpos->flag, &new_flag, &current_flag, __ATOMIC_ACQ_REL);
    assert(current_flag == STATUS_OK ||
           current_flag == STATUS_INVALID); // if second check pass.
    __atomic_fetch_add(&shmem->reader_offset, 1, __ATOMIC_ACQ_REL);
  }
}

int add_log(int facility, int priority, const char* message, struct shmmng* shmem){
  unsigned long long offset = __atomic_fetch_add(&shmem->first_writer_offset, 1, __ATOMIC_ACQ_REL);

    printf("add_log: %lld", offset % shmem->size);
  struct syslogmes * data = &shmem->mes[offset % shmem->size];

  while(offset - shmem->reader_offset >= shmem->size); // confirm readers distance <= size

  int current_flag, new_flag;
  new_flag = STATUS_WRITING;
  __atomic_exchange(&data->flag, &new_flag, &current_flag, __ATOMIC_ACQ_REL);
  
  /* set parameter */
  data->facility = facility;
  data->priority = priority;
  strcpy(data->message, message);

  new_flag = STATUS_OK;
  __atomic_exchange(&data->flag, &new_flag, &current_flag, __ATOMIC_ACQ_REL);
  msync(data, sizeof(data), MS_SYNC);
  if(current_flag == STATUS_WRITING)
    __atomic_fetch_add(&shmem->last_writer_offset, 1, __ATOMIC_ACQ_REL);
  msync(data, sizeof(data), MS_SYNC);

  printf("add_log_done: %lld", offset % shmem->size);

  return 0;
}
