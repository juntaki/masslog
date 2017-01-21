/*
 * masslog client library
 *
 * Copyright (c) 2014 juntaki <me@juntaki.com>
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include <syslog.h>

void masslog(int priority, const char *message, ...);
void open_masslog();
void close_masslog();
