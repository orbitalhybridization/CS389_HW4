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
#include <boost/beast/core/basic_stream.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

class Cache::Impl
{

public:

    // Set up member vars
    std::string host_ip_;
    std::string external_port_;
    int version = 10;
    tcp::resolver::results_type results;
    net::io_context ioc; // The io_context is required for all I/O
    // These objects perform our I/O
    tcp::resolver resolver_;
    beast::tcp_stream* stream_;

    ~Impl() {
        beast::error_code ec;
        stream_->socket().shutdown(tcp::socket::shutdown_both, ec); //close stream
    }    

    // Cache client constructor
    Impl(std::string host, std::string port) : resolver_(ioc) {

	host_ip_ = host;
	external_port_ = port;
	
	//try to establish connection to host via port given
	try
    	{

		// Look up the domain name
		results = resolver_.resolve(host_ip_, external_port_);

		// Make the connection on the IP address we get from a lookup
		stream_ = new beast::tcp_stream(ioc);
		stream_->connect(results);
		
		// not_connected happens sometimes
		// so don't bother reporting it.
		//
		//if(ec && ec != beast::errc::not_connected)
		//    throw beast::system_error{ec};
		

	}

	    //if don't not not work, end session w/ error.	
	    catch(std::exception const& e)
	    {
		std::cerr << "Error: " << e.what() << std::endl;
		return;
	    }
	
	}

    void set(key_type key, val_type val, size_type){


		// Set up an HTTP PUT request message
		auto target = "/" + key + "/" + val; // build target 
		http::request<http::string_body> req{http::verb::put,target,version};
		req.set(http::field::host, host_ip_);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Send the HTTP request to the remote host
		http::write(*stream_,req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		// Make sure to parse and check for errors!
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(*stream_, buffer, res);

		// Write the message to standard out, this should be the value of key
		// and the size of the value.
		std::cout << res << std::endl;

		return;


    }

     // Retrieve a pointer to the value associated with key in the cache,
     // or nullptr if not found.
     // Sets the actual size of the returned value (in bytes) in val_size.
     val_type get(key_type key, size_type&) {

		// Set up an HTTP GET request message
		auto target = key; // build target
		http::request<http::string_body> req{http::verb::get, target, version};
		req.set(http::field::host, host_ip_);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Send the HTTP request to the remote host
		http::write(*stream_, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(*stream_, buffer, res);

		// Write the message to standard out, this should be the value of key
		// and the size of the value
		std::cout << res << std::endl;

		// cast res as val_type because it comes in as a string
		std::string string = res.body();

		return res;

    }

/*
     // Delete an object from the cache, if it's still there
     bool  del(key_type key) {


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
        // Set up an HTTP head request message


        //might wanna double check This                                   ~Arthur
        http::request<http::string_body> req{http::verb::head, target, version};
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

     // Delete all data from the cache
     void reset(){


		// Set up an HTTP RESET request message
        //shouldn't http verb be post for reset? Instructions said that post should be only for reset ~ Arthur
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
*/
 private:     
    std::unordered_map<key_type, std::pair<val_type,size_type>,hash_func> storage; //stores value pointer and size of value

    hash_func hasherer;
};


Cache::Cache(std::string host, std::string port)
: pImpl_(new Impl(host, port))
{}

Cache::~Cache() {} //don't set to default...will give memory error

void Cache::set(key_type key, 
        Cache::val_type val, 
        Cache::size_type size)
{
    pImpl_->set(key,val,size);
}
/*
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
*/
