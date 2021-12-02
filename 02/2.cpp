#include <iostream>
#include <fstream>
#include <string>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	uint64_t h{ 0 }, d{ 0 };
	int64_t aim{ 0 };

	while( !ifs.eof() )
	{
		std::string dir;
		uint64_t n{ 0 };
		ifs >> dir >> n;

		switch( dir[ 0 ] )
		{
			case 'f': h += n; d += aim * n; break;
			case 'u': aim -= n; break;
			case 'd': aim += n; break;
			default: break;
		}
	}

	std::cout << "result = " << ( h * d ) << std::endl;

	return 0;
}