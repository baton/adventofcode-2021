#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <memory>


struct num_t
{
	uint64_t value = 0;
	num_t * parent = nullptr;
	std::unique_ptr< num_t > left, right;

	num_t( num_t * p = nullptr, uint64_t const v = 0 )
		: value( v )
		, parent( p )
	{}

	uint64_t * left_r()
	{
		num_t * p( this );
		while( ( p->parent != nullptr ) && ( p->parent->right.get() != p ) )
			p = p->parent;

		if( ( p->parent != nullptr ) && ( p->parent->left.get() != p ) )
		{
			p = p->parent->left.get();
			while( p->right )
				p = p->right.get();

			return &( p->value );
		}

		return nullptr;
	}

	uint64_t * right_r()
	{
		num_t * p( this );
		while( ( p->parent != nullptr ) && ( p->parent->left.get() != p ) )
			p = p->parent;

		if( ( p->parent != nullptr ) && ( p->parent->right.get() != p ) )
		{
			p = p->parent->right.get();
			while( p->left )
				p = p->left.get();

			return &( p->value );
		}

		return nullptr;
	}

	inline bool is_regular() const
	{
		return !left && !right;
	}

	uint64_t magnitude() const
	{
		if( is_regular() )
			return value;
		else
			return 3 * left->magnitude() + 2 * right->magnitude();
	}

	std::string to_string() const
	{
		if( is_regular() )
			return std::to_string( value );

		return std::string( "[" ) + left->to_string() + "," + right->to_string() + "]";
	}
};

std::unique_ptr< num_t > parse( char const * & b, char const * e, num_t * p = nullptr )
{
	std::unique_ptr< num_t > n( std::make_unique< num_t >( p, 0 ) );

	if( *b == '[' )
	{
		++b;
		n->left = parse( b, e, n.get() );
		if( *b == ',' )
			++b;
		if( *b == ' ' )
			++b;
		n->right = parse( b, e, n.get() );
		if( *b == ']' )
			++b;
	}
	else
	{
		n->value = *b - '0';
		++b;
	}

	return n;
}

inline std::unique_ptr< num_t > parse( std::string const & s )
{
	char const * b( s.c_str() );
	char const * e( b + s.length() );

	return parse( b, e );
}

bool explode( std::unique_ptr< num_t > & num, size_t const depth = 0 )
{
	if( depth < 4 )
	{
		if( num->left && explode( num->left, depth + 1 ) )
			return true;
		if( num->right && explode( num->right, depth + 1 ) )
			return true;
	}
	else if( !num->is_regular() )
	{
		uint64_t * l( num->left_r() );
		if( l != nullptr )
			*l += num->left->value;
		uint64_t * r( num->right_r() );
		if( r != nullptr )
			*r += num->right->value;
		num->left.reset();
		num->right.reset();
		num->value = 0;
		return true;
	}

	return false;
}

bool split( std::unique_ptr< num_t > & num )
{
	if( num->is_regular() )
	{
		if( num->value > 9 )
		{
			uint64_t const x( num->value >> 1 );

			num->left = std::make_unique< num_t >( num.get(), x );
			num->right = std::make_unique< num_t >( num.get(), x + ( ( x << 1 ) == num->value ? 0 : 1 ) );

			return true;
		}
		else
			return false;
	}

	return split( num->left ) || split( num->right );
}

void reduce( std::unique_ptr< num_t > & num )
{
	while( explode( num ) || split( num ) )
	{}
}

std::unique_ptr< num_t > operator+( std::unique_ptr< num_t > & a, std::unique_ptr< num_t > & b )
{
	std::unique_ptr< num_t > n( std::make_unique< num_t >() );
	n->left = std::move( a );
	n->left->parent = n.get();
	n->right = std::move( b );
	n->right->parent = n.get();

	reduce( n );

	return n;
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< std::string > nums{ std::istream_iterator< std::string >( ifs ), std::istream_iterator< std::string >() };

	uint64_t max_m( 0 );

	for( size_t i( 0 ); i != nums.size(); ++i )
		for( size_t j( 0 ); j != nums.size(); ++j )
			if( i != j )
			{
				std::unique_ptr< num_t > s( parse( nums[ i ] ) + parse( nums[ j ] ) );

				uint64_t const m( s->magnitude() );
				if( max_m < m )
					max_m = m;
			}

	std::cout << "result = " << max_m << std::endl;
	
	return 0;
}