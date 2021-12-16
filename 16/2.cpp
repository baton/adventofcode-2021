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


uint64_t parse_packet( bv_t const & data, size_t & offset )
{
	uint64_t const version( read_bits( data, offset, 3 ) );
	uint64_t const type_id( read_bits( data, offset, 3 ) );

	if( type_id == 4 )
	{
		uint64_t result( 0 );

		uint64_t fin( 0 );
		do
		{
			fin = read_bits( data, offset, 1 );
			result = ( result << 4 ) | read_bits( data, offset, 4 );
		} while( fin );

		return result;
	}
	else
	{
		std::vector< uint64_t > v;

		uint64_t const length_type_id( read_bits( data, offset, 1 ) );
		if( length_type_id == 1 )
		{
			uint64_t count( read_bits( data, offset, 11 ) );
			while( count-- )
				v.push_back( parse_packet( data, offset ) );
		}
		else
		{
			uint64_t const length( read_bits( data, offset, 15 ) );
			uint64_t const e( offset + length );
			while( offset != e )
				v.push_back( parse_packet( data, offset ) );
		}

		switch( type_id )
		{
			case 0:
				return std::accumulate( v.begin(), v.end(), 0ULL );
			case 1:
				return std::accumulate( v.begin(), v.end(), 1ULL, std::multiplies< uint64_t >() );
			case 2:
				return *std::min_element( v.begin(), v.end() );
			case 3:
				return *std::max_element( v.begin(), v.end() );
			case 5:
				return v[ 0 ] > v[ 1 ] ? 1 : 0;
			case 6:
				return v[ 0 ] < v[ 1 ] ? 1 : 0;
			case 7:
				return v[ 0 ] == v[ 1 ] ? 1 : 0;
			default:
				return 0;
		}
	}
}

inline uint64_t parse_data( bv_t const & data )
{
	size_t offset( 0 );
	return parse_packet( data, offset );
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

	uint64_t const result( parse_data( data ) );

	std::cout << "result = " << result << std::endl;

	return 0;
}