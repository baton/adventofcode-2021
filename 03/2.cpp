#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


uint64_t get_by_criteria( std::vector< std::string > values, bool const m, uint64_t const pos = 0 )
{
	uint64_t const count(
		std::count_if( values.begin(), values.end(),
			[ &pos ]( std::string const & sv ) -> bool
			{
				return sv[ pos ] == '1';
			}
		)
	);

	char const c{
		[ &values, &count, &m ]() -> char
		{
			if( count * 2 == values.size() )
				return m ? '1' : '0';
			else if( count > values.size() / 2 )
				return m ? '1' : '0';
			else
				return m ? '0' : '1';
		}()
	};

	std::vector< std::string > v2;
	std::copy_if( values.begin(), values.end(), std::back_inserter< std::vector< std::string > >( v2 ),
		[ &pos, &c ]( std::string const & sv ) -> bool
		{
			return sv[ pos ] == c;
		}
	);

	if( v2.size() == 1 )
	{
		return std::accumulate( v2[ 0 ].begin(), v2[ 0 ].end(),
			0ULL,
			[]( uint64_t const x, char const c ) -> uint64_t
			{
				return x << 1 | ( c == '1' ? 1 : 0 );
			}
		);
	}
	else
		return get_by_criteria( v2, m, pos + 1 );
}

int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< std::string > values;
	while( !ifs.eof() )
	{
		std::string s;
		ifs >> s;
		values.push_back( std::move( s ) );
	}

	uint64_t const orate( get_by_criteria( values, true ) );
	uint64_t const crate( get_by_criteria( values, false ) );

	std::cout << "result = " << ( orate * crate ) << std::endl;

	return 0;
}