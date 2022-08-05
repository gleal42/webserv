/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:01:30 by gleal             #+#    #+#             */
/*   Updated: 2022/08/05 01:44:04 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
#include <sys/mman.h>
#include <stdio.h>

/*
	List of implemented cgi extensions

	This logic will be eliminated once Parser is implemented.
	Parser will define which executable to use

	PHP CGI must be installed and php path must be correct
	Have to check on how to make this work in other computers
*/

CGIExtInterpreter CGIHandler::extension_interpreter = create_extension_pairs();

CGIExtInterpreter CGIHandler::create_extension_pairs( void )
{
	CGIExtInterpreter	temp;
	temp[".cgi"] = full_path("/test/cgi/cpp/cgi_tester");
	temp[".php"] = "/usr/local/bin/php-cgi";
	return (temp);
}

bool CGIHandler::extension_is_implemented( const std::string &extension )
{
	return(extension_interpreter.count(extension));
}

/* Constructors */
CGIHandler::CGIHandler( void )
: status_code(200)
{ /* no-op */ }

CGIHandler::CGIHandler( const std::string &uri )
: path(remove_query_string(uri)), query_string(get_query_string(uri)), extension(get_extension(path)), interpreter(extension_interpreter[extension])
{
}

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

/*
	No distinction done by server.
	Scripts used by CGI define the different behaviours
*/

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
	// Environment Variables

	std::vector<std::vector <char> > buf = environment_variables(req);
	std::vector<char *> env_vars;
	convert_to_charptr(buf, env_vars);
	char *const *envs = env_vars.data();
	std::cout << "The environment variables are:" << std::endl;
	for (size_t i = 0; i < env_vars.size() - 1; i++) {
		std::cout << envs[i] << std::endl;
	}

	// Filepath (1st and 2nd execve argument)

	std::string file_name = filename(interpreter);
	std::vector<char *> filepath;
	std::vector<char> cmd_vec = convert_to_char_vector(file_name.c_str());
	std::vector<char> fp_vec = convert_to_char_vector(path.c_str() + 1);
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
		execve(interpreter.c_str(), filepath.data(), envs);
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
	
	long sz = file::size(output_file);
	std::cout << "file has size " << sz << std::endl;
	char *file_contents = (char *) mmap(0, sz, PROT_READ, MAP_PRIVATE, output_fd, 0);
	if (file_contents == (void *)-1) {
		throw HTTPStatus<500>();
	}
	std::string str(file_contents, sz);
	str.push_back('\0');
	munmap(file_contents, sz);
	set_response(str, res);
	fclose(input_file);
	fclose(output_file);
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

/*
	I set all environment variables so that CGI can have access to them.
	These will be passed to cgi throw 3rd parameter of execve

	Can we consider turning Req method into string instead of enum
	so that std::map is not necessary for first setenv.
*/

std::vector<std::vector <char> >	CGIHandler::environment_variables( Request & req )
{
	std::vector<std::vector <char> > buf;

	std::map<enum RequestMethod, std::string> req_method;
	req_method[GET]="GET";
	req_method[POST]="POST";
	req_method[DELETE]="DELETE";
	setenv(buf, "REQUEST_METHOD", req_method[req.request_method].c_str());

	setenv(buf, "SERVER_PROTOCOL", "HTTP/1.1");
	setenv(buf, "QUERY_STRING", query_string.c_str());
	setenv(buf, "REDIRECT_STATUS", "200");
	std::string full_script_path = full_path(path.c_str());
	setenv(buf, "PATH_INFO", full_script_path.c_str());
	setenv(buf, "SCRIPT_FILENAME", (path.c_str() + 1));
	std::stringstream ss;
	ss << req._raw_body.size();
	setenv(buf, "CONTENT_LENGTH", ss.str().c_str());
	setenv(buf, "GATEWAY_INTERFACE", "CGI/1.1");
	std::string content_type = req._headers["Content-Type"];
	std::cout << "Content-type is " << content_type << std::endl;
	setenv(buf, "CONTENT_TYPE", content_type.c_str());
	
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

/*
	CGI can send a Header "Status" (optional) that we then must convert to 
	the first line of our Response.
	First I save all headers
	I check if one is Status and do some validations
	then save the number inside the CGI handler so that
	script_status() is later called when building the message
	Then I set the body in the Response
*/

void	CGIHandler::set_response( std::string bdy, Response &res )
{
	// Headers Section
	size_t crlf = bdy.find(D_CRLF);
	res.save_raw_headers(bdy.substr(0, crlf));
	std::string status = res.get_header_value("Status");
	if (status.empty())
		status = "200";
	else
		res.delete_header("Status");

	// Status Section
	size_t start_digit = status.find_first_of("0123456789");
	if (start_digit == std::string::npos)
		throw HTTPStatus<400>();
	size_t last_digit = status.find_last_of("0123456789");
	status = status.substr(start_digit, last_digit);
	if (status.find_first_not_of("0123456789") != std::string::npos)
		throw HTTPStatus<400>();
	this->status_code = str_to_nbr<int>(status);

	// Body Section
	crlf = crlf+4;
	bdy = bdy.substr(crlf);
	res.set_body(bdy);
	res.set_header("Content-Length", to_string(bdy.size()));
}

/*
	Hammertime because HTTPS only takes const values as template parameters
	Default is 500 because Server is expecting to receive an HTTP compliant
	code. If it doesn't then Internal Server Error (500)
*/

BaseStatus CGIHandler::script_status( void )
{
    if (status_code == 100)
        return (HTTPStatus<100>());
    if (status_code == 101)
        return (HTTPStatus<101>());
    if (status_code == 200)
        return (HTTPStatus<200>());
    if (status_code == 201)
        return (HTTPStatus<201>());
    if (status_code == 202)
        return (HTTPStatus<202>());
    if (status_code == 203)
        return (HTTPStatus<203>());
    if (status_code == 204)
        return (HTTPStatus<204>());
    if (status_code == 205)
        return (HTTPStatus<205>());
    if (status_code == 206)
        return (HTTPStatus<206>());
    if (status_code == 207)
        return (HTTPStatus<207>());
    if (status_code == 300)
		return (HTTPStatus<300>());
	if (status_code == 301)
		return (HTTPStatus<301>());
	if (status_code == 302)
		return (HTTPStatus<302>());
	if (status_code == 303)
		return (HTTPStatus<303>());
	if (status_code == 304)
		return (HTTPStatus<304>());
	if (status_code == 305)
		return (HTTPStatus<305>());
	if (status_code == 307)
		return (HTTPStatus<307>());
	if (status_code == 400)
		return (HTTPStatus<400>());
	if (status_code == 401)
		return (HTTPStatus<401>());
	if (status_code == 402)
		return (HTTPStatus<402>());
	if (status_code == 403)
		return (HTTPStatus<403>());
	if (status_code == 404)
		return (HTTPStatus<404>());
	if (status_code == 405)
		return (HTTPStatus<405>());
	if (status_code == 406)
		return (HTTPStatus<406>());
	if (status_code == 407)
		return (HTTPStatus<407>());
	if (status_code == 408)
		return (HTTPStatus<408>());
	if (status_code == 409)
		return (HTTPStatus<409>());
	if (status_code == 410)
		return (HTTPStatus<410>());
	if (status_code == 411)
		return (HTTPStatus<411>());
	if (status_code == 412)
		return (HTTPStatus<412>());
	if (status_code == 413)
		return (HTTPStatus<413>());
	if (status_code == 414)
		return (HTTPStatus<414>());
	if (status_code == 415)
		return (HTTPStatus<415>());
	if (status_code == 416)
		return (HTTPStatus<416>());
	if (status_code == 417)
		return (HTTPStatus<417>());
	if (status_code == 422)
		return (HTTPStatus<422>());
	if (status_code == 423)
		return (HTTPStatus<423>());
	if (status_code == 424)
		return (HTTPStatus<424>());
	if (status_code == 426)
		return (HTTPStatus<426>());
	if (status_code == 428)
		return (HTTPStatus<428>());
	if (status_code == 429)
		return (HTTPStatus<429>());
	if (status_code == 431)
		return (HTTPStatus<431>());
	if (status_code == 451)
		return (HTTPStatus<451>());
	if (status_code == 500)
		return (HTTPStatus<500>());
	if (status_code == 501)
		return (HTTPStatus<501>());
	if (status_code == 502)
		return (HTTPStatus<502>());
	if (status_code == 503)
		return (HTTPStatus<503>());
	if (status_code == 504)
		return (HTTPStatus<504>());
	if (status_code == 505)
		return (HTTPStatus<505>());
	if (status_code == 507)
		return (HTTPStatus<507>());
	if (status_code == 511)
		return (HTTPStatus<511>());
	return (HTTPStatus<500>());
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