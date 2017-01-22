/*
 * masslog common library
 *
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include <stdlib.h>
#include <pthread.h>

#define STATUS_INIT      0
#define STATUS_WRITING   (1 << 0)
#define STATUS_READING   (1 << 1)
#define STATUS_OK        (1 << 2)
#define STATUS_INVALID   (1 << 3)

#define MESSAGE_MAX 1024
struct syslogmes{
  int facility;
  int priority;
  int size;
  unsigned int flag;
  char message[MESSAGE_MAX];
};

struct shmmng{
  int id;
  size_t size;
  unsigned long long reader_offset;
  unsigned long long first_writer_offset;
  unsigned long long last_writer_offset;
  struct syslogmes mes[1];
};

struct shmmng* shmem_alloc();
struct shmmng* shmem_get();
int shmem_free();
