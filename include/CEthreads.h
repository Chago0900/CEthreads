#ifndef CETHREADS_H
#define CETHREADS_H

#include <unistd.h>

pid_t CEthread_create(int (*func)(void *), void *arg);
void CEthread_join(pid_t thread_pid);

#endif // CETHREADS_H
