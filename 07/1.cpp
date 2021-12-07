#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< uint64_t > positions;

	{
		std::vector< uint64_t > crabs;
		std::string sinp;
		ifs >> sinp;
		if( !qi::phrase_parse( sinp.cbegin(), sinp.cend(), qi::int_ % ',', qi::space, crabs ) )
			return 0;

		uint64_t const max_crab( *std::max_element( crabs.begin(), crabs.end() ) );
		positions = std::vector< uint64_t >( max_crab + 1, 0 );

		for( uint64_t const & crab : crabs )
			++positions[ crab ];
	}

	std::vector< uint64_t > distances( positions.size(), 0 );
	for( size_t p( 0 ); p != positions.size(); ++p )
	{
		uint64_t w( 0 );
		for( size_t pp( 0 ); pp != positions.size(); ++pp )
			w += positions[ pp ] * std::llabs( pp - p );
		distances[ p ] = w;
	}

	uint64_t const result( *std::min_element( distances.begin(), distances.end() ) );

	std::cout << "result = " << result << std::endl;

	return 0;
}