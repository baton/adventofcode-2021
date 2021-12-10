#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	uint64_t sum( 0 );

	while( !ifs.eof() )
	{
		std::string s;
		ifs >> s;

		std::vector< char > v;
		for( char const c : s )
		{
			bool broken( false );
			switch( c )
			{
				case '(':
					v.push_back( ')' ); break;
				case '[':
					v.push_back( ']' ); break;
				case '{':
					v.push_back( '}' ); break;
				case '<':
					v.push_back( '>' ); break;

				default:
					if( c != v.back() )
						broken = true;
					else
						v.pop_back();
					break;
			}

			if( broken )
			{
				switch( c )
				{
					case ')': sum += 3; break;
					case ']': sum += 57; break;
					case '}': sum += 1197; break;
					case '>': sum += 25137; break;
					default: break;
				}
				break;
			}
		}
	}

	std::cout << "result = " << sum << std::endl;

	return 0;
}