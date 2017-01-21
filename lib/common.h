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

struct syslogmes{
  int facility;
  int priority;
  int size;
  unsigned int flag;
  char message[1];
};

struct shmmng{
  int id;
  size_t pos;
  size_t size;
  size_t distance;
  struct syslogmes mes[1];
};


struct shmmng* shmem_alloc(int size, const char* keyfile);
struct shmmng* shmem_get(int size, const char* keyfile);
int shmem_free(struct shmmng* shmem);
