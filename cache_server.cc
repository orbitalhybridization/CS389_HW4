// Cache Server
// Code adapted from Boost Beast async server example:
// https://www.boost.org/doc/libs/develop/libs/beast/example/http/server/async/http_server_async.cpp

#include "cache.hh"
#include <iostream>
#include <string>
#include <cassert>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/value_semantic.hpp>

// Use boost to parse command line and beast for server
namespace parse_cmd = boost::program_options;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.
template<
    class Body, class Allocator,
    class Send>
void
handle_request(
    Cache* cache_,
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send)
{
    // Returns a bad request response
    auto const bad_request =
    [&req](beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
    [&req](beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
    [&req](beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };


    //Check if any of the available methods are found in request
    if(req.method() != http::verb::get &&
       req.method() != http::verb::head &&
       req.method() != http::verb::put &&
       req.method() != http::verb::delete_ &&
       req.method() != http::verb::post){
       return send(bad_request("Unknown HTTP-method"));}

    // Handle an unknown error
    beast::error_code ec;
    if(ec)
        return send(server_error(ec.message()));

    //handle put
    if(req.method() == http::verb::put) {
        //cache->set(key,value); //create or replace k,v pair in cache
        //assert(cache->get(key) == value); //check that this worked properly
	std::cout << "Made it here!" << std::endl;
    }

		/*
		//handle get
		if(req.method() == http::verb::get) {

		}

		//handle delete
		if(req.method() == http::verb::delete_) {
			http::response<http::file_body> res{
				std::piecewise_construct,
				std::make_tuple(std::move(body)),
				std::make_tuple(http::status::ok, req.version())};
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, mime_type(path));
			res.content_length(size);
			res.keep_alive(req.keep_alive());
			return send(std::move(res));
		}

		//handle head
		if(req.method() == http::verb::head) {
			http::response<http::empty_body> res{http::status::ok, req.version()};
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, mime_type(path));
			res.content_length(size);
			res.keep_alive(req.keep_alive());
			return send(std::move(res));
		}

		//handle post
		if(req.method() == http::verb::post) {

		}

		//handle errors
		//err codes: 3xx/5xx
		// Returns a server error response
		
		// from async server example
		auto const server_error = [&req](beast::string_view what)
		{
			http::response<http::string_body> res{http::status::internal_server_error, req.version()};
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = "An error occurred: '" + std::string(what) + "'";
			res.prepare_payload();
			return res;
		};


		    // Respond to HEAD request
		    if(req.method() == http::verb::head)
		    {
			http::response<http::empty_body> res{http::status::ok, req.version()};
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, mime_type(path));
			res.content_length(size);
			res.keep_alive(req.keep_alive());
			return send(std::move(res));
		    }

		    // Respond to GET request
		    http::response<http::file_body> res{
			std::piecewise_construct,
			std::make_tuple(std::move(body)),
			std::make_tuple(http::status::ok, req.version())};
		    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		    res.set(http::field::content_type, mime_type(path));
		    res.content_length(size);
		    res.keep_alive(req.keep_alive());
		    return send(std::move(res));
		*/
} // end of handle_request()

// Report a failure
void
fail(beast::error_code ec, char const* error)
{
    std::cerr << error << ": " << ec.message() << "\n";
}

// Session class
class session : public std::enable_shared_from_this<session>
{

    // This is the C++11 equivalent of a generic lambda.
    // The function object is used to send an HTTP message.
    struct send_lambda
    {
        session& self_;

        explicit
        send_lambda(session& self)
            : self_(self)
        {
        }

        template<bool isRequest, class Body, class Fields>
        void
        operator()(http::message<isRequest, Body, Fields>&& msg) const
        {
            // The lifetime of the message has to extend
            // for the duration of the async operation so
            // we use a shared_ptr to manage it.
            auto sp = std::make_shared<
                http::message<isRequest, Body, Fields>>(std::move(msg));

            // Store a type-erased version of the shared
            // pointer in the class to keep it alive.
            self_.res_ = sp;

            // Write the response
            http::async_write(
                self_.stream_,
                *sp,
                beast::bind_front_handler(
                    &session::on_write,
                    self_.shared_from_this(),
                    sp->need_eof()));
        }
    };

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    std::shared_ptr<void> res_;
    send_lambda lambda_;
    Cache* cache_;

public:
    // Take ownership of the stream
    session(
	Cache* cache,
        tcp::socket&& socket)
        : stream_(std::move(socket)),
	lambda_(*this),
	cache_(cache)
    {
    }

    // Start the asynchronous operation
    void
    run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(stream_.get_executor(),
                      beast::bind_front_handler(
                          &session::do_read,
                          shared_from_this()));
    }

    void
    do_read()
    {
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        req_ = {};

        // Set the timeout.
        stream_.expires_after(std::chrono::seconds(30));

        // Read a request
        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }

    void
    on_read(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection
        if(ec == http::error::end_of_stream)
            return do_close();

        if(ec)
            return fail(ec, "read");

        // Send the response
        handle_request(cache_, std::move(req_), lambda_);
    }

    void
    on_write(
        bool close,
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");

        if(close)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }

        // We're done with the response so delete it
        res_ = nullptr;

        // Read another request
        do_read();
    }

    void
    do_close()
    {
        // Send a TCP shutdown
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

        // At this point the connection is closed gracefully
    }
};

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    Cache* cache_;

public:
    listener(
	Cache* cache,
        net::io_context& ioc,
        tcp::endpoint endpoint)
        : ioc_(ioc)
        , acceptor_(net::make_strand(ioc))
	, cache_(cache)
    {
        beast::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if(ec)
        {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if(ec)
        {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if(ec)
        {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor_.listen(
            net::socket_base::max_listen_connections, ec);
        if(ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void
    run()
    {
        do_accept();
    }

private:
    void
    do_accept()
    {
        // The new connection gets its own strand
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()));
    }

    void
    on_accept(beast::error_code ec, tcp::socket socket)
    {
        if(ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(cache_,
                std::move(socket))->run();
        }

        // Accept another connection
        do_accept();
    }
};


// MAIN
int main(int argc, char** argv){

	//// SET UP

	// Set up defaults
	Cache::size_type maxmem = 100;
	std::string address = "127.0.0.1";
	unsigned short port = 6969;
	int threads = 1;

	// Set up args options for help menu
	parse_cmd::options_description options("Input Options");
	options.add_options()
		("-m maxmem", parse_cmd::value<Cache::size_type>(), "maximum memory for Cache object (default = 100)")
		("-s server", parse_cmd::value<int>(),"address of server (default = 127.0.0.1)")
		("-p port", parse_cmd::value<int>(),"which TCP port to connect to (default = 69)")
		("-t threads", parse_cmd::value<int>(), "how many threads to use (default = 1)")
		("help", "show help options");

	// Set up args obtained from command line in variable map
	parse_cmd::variables_map vm;	
	parse_cmd::store(parse_cmd::parse_command_line(argc,argv,options), vm);
	parse_cmd::notify(vm);

	// Check for each optional arg
	if (vm.count("help")) {
		std::cout << options << std::endl;
		return 0;
	}

	if (vm.count("-m")) {
		maxmem = vm["-m"].as<Cache::size_type>();
	}

	if (vm.count("-s")) {
		address = vm["-s"].as<std::string>();
	}


	if (vm.count("-p")) {
		port = vm["-p"].as<unsigned short>();
	}

	if (vm.count("-t")) {
		threads = vm["-t"].as<int>();
	}

	// Set up cache
	Cache* cache = new Cache(maxmem);
	
	//// SET UP SERVER
        beast::error_code ec; // error code for parsing ip
	
	net::ip::address ip_address =
		net::ip::address::from_string(address, ec); // get ip address as address object :)
		
	net::io_context ioc{threads}; // set up basic IO

  	// Create and launch a listening port
    	std::make_shared<listener>(
		cache,
		ioc,
		tcp::endpoint{ip_address, port}
	)->run();
    
	// Run the I/O service on one thread
	ioc.run();

	return 1;
}
