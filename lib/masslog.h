/*
 * masslog client library
 *
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include <syslog.h>

int add_log(int facility, int priority, const char* message, struct shmmng* shmem);
