#include <netinet/in.h>
#include <iostream>
#include <map>
#include <vector>

typedef int Function; // Stand-in for function pointer

// struct sockaddr_in
// {
//     __uint8_t         sin_len;
//     sa_family_t       sin_family;
//     in_port_t         sin_port;
//     struct in_addr    sin_addr;
//     char              sin_zero[8];
// };

typedef struct sockaddr_in SocketAddress;

class Socket { // WIP
	SocketAddress	address;
	int				port;
};

struct ServerConfig {
  std::string	server_name;
  std::string	bind_address;
  int			port;
  int			max_clients;
  int			request_timeout;
  std::string	root;
  int			input_buffer_size;
};

class Parser {
	Parser(std::string config_file);
	std::string		config_file;
	bool			is_valid( void );
	ServerConfig	get_config( void );
};

typedef std::map<int, std::string> Errors;
Errors errors; // fill with map in ./http_codes.rb

template <int Code>
class HTTPStatus: public std::exception {
	int				code;
	std::string		reason_phrase;
	std::string		type;
	HTTPStatus<Code>( void ): code(Code), reason_phrase(errors[Code]) {};
	bool 			is_info( void );
	bool 			is_success( void );
	bool 			is_redirect( void );
	bool 			is_error( void );
	bool 			is_client_error( void );
	bool 			is_server_error( void );
};

typedef std::map<std::string, std::string> ResponseHeader;
typedef std::map<std::string, std::string> RequestHeader;
typedef std::map<std::string, std::string> RequestQuery;
typedef std::map<std::string, std::string> RequestAttributes;
typedef std::map<std::string, std::string> RequestMeta;

enum RequestMethod {
	GET,
	POST,
	DELETE,
};

class URI {
	std::string		host;
	std::string		port;
	std::string		path;
	std::string		query;

	std::string		to_s( void ) {
		return std::string("http://") + host + std::string(":") + port + path + query;
	}
};

class Request {
public:
	Socket				socket;
	std::string			request_line; 		// The complete request line such as: `GET / HTTP/1.1`
	RequestMethod		request_method;
	std::string			unparsed_uri; 		// The unparsed URI of the request
	URI					request_uri;		// The parsed URI of the request
	std::string			path;
	std::string			path_info;			// The script name (CGI variable)
	std::string			query_string;		// The query from the URI of the request
	std::string			raw_header;			// The raw header of the request
	RequestHeader		header;				// The parsed header of the request
	std::string			accept;				// The Accept header value
	std::string			accept_charset;		// The Accept-Charset header value
	std::string			accept_encoding;	// The Accept-Encoding header value
	std::string			accept_language;	// The Accept-Language header value
	SocketAddress		addr;				// The socket address of the server
	SocketAddress		peeraddr;			// The socket address of the client
	RequestAttributes	attributes;			// Map of request attributes
	std::string			request_time;		// The local time this request was received
	Request( ServerConfig config );

	// Parses a request from +socket+.  This is called internally by Server
	void	parse(Socket socket) {
		// ...
		read_request_line(socket);
		read_header(socket);
		// set accept_* values
		// ...
		setup_forwarded_info();
		request_uri = parse_uri(unparsed_uri);
		// set other member variables
	}

	// Generate HTTP/1.1 100 continue response if the client expects it
	void			http_continue( void );

	// Returns the request body.
	std::string		body( void ) {
		// ...
		Function pointer;
		return read_body(socket, pointer);
	}

	Request			body_reader( void ); 		// Returns `this`, Prepares the Request object for use
	RequestQuery	query( void ); 				// Request query as a Map
	int				content_length( void );		// The content-length header
	std::string 	content_type( void );		// The content-type header
	void			each(Function pointer); 	// Iterates over each header in the request
	std::string		host( void );				// The host this request is for
	int				port( void );				// The port this request is for
	std::string		server_name( void );		// The server name this request is for
	std::string		remote_ip( void );			// The client's IP address
	RequestMeta		meta_vars( void );			// Browse document of CGI Version 1.1, see http://tools.ietf.org/html/rfc3875

	// Retrieves the request header +field+
	std::string operator [](std::string field) const {return header[field];}

	// Convert to string
	std::string		to_s( void ) { return request_line + raw_header + body(); }

private:
	static const int	MAX_URI_LENGTH = 2083;
	static const int	MAX_HEADER_LENGTH = (112 * 1024);

	// In each:
	// 		call this.read_line(socket, max_length),
	// 		sets relevant member variable,
	// 		throws appropriate errors
	void			read_request_line(Socket socket);
	void			read_header(Socket socket);

	URI				parse_uri(std::string str);
	std::string		read_body(Socket socket, Function pointer);
	std::string		read_line(Socket socket, int size);

	// Forward method and args to Socket or other IO object
	std::string		read_data(Socket socket,
							  std::string method,
							  std::vector<std::string> args);

	void			parse_query( void ); 			// Parse and set query member variable
	void			setup_forwarded_info( void );
};

class InvalidHeader: public std::exception {};		// Error class for Header

// An HTTP response.  This is filled in by the `service` method of a Handler
class Response {
public:
	int					status;			// Response status code (200)
	ResponseHeader		header;
	std::string			reason_phrase;	// Response reason phrase ("OK")

	// Body may be:
	// * a std::String;
	// * an IO-like object that responds to +#read+ and +#readpartial+;
	// * a Function pointer-like object that responds to +#call+.
	std::string			body;

	RequestMethod		request_method;
	URI					request_uri;
	std::string			filename; 		// Filename of the static file in this response.  Only used by FileHandler
	ServerConfig		config;			// Configuration for this response
	int					sent_size;		// Bytes sent in this response
	Response(ServerConfig config);

	// The response's HTTP status line
	std::string			status_line( void )  { return "HTTP/1.1 " + status + reason_phrase; }

	// Sets the response's status to the +status+ code, and reason_phrase
	void				set_status(int status);

	// Retrieves the response header +field+
	std::string operator [](std::string field) const {return header[field];}
	// Sets the response header +field+ to +value+
    std::string & operator [](std::string field) {return header[field];}

	int				content_length( void );				// The content-length header
	std::string 	content_type( void );				// The content-type header
	void			set_content_length(int length);		// Sets the content-length header to +len+
	void			set_content_type(std::string type);	// Sets the content-type header to +type+
	void			each(Function pointer); 			// Iterates over each header in the response
	void			setup_header( void );				// Sets up the headers for sending
	void			set_error(std::exception ex);		// Creates an error page for exception +ex+
    void			send_header(Socket socket);			// Sends the headers on +socket+

	// Sends the response on +socket+
	void			send_response(Socket socket) {
		try {
			setup_header();
			send_header(socket);
			send_body(socket);
		} catch (std::exception error) { };
	}

	void			send_body(Socket socket) {
		// delegate based on body type
		if (body == "IO")
			send_body_io(socket);
		else if (body == "Function")
			send_body_pointer(socket);
		else
			send_body_string(socket);
	}

private:
	std::string		check_header(std::string header_value); // throws InvalidHeader
	void			error_body( void );						// HTML for error on body
	void			send_body_io(Socket socket);			// Socket.send(data) set this.sent_size
	void			send_body_pointer(Socket socket);
	void			send_body_string(Socket socket);
};

class Handler {
	virtual void	service(Request req, Response res) = 0;
};
class FileHandler: public Handler {
	void			service(Request req, Response res);
};
class CGIHandler: public Handler {
	void			service(Request req, Response res);
};

enum ServerStatus {
	Stop,
	Running,
	Shutdown,
};

class Server {
	ServerStatus			status;
	ServerConfig			config;
	void					request_callback(Request req, Response res);
	int						tokens;		// Tokens control the number of outstanding clients.  The MaxClients configuration sets this.
	std::vector<Socket>		listeners; 	// Sockets listening for connections.
	Server(ServerConfig config) {
		// ...
		listen(config.bind_address, config.port);
	}
	void		listen(std::string address, int port);	// Adds listener from +address+ and +port+ to the server (listeners[])
	void		start( void ); 		// calls run(), Starts the server and runs for each connection.
	void		stop( void ); 		// Stops the server from accepting new connections.
	void		shutdown( void );	// Shuts down the server and all listening sockets.  New listeners must be provided to restart the server.

	// Main call method on Server
	void	run(Socket socket) {
		Request 	req(config);
		Response 	res(config);
		try {
			// while timeout and Running
			req.parse(socket);
			res.request_method = req.request_method;
			res.request_uri = req.request_uri;
			if (request_callback) {
				request_callback(req, res);
			}
			service(req, res);
		}
		catch (HTTPStatus error) {
			res.set_error(error);
			if (error.code) {
				res.status = error.code;
			}
		}
		if (req.request_line != "") {
			res.send_response(socket);
		}
	}

	// Services +req+ and fills in +res+
	void	service(Request req, Response res) {
		// Pick handler::service function pointer based on available info
		// ..
		// req.script_name = script_name;
		// req.path_info = path_info;
		FileHandler handler;
		handler.service(req, res);
	}

private:
	Socket	accept_client(Server server);	// Accepts a TCP client socket from the TCP server socket and returns the client socket.
	void	cleanup_listener( void );		// close FD log("close TCP Socket $(address)")
};

