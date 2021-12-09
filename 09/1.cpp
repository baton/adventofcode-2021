#include <iostream>
#include <fstream>
#include <string>
#include <vector>


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

	size_t sum( 0 );
	for( size_t y( 0 ); y != field.size(); ++y )
		for( size_t x( 0 ); x != field[ y ].size(); ++x )
			if( is_lowest( y, x, field ) )
				sum += ( field[ y ][ x ] - '0' ) + 1;

	std::cout << "result = " << sum << std::endl;

	return 0;
}