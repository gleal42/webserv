#ifndef __KQUEUE_H__
# define __KQUEUE_H__

#include <stdexcept>
#include <sys/event.h>

class Kqueue 
{
	public:
		struct CreateError : public std::runtime_error {
			CreateError( void );
		};
		Kqueue();
		// Getters
		int		fd() const;
		void	update_event(int ident, short filter, u_short flags);
	private:
		int	_fd;
};

#endif
