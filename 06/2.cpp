#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

uint64_t constexpr days( 256 );


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::array< uint64_t, 9 > timers{ 0 };

	{
		std::vector< uint64_t > fishes;
		std::string sinp;
		ifs >> sinp;
		if( !qi::phrase_parse( sinp.cbegin(), sinp.cend(), qi::int_ % ',', qi::space, fishes ) )
			return 0;

		for( uint64_t const & fish : fishes )
			++timers[ fish ];
	}

	for( size_t day( 0 ); day != days; ++day )
	{
		uint64_t const x( timers[ 0 ] );

		for( size_t i( 0 ); i != timers.size() - 1; ++i )
			timers[ i ] = timers[ i + 1 ];

		timers[ 6 ] += x;
		timers[ 8 ] = x;
	}

	uint64_t const result( std::accumulate( timers.begin(), timers.end(), 0ULL ) );

	std::cout << "result = " << result << std::endl;

	return 0;
}