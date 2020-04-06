# HW4: Let's network
Ian Jackson, Arthur Lawson

## Goal:  

## Part 1


## Part 2

We got around the memory ownership issue with Cache::get() by deep copying the value into a sharedptr ???_----

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

