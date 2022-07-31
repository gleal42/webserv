/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:01:30 by gleal             #+#    #+#             */
/*   Updated: 2022/07/31 20:34:06 by gleal            ###   ########.fr       */
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
	std::vector<char> cmd_vec = convert_to_char_vector("php-cgi");
	std::vector<char> fp_vec = convert_to_char_vector(req._path.c_str() + 1);
	filepath.push_back(cmd_vec.data());
	filepath.push_back(fp_vec.data());
	filepath.push_back(NULL);

	std::cout << "CGI 1 Argument is " << filepath[0] << std::endl;
	std::cout << "CGI 2 Argument is " << filepath[1] << std::endl;

	FILE *input_file = tmpfile();
	FILE *output_file = tmpfile();
	// FILE *output_file = fopen("Checking.txt", "w+");
	if (input_file == NULL || output_file == NULL)
	{
		std::cerr << "Temp file error" << std::endl;
		throw HTTPStatus<500>();
	}

	int input_fd = fileno(input_file);
	int output_fd = fileno(output_file);
	
	std::cout << "Body is " << req._raw_body.c_str() << std::endl;
	write(input_fd, req._raw_body.c_str(), req._raw_body.size());
	rewind(input_file);
	
	pid_t pid = 0, w_pid = 0;
	int status;
	pid=fork();
	if (pid == 0)
	{
		std::cout << "HERE we go" << std::endl;
		dup2(input_fd, STDIN_FILENO);
		dup2(output_fd, STDOUT_FILENO);
		execve("/usr/local/bin/php-cgi", filepath.data(), envs);
		// exit(EXIT_FAILURE);
	}
	w_pid = waitpid(pid, &status, 0);
	(void)w_pid;
	// LLDB sends EINTR signal
	// if (w_pid == -1)
	// {
	// 	std::cerr << "WPID ERROR" << std::endl;
	// 	perror("Didnt work because ");
	// 	throw HTTPStatus<500>();
	// }
	if (status == EXIT_FAILURE)
	{
		std::cerr << "Child ERROR" << std::endl;
		throw HTTPStatus<500>();
	}
	fseek(output_file, 0L, SEEK_END);
	long sz = ftell(output_file);
	rewind(output_file);
	sz = sz - ftell(output_file);

	std::cout << "file has size " << sz << std::endl;
	char *file_contents = (char *) mmap(0, sz, PROT_READ, MAP_PRIVATE, output_fd, 0);
	if (file_contents == (void *)-1) {
		throw HTTPStatus<500>();
	}
	std::cout << "File has size: " << file_contents << std::endl;
	std::string str(file_contents, sz);
	str.push_back('\0');
	set_response(str, res);
	munmap(file_contents, sz);
	fclose(input_file);
	fclose(output_file);
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
	setenv(buf, "QUERY_STRING", "test=querystring");
	setenv(buf, "REDIRECT_STATUS", "200");

	std::string full_script_path = full_path(req._path.c_str());
	setenv(buf, "PATH_INFO", full_script_path.c_str());
	setenv(buf, "SCRIPT_FILENAME", "test/cgi/test.php");

	std::stringstream ss;
	ss << req._raw_body.size();
	setenv(buf, "CONTENT_LENGTH", ss.str().c_str());

	setenv(buf, "GATEWAY_INTERFACE", "CGI/1.1");

	setenv(buf, "CONTENT_TYPE", "application/x-www-form-urlencoded");
	
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

// Delete Status

BaseStatus	CGIHandler::set_response( std::string bdy, Response &res )
{
	size_t crlf = bdy.find(D_CRLF);
	res.save_raw_headers(bdy.substr(0, crlf));
	std::string status = res.get_header_value("Status");
	if (status.empty()) {
		status = "200";
	}
	size_t start_digit = status.find_first_of("0123456789");
	if (start_digit == std::string::npos)
		throw HTTPStatus<400>();
	size_t last_digit = status.find_last_of("0123456789");
	status = status.substr(start_digit, last_digit);
	if (status.find_first_not_of("0123456789") != std::string::npos)
		throw HTTPStatus<400>();
	std::stringstream ss(status);
	int nbr_status;
	ss >> nbr_status;
	crlf = crlf+4;
	bdy = bdy.substr(crlf);
	res.set_body(bdy.data());
	std::stringstream len;
	len << bdy.size();
	res.set_header("Content-Length", len.str());
	return HTTPStatus<200>();
}

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

// int fd[2];
// int p = pipe(fd);
// if (p == -1)
// 	throw HTTPStatus<500>();