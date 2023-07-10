#include <vector>
#include <charconv>
#include <string_view>
#include <algorithm>
#include <iostream>

#include <boost/iostreams/device/mapped_file.hpp>

#include "utilities.hpp"

constexpr auto input_path = "data/day05.txt";

namespace io = boost::iostreams;

template <class C, class E>
int simulate( C& jumps,  E& execute )
{
    int count = 0;
    const auto n = jumps.size();
    auto it = jumps.begin();
    while ( it < jumps.end() && it >= jumps.begin() ) {
	execute( it );
	count++;
    }
    return count;
}

int main( void ) {
    io::mapped_file input_file;
    input_file.open( input_path );
    std::string_view v( input_file.data(),  input_file.size() );
    trim_right( v, " \t\r\v\n");
    const auto lines = split( v, '\n' );
    std::vector<int> jumps( lines.size() );

    std::transform( lines.cbegin(), lines.cend(), jumps.begin(), []( const std::string_view& tok ) {
	int val;
	std::from_chars( tok.begin(), tok.end(), val );
	return val;
    } );
    decltype( jumps ) jumps_copy( jumps.size() );
    std::copy( jumps.begin(), jumps.end(), jumps_copy.begin() );
    auto exec = []( decltype( jumps.begin() )& it ) {
	auto offset = *it;
	(*it)++;
	std::advance( it, offset );
    };
    std::cout << simulate( jumps, exec ) << '\n';
    auto new_exec = []( decltype( jumps_copy.begin() )& it ) {
	auto offset = *it;
	if ( offset >= 3 ) {
	    (*it)--;
	} else {
	    (*it)++;
	}
	std::advance( it, offset );
    };
    std::cout << simulate( jumps_copy, new_exec );
    return 0;
}
