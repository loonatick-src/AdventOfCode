#include <algorithm>
#include <iterator>
#include <iostream>
#include <sysexits.h>

#include "utilities.hpp"

void split_mut( std::vector<std::string_view>& tokens, const std::string_view& str, const char delim )
{
    if ( str.size() == 0 ) {
	return;
    }
    const auto is_delim = [&]( std::string_view::value_type n ) { return n == delim; };
    auto first = std::find_if_not( str.begin(), str.end(), is_delim );
    while ( first != str.end() )
	{
	    auto last = std::find( first, str.end(), delim );

	    tokens.push_back( std::string_view( first, static_cast<std::string_view::size_type>( last - first ) ) );
	    first = std::find_if_not( last, str.end(), is_delim );
	}
}

std::vector<std::string_view> split( const std::string_view& str, const char delim )
{
    std::vector<std::string_view> tokens;
    split_mut( tokens, str, delim );
    return tokens;
}

int report_errors( std::errc ec ) {
    if ( ec == std::errc::invalid_argument ) {
	std::cerr << "That isn't a number.\n";
	return EX_DATAERR;
    }
    else if ( ec == std::errc::result_out_of_range ) {
	std::cerr << "This number is larger than an int.\n";
	return EX_DATAERR;
    }
    return 0;
}
