#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

size_t constexpr game_size{ 5 };


using value_t = std::pair< uint64_t, bool >;
using line_t = std::array< value_t, game_size >;
using card_t = std::array< line_t, game_size * 2 >;
using cards_t = std::vector< card_t >;


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< uint64_t > game;
	std::string sinp;
	ifs >> sinp;
	if( !qi::phrase_parse( sinp.cbegin(), sinp.cend(), qi::int_ % ',', qi::space, game ) )
	{
		std::cout << "parse error" << std::endl;
		return 0;
	}

	cards_t cards;
	while( !ifs.eof() )
	{
		std::array< uint64_t, game_size * game_size > nums;
		for( uint64_t & n : nums )
			ifs >> n;

		card_t card;
		for( size_t i( 0 ); i != game_size; ++i )
			for( size_t j( 0 ); j != game_size; ++j )
			{
				card[ i ][ j ] = { nums[ i * game_size + j ], false };
				card[ i + 5 ][ j ] = { nums[ j * game_size + i ], false };
			}

		cards.push_back( std::move( card ) );
	}

	cards_t out;
	uint64_t last_number{ 0 };
	for( uint64_t const & number : game )
	{
		for( card_t & card : cards )
		{
			for( line_t & line : card )
			{
				auto i = std::find( line.begin(), line.end(), std::make_pair( number, false ) );
				if( i != line.end() )
					i->second = true;
			}
		}

		auto outed{
			std::remove_if( cards.begin(), cards.end(),
				[]( card_t const & card ) -> bool
				{
					return std::transform_reduce( card.begin(), card.end(),
						false,
						std::logical_or(),
						[]( line_t const & line ) -> bool
						{
							return std::transform_reduce( line.begin(), line.end(),
								true,
								std::logical_and(),
								[]( value_t const & v ) -> bool
								{
									return v.second;
								}
							);
						}
					);
				}
			)
		};
		if( outed != cards.end() )
		{
			out.insert( out.end(), outed, cards.end() );
			cards.erase( outed, cards.end() );
		}

		last_number = number;
		if( cards.empty() )
			break;
	}

	if( !out.empty() )
	{
		card_t const & card( out.back() );

		uint64_t const sum_unmarked{
			std::transform_reduce( card.begin(), card.begin() + game_size,
				0ULL,
				std::plus(),
				[]( line_t const & line ) -> uint64_t
				{
					return std::transform_reduce( line.begin(), line.end(),
						0ULL,
						std::plus(),
						[]( value_t const & v ) -> uint64_t
						{
							return v.second ? 0ULL : v.first;
						}
					);
				}
			)
		};

		std::cout << "result = " << ( last_number * sum_unmarked ) << std::endl;

		return 0;
	}

	return 0;
}