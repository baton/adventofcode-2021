#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< int64_t > diffs;

	std::adjacent_difference(
		std::istream_iterator< uint64_t >( ifs ), std::istream_iterator< uint64_t >(),
		std::back_inserter< std::vector< int64_t > >( diffs )
	);

	uint64_t const result{
		std::count_if( std::next( diffs.begin() ), diffs.end(),
			[]( int64_t const x ) -> bool
			{
				return x > 0;
			}
		)
	};

	std::cout << "result = " << result << std::endl;

	return 0;
}