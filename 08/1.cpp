#include <iostream>
#include <fstream>
#include <string>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	uint64_t count( 0 );

	while( !ifs.eof() )
	{
		std::string s;
		for( size_t i( 0 ); i != 11; ++i )
			ifs >> s;

		for( size_t i( 0 ); i != 4; ++i )
		{
			ifs >> s;
			switch( s.length() )
			{
				case 2:
				case 3:
				case 4:
				case 7:
					++count;
				default:
					break;
			}
		}	
	}

	std::cout << "result = " << count << std::endl;

	return 0;
}