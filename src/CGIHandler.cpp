/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:01:30 by gleal             #+#    #+#             */
/*   Updated: 2022/07/28 15:55:47 by gleal            ###   ########.fr       */
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
	std::vector<std::vector <char> > buf;
	set_environment_variables(buf, req);
	std::vector<char *> env_vars;

	for_each(buf.begin(), buf.end(), copy_convert_vector(env_vars));

	char *const *envs = env_vars.data();

	std::cout << "Envs are " << envs[0] << std::endl;

	int status;
	const char *str = req._path.c_str() + 1;
	std::cout << "CGI Argument is " << str << std::endl;
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

void	CGIHandler::set_environment_variables( std::vector<std::vector <char> > &buf,  Request & req )
{
	setenv(buf, "REQUEST_METHOD", "GET");
	
	// std::string	env_var("REQUEST_METHOD=GET", strlen("REQUEST_METHOD=GET") + 1);
	// std::vector<char> word(env_var.begin(), env_var.end());
	// buf.push_back(word.data());


	// for_each (buf.begin(), buf.end(), &print<char *>);
	// buf.push_back(NULL);

	(void)req;
}

void	CGIHandler::setenv( std::vector<std::vector <char> > &buf,  const char * var, const char * value)
{
	std::string	env_var(var);
	env_var.push_back('=');
	env_var += value;
	env_var.push_back('\0');
	buf.push_back(std::vector<char>(env_var.begin(), env_var.end()));

	// std::vector<char> word(env_var.begin(), env_var.end());
	// buf.push_back(word.data());
	std::cout << "The environment variables will be:"<< std::endl;
	std::cout << buf[0].data() << std::endl;
}

// if (req.request_method == GET) {
// 	req_method = "GET";
// } else if (req.request_method == POST) {
// 	req_method = "POST";
// } else if (req.request_method == DELETE) {
// 	req_method = "DELETE";
// }