#include <charconv>
#include <iostream>
#include <type_traits>
#include <set>
#include <algorithm>

#include <boost/iostreams/device/mapped_file.hpp>

#include "utilities.hpp"

namespace io = boost::iostreams;

constexpr auto input_path = "data/day04.txt";

template <class S>
bool is_valid_passphrase( const S& passphrase )
{
    std::vector<std::string_view> words;
    std::set<std::string_view> found;
    split_mut( words, passphrase, ' ' );
    for ( const auto& word: words ) {
	auto pos = found.find( word );
	if ( pos != found.end() ) {
	    return false;
	} else {
	    found.insert( pos, word );
	}
    }
    return true;
}

template <class S>
bool is_valid_passphrase2( const S& passphrase )
{
    std::set<std::string> found;
    std::vector<std::string_view> words;
    split_mut( words, passphrase, ' ' );
    for ( auto& word_view: words ) {
	auto word = std::string( word_view );
	std::sort( word.begin(), word.end() );
	auto pos = found.find( word );
	if ( pos != found.end() ) {
	    return false;
	} else {
	    found.insert( pos, word );
	}
    }
    return true;
}

int main( void ) {
    io::mapped_file input_file;
    input_file.open( input_path );
    if ( !input_file.is_open() ) {
	std::cerr << "Failed to open file!\n";
	return 1;
    }
    auto v = std::string_view( input_file.data(), input_file.size() );
    trim_right( v, " \t\r\v\n" );
    const auto lines = split( v, '\n' );
    int count = 0;
    for ( auto& line: lines ) {
	if ( is_valid_passphrase( line ) )
	    count++;
    }
    std::cout << count << '\n';
    count = 0;
    for ( auto& line: lines ) {
	if ( is_valid_passphrase2( line ) )
	    count++;
    }
    std::cout << count << '\n';
    return 0;
}
