#include <iostream>
#include <boost/iostreams/device/mapped_file.hpp>
#include <algorithm>
#include <iterator>
#include <string_view>

namespace io = boost::iostreams;

constexpr auto input_path = "data/day01.txt";

template <class InputType, class ItAdvance>
int solve( const InputType& contents, const ItAdvance& adv )
{
    int result = 0;
    auto it1 = contents.begin();
    for ( ; it1 != contents.end(); std::advance( it1, 1 ) )
    {
	auto it2 = adv( it1 );
	if ( *it1 == *it2 ) {
	    result += static_cast<decltype( result )>( *it1 - '0' );
	}
    }
    return result;
}

int main()
{
    io::mapped_file input_file;
    input_file.open( input_path );
    // TODO: this handling is probably not the most idiomatic
    if ( input_file.is_open() )
    {
	auto input_view = std::string_view( input_file.data(), input_file.size() );
	input_view.remove_suffix( std::min( input_view.size() - input_view.find_last_not_of(" \t\r\v\n")-1, input_view.size() ) );
	const auto step = input_view.size() / 2;
	auto adv1 = [&]( decltype( input_view.begin() ) it )
	{
	    auto rv = std::next( it );
	    if ( rv == input_view.end() ) {
		rv = input_view.begin();
	    }
	    return rv;
	};
	std::cout << solve( input_view, adv1 ) << std::endl;
	auto adv2 = [&, step]( decltype( input_view.begin() ) it )
	{
	    auto rv = std::next( it, step );
	    if ( rv < input_view.end() )
	    {
		return rv;
	    }
	    auto overstep = rv - input_view.end();
	    return std::next( input_view.begin(), overstep );
	};
	std::cout << solve( input_view, adv2 );
	input_file.close();
    } else
    {
	std::cerr << "Failed to open file.";
	return 1;
    }
    return 0;
}
