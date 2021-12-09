#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


using field_t = std::vector< std::string >;


inline bool is_lowest( size_t const y, size_t const x, field_t const & field )
{
	char const v( field[ y ][ x ] );

	bool result( true );

	if( x != 0 )
		result &= v < field[ y ][ x - 1 ];
	if( y != 0 )
		result &= v < field[ y - 1 ][ x ];
	if( x != field[ y ].size() - 1 )
		result &= v < field[ y ][ x + 1 ];
	if( y != field.size() - 1 )
		result &= v < field[ y + 1 ][ x ];

	return result;
}

void fill_basin( size_t const y, size_t const x, field_t & field )
{
	field[ y ][ x ] = '.';
	if( ( x != 0 ) && ( field[ y ][ x - 1 ] != '9' ) && ( field[ y ][ x - 1 ] != '.' ) )
		fill_basin( y, x - 1, field );
	if( ( y != 0 ) && ( field[ y - 1 ][ x ] != '9' ) && ( field[ y - 1 ][ x ] != '.' ) )
		fill_basin( y - 1, x, field );
	if( ( x != field[ y ].size() - 1 ) && ( field[ y ][ x + 1 ] != '9' ) && ( field[ y ][ x + 1 ] != '.' ) )
		fill_basin( y, x + 1, field );
	if( ( y != field.size() - 1 ) && ( field[ y + 1 ][ x ] != '9' ) && ( field[ y + 1 ][ x ] != '.' ) )
		fill_basin( y + 1, x, field );
}

uint64_t get_basin_size( size_t const y, size_t const x, field_t const & field )
{
	field_t f( field );
	fill_basin( y, x, f );

	return std::transform_reduce( f.begin(), f.end(),
		0ULL,
		std::plus(),
		[]( std::string const & s ) -> uint64_t
		{
			return std::count( s.begin(), s.end(), '.' );
		}
	);
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

		field.push_back( std::move( s ) );
	}

	std::vector< uint64_t > sizes;
	for( size_t y( 0 ); y != field.size(); ++y )
		for( size_t x( 0 ); x != field[ y ].size(); ++x )
			if( is_lowest( y, x, field ) )
				sizes.push_back( get_basin_size( y, x, field ) );

	std::sort( sizes.begin(), sizes.end(), std::greater() );

	std::cout << "result = " << ( sizes[ 0 ] * sizes[ 1 ] * sizes[ 2 ] ) << std::endl;

	return 0;
}