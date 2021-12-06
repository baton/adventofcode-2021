#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace qi = boost::spirit::qi;


struct line_t
{
	uint64_t x1, y1, x2, y2;
};
BOOST_FUSION_ADAPT_STRUCT(
	line_t,
	( uint64_t, x1 )
	( uint64_t, y1 )
	( uint64_t, x2 )
	( uint64_t, y2 )
)

int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::vector< line_t > lines;
	uint64_t max_x{ 0 }, max_y{ 0 };

	std::ifstream ifs( argv[ 1 ] );
	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );
		line_t line;
		if( !qi::phrase_parse( s.cbegin(), s.cend(), qi::uint_ >> ',' >> qi::uint_ >> "->" >> qi::uint_ >> ',' >> qi::uint_, qi::space, line ) )
			return 0;

		if( max_x < line.x1 )
			max_x = line.x1;
		if( max_x < line.x2 )
			max_x = line.x2;
		if( max_y < line.y1 )
			max_y = line.y1;
		if( max_y < line.y2 )
			max_y = line.y2;
		lines.push_back( std::move( line ) );
	}
	++max_x;
	++max_y;

	std::vector< uint64_t > field( max_x * max_y, 0ULL );
	for( line_t const & line : lines )
	{
		if( line.x1 == line.x2 )
		{
			uint64_t const x{ line.x1 };
			for( uint64_t y( std::min( line.y1, line.y2 ) ); y != std::max( line.y1, line.y2 ) + 1; ++y )
				++field[ y * max_x + x ];
		}
		else if( line.y1 == line.y2 )
		{
			uint64_t const y{ line.y1 };
			for( uint64_t x( std::min( line.x1, line.x2 ) ); x != std::max( line.x1, line.x2 ) + 1; ++x )
				++field[ y * max_x + x ];
		}
	}

	int64_t const result{
		std::count_if( field.begin(), field.end(),
			[]( uint64_t const & v ) -> bool
			{
				return v > 1;
			}
		)
	};

	std::cout << "result = " << result << std::endl;

	return 0;
}