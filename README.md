# HW4: Let's network
Ian Jackson, Arthur Lawson

## Goal:  

Part 0:

Some of the bugs in our previous code we had to fix were making the cache deep copy values into a shared ptr, changing the evictor to adhere to the original "no hh alteration" rules, and changing how our evictor handles values that are too large. For some of these, we referenced the repos of the people's code we tested for HW3.

## Part 1

For our server we used curl in the linux terminal to test out each request. We followed the advice of Eitan and focused on the PUT request and the rest kind of fell in place from there. The get requestcaused a but of an issue because our values were not deep copied in our original Cache_lib. Once we fixed that, everything else was pretty straightforward.

## Part 2

network test


## Bugs, Leaks, and Warnings!

Three memory leak blocks

## Contributions / Sources

Cache_Lib Code Adapted from Hannah Hellerstein & David Riso's Repo:
https://github.com/orbitalhybridization/CS389_HW3/blob/master/Systems_HW2-master/cache_lib.cc

Beast Examples:
Async Server: https://www.boost.org/doc/libs/develop/libs/beast/example/http/server/async/http_server_async.cpp
Sync Client: https://www.boost.org/doc/libs/develop/libs/beast/example/http/client/sync/http_client_sync.cpp


TODO: 1) change del and reset(?) methods from old_evictor.hh
	6) JSON BODY TYPE
	7) error conditions in test file for client (bad key, garbage requests, etc)

