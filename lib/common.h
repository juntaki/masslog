/*
 * masslog common library
 *
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include <stdlib.h>
#include <pthread.h>

#define MAGIC_READ     0xdfdfdfdf
#define MAGIC_WRITE    0xedededed
#define MAGIC_END      0xabababab

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
