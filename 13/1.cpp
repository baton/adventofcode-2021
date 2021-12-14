#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <memory>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace qi = boost::spirit::qi;


using dot_t = std::pair< uint64_t, uint64_t >;
using dots_t = std::set< dot_t >;


inline uint64_t fold_at( uint64_t const c, uint64_t const f )
{
	return ( c < f ) ? c : f - ( c - f );
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	dots_t dots;
	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );

		if( s.empty() )
			break;

		dot_t dot;
		if( !qi::phrase_parse( s.begin(), s.end(), qi::int_ >> ',' >> qi::int_, qi::space, dot ) )
			return 0;

		dots.insert( std::move( dot ) );
	}

	if( !ifs.eof() )
	{
		std::string t1, t2, s;
		ifs >> t1 >> t2 >> s;

		std::pair< char, uint64_t > p;
		if( !qi::phrase_parse( s.begin(), s.end(), qi::char_ >> '=' >> qi::int_, qi::space, p ) )
			return 0;
		bool const fold_x( p.first == 'x' );
		uint64_t const v( p.second );

		dots_t folded;
		
		for( dot_t const & d : dots )
		{
			if( fold_x )
				folded.emplace( fold_at( d.first, v ), d.second );
			else
				folded.emplace( d.first, fold_at( d.second, v ) );
		}

		dots = std::move( folded );
	}

	std::cout << "result = " << dots.size() << std::endl;

	return 0;
}