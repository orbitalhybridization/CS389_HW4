//cache_lib.cc

#include "cache.hh"
#include "fifo_evictor.hh"
#include <vector>
#include <iostream>
#include <map>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

class Cache::Impl
{

public:

    Impl(size_type maxmem,
        float max_load_factor = 0.75,
        Evictor* evictor = nullptr,
        hash_func hasher = std::hash<key_type>()), : max(maxmem),
        max_load_factor(max_load_factor), evictor(evictor), hasher(hasher)
        {storage = std::unordered_map<key_type, std::pair<val_type, size_type>, hash_func> (1, hasher);}

    // Cache client constructor
    Impl(std::string host, std::string port), : host_ip(host), external_port(port) {
	
	//try to establish connection to host via port given
	try
    	{

		// The io_context is required for all I/O
		net::io_context ioc;

		// These objects perform our I/O
		tcp::resolver resolver(ioc);
		beast::tcp_stream stream(ioc);

		// Look up the domain name
		auto const results = resolver.resolve(host_ip, external_port);

		// Make the connection on the IP address we get from a lookup
		stream.connect(results);
		
		// not_connected happens sometimes
		// so don't bother reporting it.
		//
		if(ec && ec != beast::errc::not_connected)
		    throw beast::system_error{ec};

	    }

	    //if don't not not work, end session w/ error.	
	    catch(std::exception const& e)
	    {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	    }
	
	}

    size_type max;
    std::string host_ip;
    std::string external_port;
    float max_load_factor;
    Evictor* evictor;
    hash_func hasher;
    size_type tracker = 0.0;

    ~Impl() = default;

    void set(key_type key, val_type val, size_type size){


		// Set up an HTTP PUT request message
		http::request<http::string_body> req{http::verb::put, target, version};
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Send the HTTP request to the remote host
		http::write(stream, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(stream, buffer, res);

		// Write the message to standard out, this should be the value of key
		// and the size of the value
		std::cout << res << std::endl;

		// Gracefully close the socket
		//beast::error_code ec;
		//stream.socket().shutdown(tcp::socket::shutdown_both, ec);

		// If we get here then the connection is closed gracefully
	    	//return EXIT_SUCCESS;


    }

     // Retrieve a pointer to the value associated with key in the cache,
     // or nullptr if not found.
     // Sets the actual size of the returned value (in bytes) in val_size.
     val_type get(key_type key, size_type& val_size) {

		// Set up an HTTP GET request message
		http::request<http::string_body> req{http::verb::get, target, version};
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Send the HTTP request to the remote host
		http::write(stream, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(stream, buffer, res);

		// Write the message to standard out, this should be the value of key
		// and the size of the value
		std::cout << res << std::endl;

		// Gracefully close the socket
		//beast::error_code ec;
		//stream.socket().shutdown(tcp::socket::shutdown_both, ec);

		// If we get here then the connection is closed gracefully
	    	//return EXIT_SUCCESS;


     // Delete an object from the cache, if it's still there
     bool del(key_type key) {


		// Set up an HTTP DELETE_ request message
		http::request<http::string_body> req{http::verb::delete_, target, version};
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Send the HTTP request to the remote host
		http::write(stream, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(stream, buffer, res);

		// Write the message to standard out, this should be the value of key
		// and the size of the value
		std::cout << res << std::endl;

		// Gracefully close the socket
		//beast::error_code ec;
		//stream.socket().shutdown(tcp::socket::shutdown_both, ec);

		// If we get here then the connection is closed gracefully
	    	//return EXIT_SUCCESS;


     }

     // Compute the total amount of memory used up by all cache values (not keys)
     size_type space_used() const {

     }

     // Delete all data from the cache
     void reset(){


		// Set up an HTTP RESET request message
		http::request<http::string_body> req{http::verb::put, target, version};
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Send the HTTP request to the remote host
		http::write(stream, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(stream, buffer, res);

		// Write the message to standard out, this should be the value of key
		// and the size of the value
		std::cout << res << std::endl;

		// Gracefully close the socket
		//beast::error_code ec;
		//stream.socket().shutdown(tcp::socket::shutdown_both, ec);

		// If we get here then the connection is closed gracefully
	    	//return EXIT_SUCCESS;

     }

 private:     
    std::unordered_map<key_type, std::pair<val_type,size_type>,hash_func> storage; //stores value pointer and size of value

    hash_func hasherer;
};

Cache::Cache(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher)
: pImpl_(new Impl(maxmem, max_load_factor, evictor, hasher))
{}

Cache::~Cache() {} //don't set to default...will give memory error

void Cache::set(key_type key, 
        Cache::val_type val, 
        Cache::size_type size)
{
    pImpl_->set(key,val,size);
}

Cache::val_type Cache::get(key_type key, 
        Cache::size_type& val_size) const
{
    return pImpl_->get(key,val_size);
}

bool Cache::del(key_type key)
{
    return pImpl_->del(key);
}

Cache::size_type Cache::space_used() const
{
    return pImpl_->space_used();
}

void Cache::reset()
{
    pImpl_->reset();
}
