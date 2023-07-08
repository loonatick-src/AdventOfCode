#include <iostream>
#include <boost/iostreams/device/mapped_file.hpp>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string_view>
#include <charconv>

namespace io = boost::iostreams;

constexpr auto input_path = "data/day02.txt";

// TODO: move to a utilities file
template <class StringType, typename NeedleType>
void trim_right( StringType &haystack, NeedleType& needle ) {
    auto trim_pos = haystack.size() - haystack.find_last_not_of( needle ) - 1;
    haystack.remove_suffix( std::min( trim_pos,  haystack.size() ) );
    return;
}

// TODO: move to a utilities file
void split_mut( std::vector<std::string_view>& tokens, const std::string_view& str, const char delim = '\t' ) {
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

// TODO: move to a utilities file
std::vector<std::string_view> split( const std::string_view& str, const char delim = '\n' ) {
    std::vector<std::string_view> tokens;
    split_mut( tokens, str, delim );
    return tokens;
}

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
	for ( const auto& tok: tokens )
	{
	    auto rslt = std::from_chars( tok.begin(), tok.end(), current_value );
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
    const auto lines = split(v);
    
    std::cout << calculate_checksum( lines ) << std::endl;
    std::cout << calculate_new_checksum( lines ) << std::endl;
    return 0;
}
