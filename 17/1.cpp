#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <memory>

#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::vector< int64_t > target;

	std::ifstream ifs( argv[ 1 ] );
	std::string s;
	std::getline( ifs, s );
	if( !qi::phrase_parse(
		s.begin(), s.end(),
		"target area: x=" >> qi::int_ >> ".." >> qi::int_ >> ", y=" >> qi::int_ >> ".." >> qi::int_, qi::space,
		target ) )
		return 0;

	// |y1| * (|y1| - 1) / 2
	uint64_t const result( std::abs( target[ 2 ] ) * ( std::abs( target[ 2 ] ) - 1 ) / 2 );

	std::cout << "result = " << result << std::endl;
	
	return 0;
}