#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>


using bit_t = unsigned char;
using bv_t = std::vector< bit_t >;


inline uint64_t read_bits( bv_t const & data, size_t & offset, size_t const length )
{
	uint64_t const x( std::accumulate(
		data.begin() + offset, data.begin() + offset + length,
		0ULL,
		[]( uint64_t const x, unsigned char const v ) -> uint64_t
		{
			return ( x << 1 ) | v;
		}
	) );

	offset += length;

	return x;
}


size_t parse_packet( bv_t const & data, size_t const offset, std::function< void( uint64_t const ) > f )
{
	size_t o( offset );

	uint64_t const version( read_bits( data, o, 3 ) );
	uint64_t const type_id( read_bits( data, o, 3 ) );

	if( type_id == 4 )
	{
		uint64_t fin( 0 );
		do
		{
			fin = read_bits( data, o, 1 );
			o += 4;
		} while( fin );
	}
	else
	{
		uint64_t const length_type_id( read_bits( data, o, 1 ) );
		if( length_type_id == 1 )
		{
			uint64_t count( read_bits( data, o, 11 ) );
			while( count-- )
				o = parse_packet( data, o, f );
		}
		else
		{
			uint64_t const length( read_bits( data, o, 15 ) );
			uint64_t const e( o + length );
			while( o != e )
				o = parse_packet( data, o, f );
		}
	}

	f( version );

	return o;
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	bv_t data;

	std::ifstream ifs( argv[ 1 ] );
	std::string s;
	ifs >> s;
	for( char const c : s )
	{
		unsigned char const x( c <= '9' ? c - '0' : c - 'A' + 0xA );
		for( size_t i( 0 ); i != 4; ++i )
			data.push_back( x & ( 1 << ( 3 - i )  ) ? 1 : 0 );
	}

	uint64_t sum_version( 0 );
	parse_packet(
		data, 0,
		[ &sum_version ]( uint64_t const v )
		{
			sum_version += v;
		}
	);

	std::cout << "result = " << sum_version << std::endl;

	return 0;
}