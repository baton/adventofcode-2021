#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


inline uint64_t score( char const c )
{
	switch( c )
	{
		case ')': return 1;
		case ']': return 2;
		case '}': return 3;
		case '>': return 4;
		default: return 0;
	}
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< uint64_t > scores;

	while( !ifs.eof() )
	{
		std::string s;
		ifs >> s;

		std::vector< char > v;
		bool broken( false );
		for( char const c : s )
		{
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
				break;
		}

		if( !broken && !v.empty() )
		{
			scores.push_back( std::accumulate( v.crbegin(), v.crend(),
				0ULL,
				[]( uint64_t const v, char const c ) -> uint64_t
				{
					return v * 5 + score( c );
				}
			) );
		}
	}

	std::sort( scores.begin(), scores.end() );

	std::cout << "result = " << scores[ scores.size() / 2 ] << std::endl;

	return 0;
}