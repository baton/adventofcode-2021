#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>


using field_t = std::vector< std::vector< uint64_t > >;


inline void fire( size_t const x, size_t const y, field_t & field )
{
	if( field[ x ][ y ] == 11 )
		return;

	if( ++field[ x ][ y ] < 10 )
		return;

	field[ x ][ y ] = 11;

	if( x != 0 )
		fire( x - 1, y, field );
	if( y != 0 )
		fire( x, y - 1, field );
	if( x != field.size() - 1 )
		fire( x + 1, y, field );
	if( y != field[ x ].size() - 1 )
		fire( x, y + 1, field );
	if( ( x != 0 ) && ( y != 0 ) )
		fire( x - 1, y - 1, field );
	if( ( x != field.size() - 1 ) && ( y != 0 ) )
		fire( x + 1, y - 1, field );
	if( ( x != 0 ) && ( y != field[ x ].size() - 1 ) )
		fire( x - 1, y + 1, field );
	if( ( x != field.size() - 1 ) && ( y != field[ x ].size() - 1 ) )
		fire( x + 1, y + 1, field );
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	field_t field;

	while( !ifs.eof() )
	{
		std::string s;
		ifs >> s;

		std::vector< uint64_t > v;
		std::transform( s.begin(), s.end(), std::back_inserter< std::vector< uint64_t > >( v ),
			[]( char const c ) -> uint64_t
			{
				return c - '0';
			}
		);

		field.push_back( std::move( v ) );
	}

	uint64_t steps( 0 );

	for( size_t count( 0 ); count != field.size() * field[ 0 ].size(); )
	{
		++steps;

		for( size_t x( 0 ); x != field.size(); ++x )
			for( size_t y( 0 ); y != field[ x ].size(); ++y )
				++field[ x ][ y ];

		for( size_t x( 0 ); x != field.size(); ++x )
			for( size_t y( 0 ); y != field[ x ].size(); ++y )
				if( field[ x ][ y ] == 10 )
					fire( x, y, field );

		count = 0;
		std::for_each( field.begin(), field.end(),
			[ &count ]( std::vector< uint64_t > & v )
			{
				std::for_each( v.begin(), v.end(),
					[ &count ]( uint64_t & x )
					{
						if( x > 9 )
						{
							++count;
							x = 0;
						}
					}
				);
			}
		);
	}

	std::cout << "result = " << steps << std::endl;

	return 0;
}