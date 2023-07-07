#include <iostream>
#include <boost/iostreams/device/mapped_file.hpp>
#include <algorithm>
#include <iterator>

namespace io = boost::iostreams;

constexpr auto input_path = "data/day01.txt";

template <class InputType>
int solve(InputType contents)
{
    int result = 0;
    auto it1 = contents.begin();
    auto it2 = std::next( it1 );
    for ( ; it2 != contents.end(); std::advance( it1, 1 ), std::advance( it2, 1 ) )
    {
	if ( *it1 == *it2 ) {
	    result += static_cast<decltype( result )>( *it1 - '0' );
	}
    }
    std::advance( it1, -1 );
    if ( *it1 == *contents.begin() ) {
	result += static_cast<decltype( result )>( *it1 - '0' );
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
	std::cout << solve( input_file );
	input_file.close();
    } else {
	std::cerr << "Failed to open file.";
	return 1;
    }
    return 0;
}
