# select/poll/epoll/kqueue

> Credit: https://geeki.wordpress.com/2016/01/29/selectpollepollkqueue/

This article gives a very nice summary of what are the pros and cons of different I/O multiplexing mechanisms provided by POSIX operating systems (like Linux/FreeBSD)

## `select`

We will assume here, select is level 0 to start with (and unfortunately, it is still being used by tons of legacy code). The basic problems of select are:

* A fixed size bitmap with which you can specify the fds
* Stateless – every time one has to set all the interested fds, as once select returns, only active (result) fds will be set
* Scan through all the fds to see which are set (both by the process when select returns, as well as by the kernel when select is called) – doing unnecessary work

## `poll`

which poll tries to solve to some extent are:

* A fixed size bitmap is replaced by a variable array
* Two separate fields are provided to track interest and the result – i.e. interest is given through one field and result is returned in another field – the only advantage here is that we don’t need to re-initialize all the interest set every time we call poll
* Kernel does not need to scan through all the fds as only the interested fds are passed on to it (no bitmap, just an array of interested fds). However, when the select returns, the process will still have to scan through all the fds to find which is active

However

* poll is still stateless – interest fds are to be copied everytime – from user space to kernel space and vice versa

## `epoll`

Then came along epoll:

* It made a transition to stateful event tracking – programs register an fd once and it will be tracked until program wishes to remove it.
* So, everytime there is no need to send a list of interested events to the kernel and neither the kernel needs to scan the list of interested events everytime, it is already maintained.
* A separate function blocks waiting for any event to happen and when it returns, the process scans only through the result events, but not through all events (like poll does)

## `kqueue`

Then came along kqueue:

* kqueue is even more generic framework where not just sockets, but timers, signals, regular files, processes etc. can be kept and tracked for events.
* The framework is such that it is easily extensible to any future events that may get implemented.
* In other words, kqueue is one stop call for tracking all kinds of events. Just that it is not available in Linux yet, only in FreeBSD. 🙂
