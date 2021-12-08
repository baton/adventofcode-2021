#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>


using dig_t = uint64_t;


inline dig_t to_bin( std::string const & s )
{
	dig_t x( 0 );
	for( char const c : s )
		x |= 1 << ( c - 'a' );
	return x;
}

inline size_t bits( dig_t d )
{
	size_t r( 0 );
	while( d )
	{
		++r;
		d &= d - 1;
	}
	return r;
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	uint64_t sum( 0 );

	while( !ifs.eof() )
	{
		std::vector< dig_t > xdigits;
		std::array< dig_t, 10 > digits;
		for( size_t i( 0 ); i != 10; ++i )
		{
			std::string s;
			ifs >> s;

			dig_t d( to_bin( s ) );
			switch( s.length() )
			{
				case 2:
					digits[ 1 ] = d;
					break;
				case 3:
					digits[ 7 ] = d;
					break;
				case 4:
					digits[ 4 ] = d;
					break;
				case 7:
					digits[ 8 ] = d;
					break;
				default:
					xdigits.push_back( d );
					break;
			}
		}

		{
			dig_t const x( digits[ 4 ] | digits[ 7 ] );
			for( std::vector< dig_t >::const_iterator i( xdigits.begin() ); i != xdigits.end(); ++i )
				if( bits( x ^ *i ) == 1 )
				{
					digits[ 9 ] = *i;
					xdigits.erase( i );

					break;
				}
		}

		{
			dig_t const x( ( digits[ 8 ] ^ digits[ 7 ] ) | ( digits[ 1 ] ^ digits[ 7 ] ) );
			for( std::vector< dig_t >::const_iterator i( xdigits.begin() ); i != xdigits.end(); ++i )
				if( ( *i & x ) == x )
				{
					digits[ 6 ] = *i;
					xdigits.erase( i );

					break;
				}
		}

		{
			for( std::vector< dig_t >::const_iterator i( xdigits.begin() ); i != xdigits.end(); ++i )
				if( bits( *i ) == 6 )
				{
					digits[ 0 ] = *i;
					xdigits.erase( i );

					break;
				}
		}

		{
			for( std::vector< dig_t >::const_iterator i( xdigits.begin() ); i != xdigits.end(); ++i )
				if( ( *i & digits[ 7 ] ) == digits[ 7 ] )
				{
					digits[ 3 ] = *i;
					xdigits.erase( i );

					break;
				}
		}

		{
			for( std::vector< dig_t >::const_iterator i( xdigits.begin() ); i != xdigits.end(); ++i )
				if( ( *i & digits[ 6 ] ) == *i )
				{
					digits[ 5 ] = *i;
					xdigits.erase( i );

					break;
				}
		}

		digits[ 2 ] = xdigits.front();


		std::string pipe;
		ifs >> pipe;


		uint64_t v( 0 );
		for( size_t i( 0 ); i != 4; ++i )
		{
			std::string s;
			ifs >> s;
			dig_t const d( to_bin( s ) );
			for( size_t i( 0 ); i != 10; ++i )
				if( d == digits[ i ] )
				{
					v = v * 10 + i;
					break;
				}
		}

		sum += v;
	}

	std::cout << "result = " << sum << std::endl;

	return 0;
}