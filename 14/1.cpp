#include <iostream>
#include <fstream>
#include <string>
#include <map>


uint64_t constexpr steps{ 10 };


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::string tpl;
	ifs >> tpl;

	std::map< std::string, char > rules;

	while( !ifs.eof() )
	{
		std::string p, t, c;
		ifs >> p >> t >> c;

		rules.emplace( p, c[ 0 ] );
	}

	for( uint64_t step( 0 ); step != steps; ++step )
	{
		std::string t;
		t.reserve( tpl.size() * 2 );
		for( size_t i( 0 ); i != tpl.length() - 1; ++i )
		{
			std::string const s( tpl.substr( i, 2 ) );
			t.push_back( s[ 0 ] );
			t.push_back( rules[ s ] );
		}
		t.push_back( tpl.back() );

		tpl = std::move( t );
	}

	std::map< char, uint64_t > counts;
	for( char const & c : tpl )
		++counts[ c ];

	uint64_t mn( tpl.size() ), mx( 0 );
	for( auto const & [ k, v ] : counts )
	{
		if( v < mn )
			mn = v;
		if( mx < v )
			mx = v;
	}

	std::cout << "result = " << ( mx - mn ) << std::endl;

	return 0;
}