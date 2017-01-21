/*
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

static int get_pos(void* data, int size){
  struct shmmng *mng = (struct shmmng*)DATA2SHM(data);
  int pos;

 retry:
  pos = __atomic_fetch_add(&mng->pos, size, __ATOMIC_RELAXED);

  if (pos + size > mng->size){
    if (_get_lock(data)){
      mng->pos = 0;             /* should be atomic? */
    }
    goto retry;
  }

  while(mng->lock);

  return pos;
}

int add_log(int facility, int priority, const char* message, char* logq){
  int size = strlen(message) + sizeof(struct syslogmes);
  struct shmmng* mng = (struct shmmng*)DATA2SHM(logq);
  struct syslogmes * data;
  int pos;

  pos = get_pos(logq, size);

  data = (struct syslogmes*)(logq + pos % mng->size);

  /* set parameter */
  data->facility = facility;
  data->priority = priority;
  data->size = size;
  memcpy(data->message, message, strlen(message) + 1);
  data->flag = MAGIC_WRITE;
  /* TODO set null at end */

  DEBUG_PRINT("writepos: %d (%d)\n", mng->pos, mng->pos % mng->size);

  return 0;
}

