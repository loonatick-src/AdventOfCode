#include <iostream>
#include <boost/iostreams/device/mapped_file.hpp>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string_view>
#include <charconv>

#include <sysexits.h>

#include "utilities.hpp"

namespace io = boost::iostreams;

constexpr auto input_path = "data/day02.txt";

// TODO: move to a utilities file

template <class C>
int calculate_checksum( const C& lines )
{
    std::vector<std::string_view> tokens;
    int checksum = 0;
    for ( const auto& line: lines )
	{
	    auto min_elmt = std::numeric_limits<int>::max();
	    auto max_elmt = std::numeric_limits<int>::min();
	    split_mut( tokens, line, '\t');
	    int current_value;
	    for ( const auto& tok: tokens ) {
		    auto [ptr, ec] = std::from_chars( tok.begin(), tok.end(), current_value );
		    if ( ec == std::errc::invalid_argument ) {
			std::cerr << "That isn't a number.\n";
			return EX_DATAERR;
		    }
		    else if ( ec == std::errc::result_out_of_range ) {
			std::cout << "This number is larger than an int.\n";
			return EX_DATAERR;
		    }
		    // TODO: consider handling errors in rslt?
		    min_elmt = std::min( min_elmt, current_value );
		    max_elmt = std::max( max_elmt, current_value );
		}
	    checksum += max_elmt - min_elmt;
	    min_elmt = std::numeric_limits<int>::max();
	    max_elmt = std::numeric_limits<int>::min();
	    tokens.clear();
	}
    return checksum;
}

template <class C>
int calculate_new_checksum( const C& lines )
{
    int checksum = 0;
    std::vector<int> found;
    std::vector<std::string_view> tokens;
    auto output_it = std::back_inserter( found );
    for ( const auto& line: lines )
	{
	    split_mut( tokens, line, '\t' );
	    std::transform( tokens.cbegin(), tokens.cend(), output_it,
			    [&]( decltype( tokens )::value_type tok ) {
				decltype( found )::value_type curr_val;
				std::from_chars( tok.begin(), tok.end(), curr_val );
				return curr_val;
			    } );
	    for ( size_t i = 0lu; i < found.size(); i++ )
		{
		    auto ni = found[i];
		    for ( auto j = i+1; j < found.size(); j++ )
			{
			    auto nj = found[j];
			    const auto [n1, n2] = std::minmax( ni, nj );
			    if ( n2 % n1 == 0 )
				{
				    checksum += n2 / n1;
				    goto outer;
				}

			}
		}
	outer:
	    tokens.clear();
	    found.clear();
	}
    return checksum;
}


// template <class InputType>
// int solve( InputType& v )
// {
    
// }
int main( void )
{
    io::mapped_file input;
    input.open( input_path );
    if ( !input.is_open() )
	{
	    std::cerr << "Failed to open file!\n";
	    return 1;
	}
    auto v = std::string_view( input.data(), input.size() );
    trim_right( v, " \t\r\v\n" );
    const auto lines = split(v, '\n' );
    
    std::cout << calculate_checksum( lines ) << std::endl;
    std::cout << calculate_new_checksum( lines ) << std::endl;
    return 0;
}
