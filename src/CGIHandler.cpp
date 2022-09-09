/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:01:30 by gleal             #+#    #+#             */
/*   Updated: 2022/09/09 00:26:19 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

#include <stdio.h>
#include <sys/wait.h>

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
	#if defined(DARWIN)
	temp[".php"] = full_path("test/cgi/php-cgi-mac");
	temp[".cgi"] = full_path("test/cgi/cpp/cgi_tester_mac");
	#endif
	#if defined(LINUX)
	temp[".php"] = full_path("test/cgi/php-cgi-linux");
	temp[".cgi"] = full_path("test/cgi/cpp/cgi_tester_ubuntu");
	#endif
	
	return (temp);
}

bool CGIHandler::extension_is_implemented( const std::string &extension )
{
	return(extension_interpreter.count(extension));
}

/* Constructors */
CGIHandler::CGIHandler( void ) { /* no-op */ }

CGIHandler::CGIHandler( const URI &uri, const in_addr &connection_addr )
: query_string(uri.query), connection_address(&connection_addr), uri(&uri)
{
	// script_path = script_path_parse(uri.path);
	// extra_path = extra_path_parse(uri.path);

	script_path = uri.path;
	extension = get_extension(script_path);
	interpreter = extension_interpreter[extension];
	std::cout <<
	"Script Path is " << script_path << std::endl <<
	"Extension is " << extension << std::endl <<
	"Extra_path is " << extra_path << std::endl <<
	"Query_string is " << query_string << std::endl <<
	"Interpreter is " << interpreter << std::endl <<
	"Connection_address is " << inet_ntoa(*connection_address);
}

CGIHandler::CGIHandler( CGIHandler const & src ) : Handler(){ *this = src; }

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

/*
	https://github.com/gleal42/webserv/issues/18

	So CGI basically needs 3 things:
	- Standard Input with request body (in case of Post requests)
	- Script to execute as argument
	- Environment Variables
		- QUERY_STRING for GET Requests
		- Others defined in RFC

	In order to send environment variables we use fork and execve:

	However there are 4 ways to achieve interprocess communication:
	- Memory Mapped files
	- Shared memory
	- Named pipes
	- Local TCP/UDP sockets

	These are ordered in from most performant to least performant
	(check issue links)

	I considered using shared memory however we couldn't perform
	dup2 on shared memory fds.
	I might try to use mmap instead of writing (will compare performance).

	Usually in C we allocate memory using malloc/strdup. However I used the
	allocator from vector. The problem is that when we leave scope vector
	frees memory. This sucks because the function is quite big and there's
	not much I can do about it.

	Waitpid terminates with signals and debugger lldb sends an interrupt
	signal. Don't know how to handle this.

	This line was used to debug what was being written:
	FILE *output_file = fopen("Checking.txt", "w+");
*/

void	CGIHandler::execute_cgi_script( Request & req, Response & res  )
{
	FILE *input_file = tmpfile();
	FILE *output_file = tmpfile();
	if (input_file == NULL || output_file == NULL) {
		std::cerr << "Temp file error" << std::endl;
		throw HTTPStatus<500>();
	}
	int input_fd = fileno(input_file);
	int output_fd = fileno(output_file);
	std::cout << "Body is " << req._raw_body.data() << std::endl;
	write(input_fd, req._raw_body.data(), req._raw_body.size());
	rewind(input_file);

	pid_t pid = 0, w_pid = 0;
	int status;
	pid=fork();
	if (pid == 0)
	{
		// Environment Variables
		std::vector<char *> env_vars;
		std::vector< std::vector<char> > buf = environment_variables(req);
		convert_to_charptr_vec(buf, env_vars);
		char *const *envs = env_vars.data();
		print_env_variables(envs, (env_vars.size() - 1));

		// CGI arguments (1st and 2nd execve argument)
		std::vector<char *> cgi_args;
		std::vector<char> cmd_vec = convert_to_char_vector(filename(interpreter));
		std::vector<char> filepath = convert_to_char_vector(script_path);
		cgi_args.push_back(cmd_vec.data());
		cgi_args.push_back(filepath.data());
		cgi_args.push_back(NULL);
		std::cout << "Interpreter is " << interpreter << std::endl;
		std::cout << "CGI 1 Argument is " << cgi_args[0] << std::endl;
		std::cout << "CGI 2 Argument is " << cgi_args[1] << std::endl;

		dup2(input_fd, STDIN_FILENO);
		dup2(output_fd, STDOUT_FILENO);
		execve(interpreter.c_str(), cgi_args.data(), envs);
		// exit(EXIT_FAILURE);
	}
	w_pid = waitpid(pid, &status, 0);
	(void)w_pid;
	// // if (w_pid == -1) // LLDB sends EINTR signal{
	// // 	throw HTTPStatus<500>();
	// // }
	if (status == EXIT_FAILURE) {
		// std::cerr << "Child ERROR" << std::endl;
		throw HTTPStatus<500>();
	}
	set_response(file::get_string(output_file, output_fd), res);
	fclose(input_file);
	fclose(output_file);
}

/*
	Converts vector of chars to string by accessing data.
	We need to use a refence to vector so that strings continue
	valid when we leave function scope.
*/

void	CGIHandler::convert_to_charptr_vec(std::vector< std::vector<char> > &vec_of_vec, std::vector<char *> &vec_of_charptr)
{
	for (std::vector< std::vector<char> >::iterator it = vec_of_vec.begin();
		it != vec_of_vec.end();
		it++)
	{
		vec_of_charptr.push_back(it->data());
	}
	vec_of_charptr.push_back(NULL);
}

/*
	I set all environment variables so that CGI can have access to them
	in the script.
	These will be passed to cgi throw 3rd parameter of execve

	Does it make sense to turn Req method into string instead of enum
	so that this temporary std::map is not necessary for first setenv.
*/

std::vector< std::vector<char> >	CGIHandler::environment_variables( Request & req )
{
	std::vector< std::vector<char> > buf;

	set_env(buf, "AUTH_TYPE", req.get_auth_type());
	set_env(buf, "CONTENT_LENGTH", to_string(req._raw_body.size()).c_str());
	set_env(buf, "CONTENT_TYPE", req._headers["Content-Type"].c_str());
	set_env(buf, "GATEWAY_INTERFACE", "CGI/1.1");
	set_env(buf, "PATH_INFO", ("/" + extra_path));
	std::string full_script_path = full_path(extra_path);
	url::decode(full_script_path);
	set_env(buf, "PATH_TRANSLATED", full_script_path);
	set_env(buf, "QUERY_STRING", query_string);
	set_env(buf, "REMOTE_ADDR", inet_ntoa(*connection_address));
	set_env(buf, "REMOTE_HOST", address_to_hostname((sockaddr *)connection_address));
	set_env(buf, "REMOTE_IDENT", ""); // The server may choose not to support this feature, or not to request the data for efficiency reasons, or not to return available identity data.
	set_env(buf, "REMOTE_USER", req.get_remote_user()); // The server may choose not to support this feature, or not to request the data for efficiency reasons, or not to return available identity data.
	set_env(buf, "REQUEST_METHOD", req.method_to_str());
	set_env(buf, "SCRIPT_NAME", script_path);
	set_env(buf, "SCRIPT_FILENAME", script_path);
	set_env(buf, "SERVER_NAME", req.get_hostname());
	set_env(buf, "SERVER_PORT", to_string(uri->port));
	set_env(buf, "SERVER_PROTOCOL", "HTTP/1.1");
	set_env(buf, "SERVER_SOFTWARE", "Bosses/V1");
	set_env(buf, "REDIRECT_STATUS", "200");
	return (buf);
}

void	CGIHandler::set_env( std::vector< std::vector<char> > &buf, const std::string &var, const std::string & value)
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

void	CGIHandler::set_response( std::string body, Response &res )
{
	// Headers Section
	LOG("CGI BODY IS " << body);
	size_t crlf = body.find(D_CRLF);
	res.save_raw_headers(body.substr(0, crlf));
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
	this->set_status_code(str_to_nbr<int>(status));

	// Body Section
	crlf = crlf+4;
	body = body.substr(crlf);
	res.set_body(body);
	res.set_header("Content-Length", to_string(body.size()));
}
