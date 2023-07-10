#include <vector>
#include <charconv>
#include <string_view>
#include <algorithm>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <iterator>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/container_hash/hash.hpp>

#include "utilities.hpp"

constexpr auto input_path = "data/day06.txt";
// constexpr auto input_path = "data/test_input.txt";

namespace io = boost::iostreams;

template <class S, class V>
bool contains_or_insert( S& haystack, V& needle )
{
    auto it = haystack.find( needle );
    if ( it == haystack.end() ) {
	haystack.insert( it, needle );
	return false;
    } else {
	return true;
    }
}

template <class V>
struct Hash {
    auto operator()( const V& v ) const {
        return boost::hash_range( v.begin(), v.end() );
    }
};

template <class V>
int realloc_banks( V& banks )
{
    std::unordered_set<V, Hash<V>> reference_banks;
    reference_banks.insert( banks );
    const auto num_banks = banks.size();
    int count = 0;
    do {
	auto it = std::max_element( banks.begin(), banks.end() );
	auto num_blocks = *it;
	*it = 0;
	auto blocks_per_bank = num_blocks / num_banks;
	auto rem_blocks = num_blocks % num_banks;
	if ( blocks_per_bank != 0 ) {
	    std::transform( banks.begin(), banks.end(), banks.begin(),
			    [=]( typename V::value_type v ) {
				return v + blocks_per_bank;
			    } );
	}
	// a ring buffer would have been nice
	std::advance( it, 1 );
	if ( it == banks.end() ) it = banks.begin();
	auto rem_banks = std::min( static_cast<decltype(rem_blocks)>(banks.end() - it), rem_blocks );
	const auto incl = []( typename V::value_type v ) { return v+1; };
	auto e1 = std::min( std::next( it, rem_banks ), banks.end() );
	std::transform( it, e1,  it, incl );
	rem_blocks -= (e1 - it);
	if ( rem_blocks > 0 ) {
	    std::transform( banks.begin(), std::next( banks.begin(), rem_blocks ), banks.begin(), incl );
	}
	count++;
    } while ( !contains_or_insert( reference_banks, banks ) );
    return count;
}

int main( void )
{
    io::mapped_file input_file;
    input_file.open( input_path );
    std::string_view v( input_file.data(),  input_file.size() );
    trim_right( v, " \t\r\v\n");
    std::vector<std::string_view> tokens = split( v, '\t' );
    std::vector<int> banks( tokens.size() );
    std::transform( tokens.cbegin(), tokens.cend(), banks.begin(), []( const std::string_view& tok ) {
	int val;
	std::from_chars( tok.begin(), tok.end(), val );
	return val;
    } );

    std::cout << realloc_banks( banks ) << '\n';
    // just simulate again lol
    std::cout << realloc_banks( banks );
    
}
