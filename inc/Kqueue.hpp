#ifndef __KQUEUE_H__
# define __KQUEUE_H__

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

void add_event(int kq, int socket_fd, short filter);
void enable_event(int kq, int socket_fd, short filter);
void disable_event(int kq, int socket_fd, short filter);
void delete_event(int kq, int socket_fd, short filter);

#endif
