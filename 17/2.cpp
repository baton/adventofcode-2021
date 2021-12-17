#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <memory>

#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::vector< int64_t > target;

	std::ifstream ifs( argv[ 1 ] );
	std::string s;
	std::getline( ifs, s );
	if( !qi::phrase_parse(
		s.begin(), s.end(),
		"target area: x=" >> qi::int_ >> ".." >> qi::int_ >> ", y=" >> qi::int_ >> ".." >> qi::int_, qi::space,
		target ) )
		return 0;

	std::set< std::pair< int64_t, int64_t > > velocities;

	for( int64_t vy0( target[ 2 ] ); vy0 != -target[ 2 ]; ++vy0 )
		for( int64_t vx0( 1 ); vx0 != target[ 1 ] + 1; ++vx0 )
		{
			int64_t x( 0 ), y( 0 ), vx( vx0 ), vy( vy0 );
			while( ( x <= target[ 1 ] ) && ( y >= target[ 2 ] ) )
			{
				x += vx;
				y += vy;

				if( ( x >= target[ 0 ] ) && ( x <= target[ 1 ] )
					&& ( y >= target[ 2 ] ) && ( y <= target[ 3 ] ) )
				{
					velocities.emplace( vx0, vy0 );
					break;
				}

				if( vx != 0 )
					--vx;
				--vy;
			}
		}

	std::cout << "result = " << velocities.size() << std::endl;
	
	return 0;
}