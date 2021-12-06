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
	int64_t x1, y1, x2, y2;
};
BOOST_FUSION_ADAPT_STRUCT(
	line_t,
	( int64_t, x1 )
	( int64_t, y1 )
	( int64_t, x2 )
	( int64_t, y2 )
)

int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::vector< line_t > lines;
	int64_t max_x{ 0 }, max_y{ 0 };

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
		int64_t dx( line.x2 - line.x1 );
		if( dx != 0 )
			dx /= std::abs( dx );
		int64_t dy( line.y2 - line.y1 );
		if( dy != 0 )
			dy /= std::abs( dy );
		int64_t x( line.x1 ), y( line.y1 );

		++field[ y * max_x + x ];
		do
		{
			x += dx;
			y += dy;
			++field[ y * max_x + x ];
		} while( x != line.x2 || y != line.y2 );
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