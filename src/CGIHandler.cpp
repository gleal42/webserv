/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:01:30 by gleal             #+#    #+#             */
/*   Updated: 2022/07/27 19:48:15 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

/* Constructors */
CGIHandler::CGIHandler( void ) { /* no-op */ }
CGIHandler::CGIHandler( CGIHandler const & src ) { *this = src; }

/* Destructor */
CGIHandler::~CGIHandler( void ) { /* no-op */ }

/* Assignment operator */
CGIHandler &	CGIHandler::operator = ( CGIHandler const & rhs )
{
	if (this != &rhs) {
		//value = rhs.value;
	}
	return *this;
}

void	CGIHandler::do_GET( Request & req, Response & res )
{
	execute_cgi_script( req, res );
}

void	CGIHandler::do_POST( Request & req, Response & res )
{
	execute_cgi_script( req, res );
}

void	CGIHandler::do_DELETE( Request & req, Response & res )
{
	execute_cgi_script( req, res );
}

void	CGIHandler::execute_cgi_script( Request & req, Response & res  )
{
	pid_t pid, w_pid;
	// int p[2];
	// if (pipe(p) < 0)
	// {
	// 	throw HTTPStatus<500>();
	// }
	std::string	req_met("REQUEST_METHOD=GET");
	std::vector<char> buf1(req_met.begin(), req_met.end());
	std::vector<char *> buf;
	buf.push_back(buf1.data());
	buf.push_back(NULL);

	int status;
	const char *str = req._path.c_str() + 1;
	std::cout << "CGI Argument is " << str << std::endl;

	char *const *envs = buf.data();

	std::cout << "Envs are " << envs << std::endl;
	pid=fork();
	if (pid == 0)
	{
		execve(str, NULL, envs);
		exit(EXIT_FAILURE);
	}
	w_pid = waitpid(pid, &status, 0);
	if (w_pid == -1 || status == EXIT_FAILURE)
		throw HTTPStatus<500>();
	res.set_default_body(); // temporary
}

// Can we turn Req method into char*?

// void	CGIHandler::set_environment_variables( std::vector<char *> &buf,  Request & req )
// {
// 	std::vector<char> word(req_met, req_met + sizeof(req_met) / sizeof(char));
// 	buf.push_back(word.data());

// 	std::cout << "The environment variables will be:"<< std::endl;
// 	for_each (buf.begin(), buf.end(), &print<char *>);

// 	(void)req;
// }

	// if (req.request_method == GET) {
	// 	req_method = "GET";
	// } else if (req.request_method == POST) {
	// 	req_method = "POST";
	// } else if (req.request_method == DELETE) {
	// 	req_method = "DELETE";
	// }