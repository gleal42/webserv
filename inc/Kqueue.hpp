#ifndef __KQUEUE_H__
# define __KQUEUE_H__

#include <stdexcept>
#include <sys/event.h>

class Kqueue 
{
	struct CreateError : public std::runtime_error {
		CreateError( void );
	};
	public:
		Kqueue();
		int		getFd() const;
		void	updateEvent(int ident, short filter, u_short flags, u_int fflags, int data, void *udata);
	private:
		int	_fd;
};

#endif
