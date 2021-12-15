#include <iostream>
#include <fstream>
#include <iterator>
#include <memory>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>


using field_t = std::vector< std::vector< uint64_t > >;
using point_t = std::pair< uint64_t, uint64_t >;
using visited_t = std::map< point_t, uint64_t >;
using front_t = std::set< point_t >;


inline bool in_visited( point_t const & p, visited_t const & visited )
{
	return visited.find( p ) != visited.end();
}

inline visited_t nexts( uint64_t const x, uint64_t const y, field_t const & field )
{
	visited_t n;

	if( x != 0 )
		n.emplace( std::make_pair( x - 1, y ), field[ x - 1 ][ y ] );
	if( y != 0 )
		n.emplace( std::make_pair( x, y - 1 ), field[ x ][ y - 1 ] );
	if( x != field.size() - 1 )
		n.emplace( std::make_pair( x + 1, y ), field[ x + 1 ][ y ] );
	if( y != field[ 0 ].size() - 1 )
		n.emplace( std::make_pair( x, y + 1 ), field[ x ][ y + 1 ] );

	return n;
}

inline visited_t nexts( point_t const & p, field_t const & field )
{
	return nexts( p.first, p.second, field );
}

inline uint64_t incl( uint64_t x )
{
	++x;
	if( x == 10 )
		x = 1;
	return x;
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	field_t field;

	std::ifstream ifs( argv[ 1 ] );
	for( std::string s; std::getline( ifs, s ); )
	{
		std::vector< uint64_t > v;
		std::transform(
			s.begin(), s.end(),
			std::back_inserter< std::vector< uint64_t > >( v ),
			[]( char const c ) -> uint64_t
			{
				return c - '0';
			}
		);
		size_t const sz( v.size() );
		for( size_t i( 1 ); i != 5; ++i )
			for( size_t j( 0 ); j != sz; ++j )
				v.push_back( incl( v[ sz * ( i - 1 ) + j ] ) );

		field.push_back( std::move( v ) );
	}

	size_t const sz( field.size() );
	for( size_t i( 1 ); i != 5; ++i )
		for( size_t j( 0 ); j != sz; ++j )
		{
			std::vector v( field[ sz * ( i - 1 ) + j ] );
			std::for_each( v.begin(), v.end(), []( uint64_t & x ) { x = incl( x ); } );
			field.push_back( std::move( v ) );
		}

	point_t const target{ field.size() - 1, field[ 0 ].size() - 1 };

	visited_t weights;
	weights.emplace( std::make_pair( 0, 0 ), 0 );

	front_t front;
	front.emplace( std::make_pair( 0, 0 ) );

	while( !front.empty() )
	{
		point_t const point( front.extract( front.begin() ).value() );
		for( auto const & [ p, w ] : nexts( point, field ) )
		{
			if( !in_visited( p, weights ) || ( weights[ point ] + w < weights[ p ] ) )
			{
				weights[ p ] = weights[ point ] + w;
				front.emplace( p );
			}
		}
	}

	std::cout << "result = " << weights[ target ] << std::endl;

	return 0;
}