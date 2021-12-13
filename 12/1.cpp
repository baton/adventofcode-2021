#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <iterator>


using vertex_t = std::string;
using graph_t = std::map< vertex_t, std::set< vertex_t > >;
using path_t = std::vector< vertex_t >;
using paths_t = std::vector< path_t >;


inline bool is_once( std::string const & s )
{
	return s[ 0 ] > 'Z';
}

inline bool visited( vertex_t const & vertex, path_t const & path )
{
	return std::find( path.begin(), path.end(), vertex ) != path.end();
}


void walk( path_t prefix, vertex_t vertex, paths_t & paths, graph_t const & graph )
{
	if( vertex == "end" )
	{
		prefix.push_back( vertex );
		paths.push_back( prefix );

		return;
	}

	if( is_once( vertex ) && visited( vertex, prefix ) )
		return;

	prefix.push_back( vertex );

	for( vertex_t const & v : graph.at( vertex ) )
		if( v != "start" )
			walk( prefix, v, paths, graph );
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	graph_t graph;

	while( !ifs.eof() )
	{
		std::string s;
		ifs >> s;

		size_t p( s.find( '-' ) );

		std::string const a( s.begin(), s.begin() + p );
		std::string const b( s.begin() + p + 1, s.end() );

		graph[ a ].insert( b );
		graph[ b ].insert( a );
	}

	paths_t paths;
	walk( path_t(), "start", paths, graph );

	std::cout << "result = " << paths.size() << std::endl;

	return 0;
}