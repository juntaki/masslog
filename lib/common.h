/*
 * masslog common library
 *
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#define MNGSIZE sizeof(struct shmmng)
#define DATA2SHM(data) ((char*)data - MNGSIZE)
#define SHM2DATA(shmem) ((char*)shmem + MNGSIZE)

#define DEBUG_PRINT(...) do{                    \
                                if(masslog_debug)               \
                                  printf("DEBUG: " __VA_ARGS__);       \
                                }while(0)

#define MAGIC_READ     0xdfdfdfdf
#define MAGIC_WRITE    0xedededed

extern int masslog_debug;

struct shmmng{
  int id;
  int lock;                     /* TODO use mutex */
  int pos;
  int size;
};

struct syslogmes{
  int facility;
  int priority;
  int size;
  unsigned int flag;
  char message[1];
};

void* shmem_alloc(int size, const char* keyfile);
int shmem_free(void* data);

/* DEBUG */
void hex_dmp(const void *buf, int size);
int _free_lock(void* data);     /* TODO use mutex */
int _get_lock(void* data);      /* TODO use mutex */
