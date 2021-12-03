#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< uint64_t > counts;
	uint64_t count( 0 );

	while( !ifs.eof() )
	{
		std::string n;
		ifs >> n;

		if( counts.empty() )
			counts.resize( n.length() );

		for( size_t i( 0 ); i != n.length(); ++i )
			counts[ i ] += n[ i ] == '1' ? 1 : 0;

		++count;
	}

	uint64_t gr( 0 ), er( 0 );
	for( size_t i( 0 ); i != counts.size(); ++i )
	{
		uint64_t const b( counts[ i ] > ( count / 2 ) ? 1 : 0 );
		gr = ( gr << 1 ) | b;
		er = ( er << 1 ) | ( 1 - b );
	}

	std::cout << "result = " << ( gr * er ) << std::endl;

	return 0;
}