#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <memory>
#include <algorithm>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace qi = boost::spirit::qi;


struct point_t
{
	int x, y, z;

	point_t( int const xx = 0, int const yy = 0, int const zz = 0 )
		: x( xx )
		, y( yy )
		, z( zz )
	{}

	inline bool operator<( point_t const & rhs ) const noexcept
	{
		if( x < rhs.x )
			return true;
		if( rhs.x < x )
			return false;
		if( y < rhs.y )
			return true;
		if( rhs.y < y )
			return false;
		if( z < rhs.z )
			return true;
		if( rhs.z < z )
			return false;
		return false;
	}

	inline bool operator==( point_t const & rhs ) const noexcept
	{
		return ( x == rhs.x ) && ( y == rhs.y ) && ( z == rhs.z );
	}
};

inline point_t operator-( point_t const & a, point_t const & b )
{
	point_t r( a );
	r.x -= b.x;
	r.y -= b.y;
	r.z -= b.z;
	return r;
}

struct scanner_t
{
	size_t const index;
	std::set< point_t > beacons;
	bool matched = false;

	scanner_t( size_t const i )
		: index( i )
	{}
};

BOOST_FUSION_ADAPT_STRUCT(
	point_t,
	( int, x )
	( int, y )
	( int, z )
)

inline void normalize( std::list< point_t > & l ) noexcept
{
	point_t a( l.front() );
	for( point_t & p : l )
	{
		p.x -= a.x;
		p.y -= a.y;
		p.z -= a.z;
	}
}

inline std::set< point_t > rotate_x( std::set< point_t > const & s ) noexcept
{
	std::set< point_t > r;
	for( point_t const & p : s )
		r.emplace( p.x, p.z, -p.y );
	return r;
}

inline std::set< point_t > rotate_y( std::set< point_t > const & s ) noexcept
{
	std::set< point_t > r;
	for( point_t const & p : s )
		r.emplace( -p.z, p.y, p.x );
	return r;
}

inline std::set< point_t > rotate_z( std::set< point_t > const & s ) noexcept
{
	std::set< point_t > r;
	for( point_t const & p : s )
		r.emplace( -p.y, p.x, p.z );
	return r;
}

bool match_tail( std::list< point_t > a, std::list< point_t > b, size_t needed )
{
	if( a.empty() || b.empty() )
		return needed == 0;
	if( ( a.size() < needed ) || ( b.size() < needed ) )
		return false;
	if( needed == 0 )
		return true;

	normalize( a );
	normalize( b );

	std::list< point_t >::const_iterator ai( std::next( a.cbegin() ) ), bi( std::next( b.cbegin() ) );
	while( ( ai != a.cend() ) && ( bi != b.cend() ) )
	{
		if( *ai == *bi )
		{
			if( --needed == 0 )
				return true;
			else
			{
				++ai;
				++bi;
			}
		}

		while( ( ai != a.cend() ) && ( *ai < *bi ) )
			++ai;
		if( ai == a.cend() )
			return false;
		while( ( bi != b.cend() ) && ( *bi < *ai ) )
			++bi;
		if( bi == b.cend() )
			return false;
	}

	return false;
}

bool match( std::list< point_t > a, std::list< point_t > b, point_t & pa, point_t & pb, size_t const needed = 12 )
{
	while( a.size() >= needed )
	{
		std::list< point_t > bb( b );
		while( bb.size() >= needed )
		{
			if( match_tail( a, bb, needed - 1 ) )
			{
				pa = a.front();
				pb = bb.front();
				return true;
			}
			bb.pop_front();
		}
		a.pop_front();
	}
	return false;
}

inline bool match( std::list< point_t > const & a, std::set< point_t > const & b, point_t & pa, point_t & pb )
{
	return match( a, std::list< point_t >( b.begin(), b.end() ), pa, pb );
}

std::set< point_t > match( std::set< point_t > const & a, std::set< point_t > const & b, point_t & pa, point_t & pb )
{
	std::list< point_t > orig( a.begin(), a.end() );
	std::set< std::set< point_t > > vars;

	std::set< point_t > brx( b );
	for( size_t rx( 0 ); rx != 4; ++rx )
	{
		std::set< point_t > bry( brx );
		for( size_t ry( 0 ); ry != 4; ++ry )
		{
			vars.insert( bry );
			if( match( orig, bry, pa, pb ) )
				return bry;
			bry = rotate_y( bry );
		}
		brx = rotate_x( brx );
	}
	for( size_t rz( 0 ); rz != 2; ++rz )
	{
		brx = rotate_z( brx );
		std::set< point_t > bry( brx );
		for( size_t ry( 0 ); ry != 4; ++ry )
		{
			vars.insert( bry );
			if( match( orig, bry, pa, pb ) )
				return bry;
			bry = rotate_y( bry );
		}
		brx = rotate_z( brx );
	}

	return {};
}

int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::set< point_t > points;
	std::list< scanner_t > scanners, mscanners;

	std::ifstream ifs( argv[ 1 ] );
	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );

		if( s.empty() )
			continue;
		if( s.substr( 0, 12 ) == "--- scanner " )
		{
			scanners.emplace_back( scanners.size() );
			continue;
		}
		point_t p;
		if( !qi::phrase_parse( s.cbegin(), s.cend(), qi::int_ >> ',' >> qi::int_ >> ',' >> qi::int_, qi::space, p ) )
			return 0;
		scanners.back().beacons.insert( p );
	}

	mscanners.push_back( scanners.front() );
	points = scanners.front().beacons;
	scanners.pop_front();

	while( !scanners.empty() )
	{
		for( std::list< scanner_t >::const_iterator i( scanners.begin() ); i != scanners.end(); )
		{
			bool found( false );
			for( scanner_t const & ms : mscanners )
			{
				point_t pa, pb;
				std::set< point_t > mm( match( ms.beacons, i->beacons, pa, pb ) );
				if( !mm.empty() )
				{
					point_t const d( pa - pb );
					std::set< point_t > m2;
					for( point_t const & p : mm )
					{
						point_t pp( p.x + d.x, p.y + d.y, p.z + d.z );
						m2.insert( pp );
						points.insert( pp );
					}
					mscanners.push_back( *i );
					mscanners.back().beacons = m2;
					found = true;
					break;
				}
			}
			if( found )
				i = scanners.erase( i );
			else
				++i;
		}
	}

	uint64_t const result( points.size() );

	std::cout << "result = " << result << std::endl;

	return 0;
}