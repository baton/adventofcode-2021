#include <iostream>
#include <fstream>
#include <string>
#include <map>


uint64_t constexpr steps{ 40 };


inline void update_count( std::map< std::string, uint64_t > & pp, std::string const & s, uint64_t const c )
{
	auto i( pp.find( s ) );
	if( i != pp.end() )
		i->second += c;
	else
		pp.emplace( s, c );
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::string tpl;
	ifs >> tpl;

	std::map< std::string, std::pair< std::string, std::string > > rules;

	while( !ifs.eof() )
	{
		std::string p, t, c;
		ifs >> p >> t >> c;

		std::string a( p ), b( p );
		a[ 0 ] = c[ 0 ];
		b[ 1 ] = c[ 0 ];

		rules.emplace( p, std::make_pair( a, b ) );
	}

	std::map< std::string, uint64_t > pairs;
	for( size_t i( 0 ); i != tpl.length() - 1; ++i )
		++pairs[ tpl.substr( i, 2 ) ];

	for( size_t step( 0 ); step != steps; ++step )
	{
		std::map< std::string, uint64_t > pp;
		for( auto const & [ s, c ] : pairs )
		{
			auto i( rules.find( s ) );
			if( i != rules.end() )
			{
				update_count( pp, i->second.first, c );
				update_count( pp, i->second.second, c );
			}
			else
				pp[ s ] += c;
		}

		pairs = std::move( pp );
	}

	std::map< char, uint64_t > counts;
	for( auto const & [ k, v ] : pairs )
		counts[ k[ 0 ] ] += v;
	++counts[ tpl.back() ];

	uint64_t mn( std::numeric_limits< uint64_t >::max() ), mx( 0 );
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