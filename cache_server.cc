# Cache Server

#include "cache.hh"
#include <iostream>
#include <string>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
//#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>

# Use boost to parse command line and beast for server
namespace parse_cmd = boost::program_options;
namespace server = boost::beast;
namespace http = beast::http;	// from <boost/beast/http.hpp>
using tcp = boost::asio::ip::tcp;	// from <boost/asio/ip/tcp.hpp>
using namespace std;

# Define localhost globally
string localhost = "127.0.0.1";

int main(int argc, char** argv){

	//// SET UP

	// Set up defaults
	int port = 6969;
	Cache::size_type maxmem = 100;
	int threads = 1;
	auto server = HTTP();

	// Set up args options for help menu
	parse_cmd::options_description options("Input Options");
	options.add_options()
		("-m maxmem", parse_cmd::value<Cache::size_type>, "maximum memory for Cache object (default = 100)")
		("-s server", "not sure what this does yet, might be a boolean?)
		("-p port", parse_cmd::value<int>(),"which TCP port to connect to (default = 69)")
		("-t threads", parse_cmd::value<int>(), "how many threads to use (default = 1)")
		("help", "show help options");

	// Set up args obtained from command line in variable map
	parse_cmd::variables_map vm;	
	parse_cmd::store(parse_cmd::parse_command_line(argc,argv,desc), vm);
	parse_cmd::notify(vm);

	// Check for each optional arg
	if (vm.count("help")) {
		cout << options << endl;
		return 0;
	}

	if (vm.count("-m")) {
		maxmem = vm["-m"].as<Cache::size_type>();
	}

	//if (vm.count("-s")) {
	//	server = vm["-s"].as<???>();
	//}


	if (vm.count("-p")) {
		port = vm["-p"].as<int>();
	}

	if (vm.count("-t")) {
		threads = vm["-t"].as<int>();
	}

	
	// do we want an evictor? hasher? max_load_factor?
	// Set up cache
	Cache* cache = new Cache(Cache::size_type maxmem);
	
	//// SET UP SERVER

	// Set up Sevrer IO
	boost::asio::io_context ioc;
	tcp::resolver resolver{ioc};
	tcp::socket socket{ioc};
	
	http::request<Body,http::basic_fields<Allocator>>&& req;
	Send&& send;

	// Make connection
	//server::http:
	
	// Server loop
	//while(True){
		
		//requests passed as req.x()

		//Check if any of the available methods are found in request
		if(req.method() != http::verb::get &&
		   req.method() != http::verb::head &&
		   req.method() != http::verb::put &&
		   req.method() != http::verb::del &&
		   req.method() != http::verb::post){
		return send(bad_request("Unknown HTTP-method"));}


		//handle get
		if(req.method() == http::verb::get) {

		}
		//handle put
		if(req.method() == http::verb::put) {

		}

		//handle delete
		if(req.method() == http::verb::del) {

		}

		//handle head
		if(req.method() == http::verb::head) {

		}

		//handle post
		if(req.method() == http::verb::post) {

		}

		//handle errors
		//err codes: 3xx/5xx
		//from : https://www.boost.org/doc/libs/develop/libs/beast/example/http/server/async/http_server_async.cpp
		//{
			http::response<http::string_body> res{http::status::internal_server_error, req.version()};
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = "An error occurred: '" + std::string(what) + "'";
			res.prepare_payload();
			//return res;
		//};

		//end condition
			//break;

	..}

	return 1;
}
