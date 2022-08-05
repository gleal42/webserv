enum LogLevel {
  FATAL = 1,
  ERROR = 2,
  WARN = 3,
  INFO = 4,
  DEBUG = 5
}

class Log {
  // log-level, messages above this level will be logged
  level: LogLevel
  // Format of the timestamp which is applied to each logged line.
  TIME_FORMAT = "[%Y-%m-%d %H:%M:%S]"

  // If no level is given INFO is chosen by default
  constructor(log_file: string, level: LogLevel) {
    // open FD for log_file file path
  }

  close: () => void

  // Logs +data+ at +level+ if the given level is above the current log level
  log: (level: LogLevel, data: string) => void

  // Shortcuts using log()
  fatal: (msg: string) => void
  error: (msg: string) => void
  warn: (msg: string) => void
  info: (msg: string) => void
  debug: (msg: string) => void

  // Will the logger output LEVEL messages ?
  isFatal: () => boolean
  isError: () => boolean
  isWarn: () => boolean
  isInfo: () => boolean
  isDebug: () => boolean
}

// Implement C methods as class member functions to accept object arguments and
// hard code some of them
class Socket {
  addr: SocketAddress
}
type SocketAddress = {

}

type ServerConfig = {
  server_name: string
  bind_address: string
  port: number
  max_clients: number
  logger: Log

  request_timeout: number
  access_log: string // or maybe FD?
  root: string
  input_buffer_size: number
  output_buffer_size: number
}

// Call once per server block? So preparse to split into server blocks?
// is there general config outside server blocks?
class ConfigParser {
  config_file: string
  constructor(config_file: string) {}
  isValid: () => boolean
  get_config: () => ServerConfig
}

// Used to manage HTTP status codes.
// Check http_codes.rb
// See http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html for more information
class HTTPStatus extends Error {
  code: number // alias to_i()
  reason_phrase: string

  static NotFound: string // just to be able to HTTPStatus.NotFound below

  // Use C++ template to make class for each

  // class Info {}
  // class Success {}
  // class Redirect {}
  // class Error {}
  // class ClientError extends Error {}
  // class ServerError extends Error {}
  // class EOFError {}

  // switch (code)
  // case 100...200; parent = Info
  // case 200...300; parent = Success
  // case 300...400; parent = Redirect
  // case 400...500; parent = ClientError
  // case 500...600; parent = ServerError
  // end

  // Check error type
  isInfo: () => boolean
  isSuccess: () => boolean
  isRedirect: () => boolean
  isError: () => boolean
  isClientError: () => boolean
  isServerError: () => boolean
}

// C++ Maps
type ResponseHeaders = {
  [key: string]: string
}
type RequestHeaders = {
  [key: string]: string
}
type RequestQuery = {
  [key: string]: string
}
type RequestHeaders = {
  [key: string]: string
}
type RequestMeta = {
  [key: string]: string
}

class HTTPRequest { // `Request` already used by JavaScript
  BODY_CONTAINABLE_METHODS = ["POST"]

  socket: Socket

  // The complete request line such as: `GET / HTTP/1.1`
  request_line: string

  request_method: 'GET' | 'POST' | 'DELETE'

  // The unparsed URI of the request
  unparsed_uri: string

  // The parsed URI of the request
  request_uri: URL

  // The request path
  path: string

  // The script name (CGI variable)
  path_info: string

  // The query from the URI of the request
  query_string: string

  // The raw header of the request
  raw_header: string

  // The parsed header of the request
  header: RequestHeaders

  // The Accept header value
  accept: string

  // The Accept-Charset header value
  accept_charset: string

  // The Accept-Encoding header value
  accept_encoding: string

  // The Accept-Language header value
  accept_language: string

  // The socket address of the server
  addr: SocketAddress

  // The socket address of the client
  peeraddr: SocketAddress

  // Map of request headers
  attributes: RequestHeaders

  // The local time this request was received
  request_time: string

  constructor(config: ServerConfig) {
  }

  // Parses a request from +socket+.  This is called internally by Server
  parse(socket: Socket): void {
    // ...
    this.read_request_line(socket)
    this.read_header(socket)
    // set accept_* values
    // ...
    this.setup_forwarded_info()
    this.request_uri = this.parse_uri(this.unparsed_uri)
    // set other member variables
  }

  // Generate HTTP/1.1 100 continue response if the client expects it
  continue: () => void

  // Returns the request body.
  body(): string | null {
    // ...
    let proc: Function
    return this.read_body(this.socket, proc)
    // ...
  }

  // Prepares the HTTPRequest object for use
  body_reader: () => HTTPRequest

  // Request query as a Map
  query: () => RequestQuery

  // The content-length header
  content_length: () => number

  // The content-type header
  content_type: () => string

  // Retrieves the request header +field+
  '[]': (field: string) => string

  // Iterates over each header in the request
  each: (f: (field: string, value: string) => any) => void

  // The host this request is for
  host: () => string

  // The port this request is for
  port: () => number

  // The server name this request is for
  server_name: () => string

  // The client's IP address
  remote_ip: () => string

  // Convert to string
  to_s(): string {
    return this.request_line + this.raw_header + this.body
  }

  // This method provides the metavariables defined by the revision 3
  // of "The WWW Common Gateway Interface Version 1.1"
  // To browse the current document of CGI Version 1.1, see below:
  // http://tools.ietf.org/html/rfc3875
  meta_vars: () => RequestMeta

  // PRIVATE

  MAX_URI_LENGTH = 2083

  MAX_HEADER_LENGTH = (112 * 1024)

  // Call this.read_line(socket, max_length)
  // sets relevant member variable
  // throws appropriate errors
  read_request_line: (socket: Socket) => void
  read_header: (socket: Socket) => void

  parse_uri: (str: string) => URL

  read_body: (socket: Socket, f: Function) => string

  read_line: (socket: Socket, size: number) => string

  // Forward method and args to Socket or other IO object
  read_data: (socket: Socket, method: string, args: string[]) => string

  // Parse and set query member variable
  parse_query: () => void

  // It's said that all X-Forwarded-* headers will contain more than one
  // (comma-separated) value if the original request already contained one of
  // these headers. Since we could use these values as Host header, we choose
  // the initial(first) value. (apr_table_mergen() adds new value after the
  // existing value with ", " prefix)
  setup_forwarded_info: () => void
}

// Handlers
// TODO: talk about handlers
class Handler {}
class FileHandler extends Handler {}
class CGIHandler extends Handler {}

// An HTTP response.  This is filled in by the service method of a handler
class HTTPResponse { // `Response` already used by JavaScript

  // Error class for Header
  // class InvalidHeader extends Error {}

  // Response status code (200)
  status: number

  // Response header
  header: ResponseHeaders

  // Response reason phrase ("OK")
  reason_phrase: string

  // Body may be:
  // * a String;
  // * an IO-like object that responds to +#read+ and +#readpartial+;
  // * a Function pointer-like object that responds to +#call+.
  body: string | 'IO' | Function

  // Request method for this response
  request_method: 'GET' | 'POST' | 'DELETE'

  // Request URI for this response
  request_uri: string | URL

  // Filename of the static file in this response.  Only used by the
  // FileHandler handler.
  filename: string

  // Configuration for this response
  config: ServerConfig

  // Bytes sent in this response
  sent_size: number

  constructor(config: ServerConfig) {
  }

  // The response's HTTP status line
  status_line(): string {
    return `HTTP/1.1 ${this.status} ${this.reason_phrase}`
  }

  // Sets the response's status to the +status+ code, and reason_phrase
  set_status: (status: number) => void

  // Retrieves the response header +field+
  '[]': (field: string) => string
  // Sets the response header +field+ to +value+
  '[]=': (field: string, value: string) => void

  // The content-length header
  content_length: () => number
  // Sets the content-length header to +len+
  set_content_length: (len: number) => void

  // The content-type header
  content_type: () => string
  // Sets the content-type header to +len+
  set_content_type: (len: string) => void

  // Iterates over each header in the response
  each: (f: (field: string, value: string) => any) => void

  // Sends the response on +socket+
  send_response(socket: Socket): void {
    try {
      this.setup_header()
      this.send_header(socket)
      this.send_body(socket)
    } catch (error) { }
  }

  // Sets up the headers for sending
  setup_header: () => void

  send_header: (socket: Socket) => void

  send_body(socket: Socket): void {
    if (this.body == 'IO')
      this.send_body_io(socket)
    else if (typeof this.body == 'function')
      this.send_body_proc(socket)
    else
      this.send_body_string(socket)
  }

  // Creates an error page for exception +ex+
  set_error: (ex: Error) => void

  // PRIVATE

  check_header: (header_value: string) => string | Error // (InvalidHeader)

  // Set HTML for error on body
  error_body: () => void

  // Socket.send(data) set this.sent_size
  send_body_io: (socket: Socket) => void
  send_body_proc: (socket: Socket) => void
  send_body_string: (socket: Socket) => void
}

class Server {
  // An HTTP server uses the following attributes:
  //
  // :access_log              Access logs management
  // :bind_address            Local address for the server to bind to
  // :document_root           Root path to serve files from
  // :document_root_options   Root path to serve files from
  // :port                    Port to listen on
  // :request_callback        Called with a request and response before each
  //                          request is serviced.
  // :request_timeout         Maximum time to wait between requests
  // :server_alias            Array of alternate names for this server
  // :server_name             Name for this server for virtual hosting

  status: 'Stop' | 'Running' | 'Shutdown'
  config: ServerConfig
  request_callback: (req: HTTPRequest, res: HTTPResponse) => void

  // same as config.logger
  logger: Log

  // Tokens control the number of outstanding clients.  The
  // MaxClients configuration sets this.
  tokens: number

  // Sockets listening for connections.
  listeners: number[]

  // Creates a new HTTP server according to +config+. The default configuration
  // comes from +default+.
  constructor(config: ServerConfig) {
    // set variables from config then
    this.listen(config.bind_address, config.port)
    // this.mount(root)
  }

  // Adds listeners from +address+ and +port+ to the server (listeners[])
  listen: (address: string, port: number) => void

  // Starts the server and runs for each connection. This method
  // does not return until the server is stopped from a signal handler
  start: () => void // calls run()

  // Stops the server from accepting new connections.
  stop: () => void

  // Shuts down the server and all listening sockets.  New listeners must be
  // provided to restart the server.
  shutdown: () => void

  // socket : TCP client socket
  run(socket: Socket): void {
    const req: HTTPRequest = new HTTPRequest(this.config)
    const res: HTTPResponse = new HTTPResponse(this.config)

    try {
      // while timeout and Running
      // raise errors if needed
      req.parse(socket)
      res.request_method = req.request_method
      res.request_uri = req.request_uri

      if (this.request_callback) {
        this.request_callback.call(req, res)
      }

      this.service(req, res)
    }
    // Handle Server and Client errors
    catch (error) {
      this.logger.error(error.message)
      res.set_error(error)
      if (error.code) {
        res.status = error.code
      }
    }

    // Send response
    if (req.request_line) {
      res.send_response(socket)
      this.access_log(this.config, req, res)
    }
  }

  // Services +req+ and fills in +res+
  service(req: HTTPRequest, res: HTTPResponse): void {
    const file = this.search_file(req.path)
    if (!file) {
      throw(HTTPStatus.NotFound, `${req.path}' not found.`)
    }

    // req.script_name = script_name
    // req.path_info = path_info
    //
    // handler.service(req, res)
  }

  search_file: (path: HTTPRequest['path']) => string

  // Logs +req+ and +res+ in the access logs.
  access_log: (config: ServerConfig, req: HTTPRequest, res: HTTPResponse) => void

  // PRIVATE

  // Accepts a TCP client socket from the TCP server socket and returns
  // the client socket.
  accept_client: (server: Server) => Socket

  // close FD logger.debug("close TCP Socket $(address)")
  cleanup_listener: () => void
}

