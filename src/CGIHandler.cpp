/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:01:30 by gleal             #+#    #+#             */
/*   Updated: 2022/07/29 22:09:13 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
#include <sys/mman.h>
#include <stdio.h>

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
 
void	CGIHandler::convert_to_charptr(std::vector<std::vector <char> > &vec_of_vec, std::vector<char *> &vec_of_charptr)
{
	for (std::vector<std::vector <char> >::iterator it = vec_of_vec.begin();
		it != vec_of_vec.end();
		it++)
	{
		vec_of_charptr.push_back(it->data());
	}
	vec_of_charptr.push_back(NULL);
}

void	CGIHandler::execute_cgi_script( Request & req, Response & res  )
{
	// Environment Variables

	std::vector<std::vector <char> > buf = environment_variables(req);
	std::vector<char *> env_vars;
	convert_to_charptr(buf, env_vars);
	// std::cout << "The environment variables will be:"<< std::endl;
	// std::for_each(buf.begin(), buf.end(), print_data<std::vector <char> >);
	char *const *envs = env_vars.data();
	std::cout << "The environment variables are:" << std::endl;
	for (size_t i = 0; i < env_vars.size() - 1; i++) {
		std::cout << envs[i] << std::endl;
	}

	// Filepath (1st and 2nd execve argument)

	std::vector<char *> filepath;
	std::string fp = req._path.c_str() + 1;
	std::vector<char> fp_vec(fp.begin(), fp.end());
	fp_vec.push_back('\0');
	filepath.push_back(fp_vec.data());
	filepath.push_back(NULL);
	std::cout << "CGI Argument is " << filepath[0] << std::endl;

	// Shared memory

	// shm_unlink("shared_mem");
	// int shm_fd = shm_open("shared_mem", O_CREAT | O_RDWR, 0666);
	// if (shm_fd == -1) {
	// 	perror("OPEN SUCKS BECAUSE");
	// 	throw HTTPStatus<500>();
	// }

	// if (ftruncate(shm_fd, req._raw_body.size()) == -1) {
	// 	shm_unlink("shared_mem");
	// 	close(shm_fd);
	// 	perror("TRUNCATE BECAUSE");
	// 	throw HTTPStatus<500>();
	// }
	
	// char *shared_mem = (char *)mmap(0, req._raw_body.size() + 1, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	// if (shared_mem == (void *)-1) {
	// 	shm_unlink("shared_mem");
	// 	close(shm_fd);
	// 	perror("MMAP BECAUSE");
	// 	throw HTTPStatus<500>();
	// }

	// shared_mem[0] = 'O';
	// shared_mem[1] = 'L';
	// shared_mem[2] = 'A';
	// shared_mem[3] = '\n';
	// shared_mem[4] = '\0';
	
	int fd[2];
	int p = pipe(fd);
	if (p == -1)
		throw HTTPStatus<500>();
	
	pid_t pid = 0, w_pid = 0;
	int status;
	pid=fork();
	if (pid == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		std::cout << "HERE we go" << std::endl;
		// std::string str(shared_mem);
		// write(0, shared_mem, 4);

		execve(filepath[0], filepath.data(), envs);
		exit(EXIT_FAILURE);
	}
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	std::cout << "Baby" << std::endl;
	w_pid = waitpid(pid, &status, 0);
	// munmap(shared_mem, req._raw_body.size());
	// shm_unlink("shared_mem");
	// close(shm_fd);
	if (w_pid == -1 || status == EXIT_FAILURE)
		throw HTTPStatus<500>();
	res.set_default_body(); // temporary
}

// Can we turn Req method into string instead of enum?

std::vector<std::vector <char> >	CGIHandler::environment_variables( Request & req )
{
	std::vector<std::vector <char> > buf;

	std::map<enum RequestMethod, std::string> req_method;
	req_method[GET]="GET";
	req_method[POST]="POST";
	req_method[DELETE]="DELETE";
	setenv(buf, "REQUEST_METHOD", req_method[req.request_method].c_str());
	setenv(buf, "SERVER_PROTOCOL", "HTTP/1.1");
	setenv(buf, "PATH_INFO", req._path.c_str() + 1);
	// std::stringstream ss;
	// ss << req._raw_body.size();
	// setenv(buf, "CONTENT_LENGTH", ss.str().c_str());
	setenv(buf, "CONTENT_LENGTH", "4");
	return (buf);
}

void	CGIHandler::setenv( std::vector<std::vector <char> > &buf,  const char * var, const char * value)
{
	std::string	env_var(var);
	env_var.push_back('=');
	env_var += value;
	env_var.push_back('\0');
	buf.push_back(std::vector<char>(env_var.begin(), env_var.end()));
}
