/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:01:30 by gleal             #+#    #+#             */
/*   Updated: 2022/07/28 18:23:08 by gleal            ###   ########.fr       */
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
	// std::cout << "The environment variables will be:"<< std::endl;
	// std::for_each(buf.begin(), buf.end(), print_data<std::vector <char> >);

	for (std::vector<std::vector <char> >::iterator it = buf.begin();
		it != buf.end();
		it++)
	{
		env_vars.push_back(it->data());
	}
	env_vars.push_back(NULL);
	char *const *envs = env_vars.data();

	std::vector<char *> filepath;
	std::string fp = req._path.c_str() + 1;
	std::vector<char> fp_vec(fp.begin(), fp.end());
	fp_vec.push_back('\0');
	filepath.push_back(fp_vec.data());
	filepath.push_back(NULL);

	// const char *str = req._path.c_str() + 1;
	std::cout << "CGI Argument is " << filepath[0] << std::endl;

	std::cout << "The environment variables are:" << std::endl;
	for (size_t i = 0; i < env_vars.size() - 1; i++) {
		std::cout << envs[i] << std::endl;
	}

	int status;
	pid=fork();
	if (pid == 0)
	{
		std::cout << "HERE we go" << std::endl;
		execve(filepath[0], filepath.data(), envs);
		exit(EXIT_FAILURE);
	}
	w_pid = waitpid(pid, &status, 0);
	if (w_pid == -1 || status == EXIT_FAILURE)
		throw HTTPStatus<500>();
	res.set_default_body(); // temporary
}

// Can we turn Req method into string instead of enum?

void	CGIHandler::set_environment_variables( std::vector<std::vector <char> > &buf,  Request & req )
{
	std::map<enum RequestMethod, std::string> req_method;
	req_method[GET]="GET";
	req_method[POST]="POST";
	req_method[DELETE]="DELETE";
	setenv(buf, "REQUEST_METHOD", req_method[req.request_method].c_str());
	setenv(buf, "SERVER_PROTOCOL", "HTTP/1.1");
	setenv(buf, "PATH_INFO", req._path.c_str() + 1);
}

void	CGIHandler::setenv( std::vector<std::vector <char> > &buf,  const char * var, const char * value)
{
	std::string	env_var(var);
	env_var.push_back('=');
	env_var += value;
	env_var.push_back('\0');
	buf.push_back(std::vector<char>(env_var.begin(), env_var.end()));
}
