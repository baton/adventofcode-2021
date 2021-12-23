#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <array>
#include <memory>


size_t constexpr steps = 50;


using map_t = std::set< std::pair< int64_t, int64_t > >;
using alg_t = std::array< bool, 512 >;


inline uint64_t get( int64_t const x, int64_t const y, map_t const & m,
	int64_t const x0, int64_t const y0, int64_t const x1, int64_t const y1,
	alg_t const & a, bool const odd ) noexcept
{
	if( ( x >= x0 ) && ( x <= x1 ) && ( y >= y0 ) && ( y <= y1 ) )
		return m.count( { x, y } );
	else if( a[ 0 ] )
		return odd ? 1 : 0;
	else
		return 0;
}

inline uint64_t const index(
	int64_t const x, int64_t const y, map_t const & m,
	int64_t const x0, int64_t const y0, int64_t const x1, int64_t const y1,
	alg_t const & a, bool const odd ) noexcept
{
	uint64_t r( 0 );
	for( int64_t yy( y - 1  ); yy != y + 2; ++yy )
		for( int64_t xx( x - 1 ); xx != x + 2; ++xx )
			r = ( r << 1 ) | get( xx, yy, m, x0, y0, x1, y1, a, odd ) ;
	return r;
}

int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	alg_t a;
	map_t m;

	std::ifstream ifs( argv[ 1 ] );

	{
		std::string s;
		ifs >> s;
		for( size_t i( 0 ); i != s.length(); ++i )
			a[ i ] = ( s[ i ] == '#' );
	}

	int64_t min_x( 0 ), max_x( 0 );
	int64_t min_y( 0 ), max_y( 0 );
	while( !ifs.eof() )
	{
		std::string s;
		ifs >> s;
		for( size_t i( 0 ); i != s.length(); ++i )
			if( s[ i ] == '#' )
				m.emplace( i, max_y );

		max_x = s.length();
		++max_y;
	}

	for( size_t step( 0 ); step != steps; ++step )
	{
		map_t mm;
		int64_t mix( max_x ), mxx( min_x ), miy( max_y ), mxy( min_y );

		for( int64_t x( min_x - 1 ); x != max_x + 2; ++x )
			for( int64_t y( min_y - 1 ); y != max_y + 2; ++y )
			{
				size_t const i( index( x, y, m, min_x, min_y, max_x, max_y, a, step % 2 != 0 ) );
				if( a[ i ] )
				{
					mm.emplace( x, y );

					if( x < mix )
						mix = x;
					if( mxx < x )
						mxx = x;
					if( y < miy )
						miy = y;
					if( mxy < y )
						mxy = y;
				}
			}

		m = std::move( mm );

		min_x = mix;
		max_x = mxx;
		min_y = miy;
		max_y = mxy;
	}

	uint64_t const result( m.size() );

	std::cout << "result = " << result << std::endl;
	
	return 0;
}