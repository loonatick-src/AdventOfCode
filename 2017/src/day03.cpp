#include <cstdint>
#include <cmath>
#include <charconv>
#include <iostream>
#include <type_traits>

#include <boost/iostreams/device/mapped_file.hpp>

namespace io = boost::iostreams;

typedef uint64_t u64;
typedef int64_t i64;

// constexpr auto input_path = "data/day03.txt";
constexpr auto input_path = "data/test_input.txt";

struct Undef {};

constexpr auto undef = Undef();

template <typename T1, typename T2>
T1 clamp( const T1& value, const Undef&, const T2& hi )
{
    return value < hi ? value : static_cast<T1>(hi);
}

template <typename T1, typename T2>
T1 clamp( const T1& value, const T2& lo, const Undef& )
{
    return value > lo ? value : static_cast<T1>(lo);
}

template <typename T1, typename T2, typename T3>
T1 clamp( const T1& value, const T2& lo, const T3& hi )
{
    // TODO: lo > hi should trigger exception or summat
    if ( value < lo )
	return static_cast<T1>(lo);
    else if ( value > hi )
	return static_cast<T1>(hi);
    return value;
}

template <typename T>
T getidx( T lo )
{
    lo -= lo % 1;
    return lo / 2;
}

template <typename T>
T taxicab( const T& input_value )
{
    if ( input_value == 1 ) return 0;
    static_assert( std::is_signed<T>::value );
    auto input_fvalue = static_cast<double>( input_value );
    auto lo = static_cast<i64>( floor( sqrt( input_fvalue ) ) );
    auto hi = lo+1;
    auto losq = lo * lo; auto hisq = hi * hi;
    auto idx = getidx( lo );
    if ( lo % 2 == 0 ) {
	auto i = -idx; auto j = idx;
	auto branch_length = lo + 1;
	auto diff = input_value - losq - 1;
	if ( diff >= 0 ) {
	    auto dj = -clamp( diff, undef, branch_length - 1);
	    j += dj;
	    diff += dj;
	    i += clamp( diff, 0, undef );
	} else i += 1;
	return abs(i) + abs(j);
    } else {
	auto i = idx;
	auto j = -(idx - 1);
	auto diff = input_value - losq - 1;
	if ( diff >= 0 ) {
	    auto branch_length = lo + 1;
	    auto dj = clamp( diff, undef, branch_length - 1);
	    j += dj;
	    diff -= dj;
	    j -= clamp( diff, 0, undef );
	} else i -= 1;
	return abs(i) + abs(j);
    }
}

int main( void )
{
    io::mapped_file input;
    input.open( input_path );
    if ( !input.is_open() ) {
	std::cerr << "Failed to open file!\n";
	return 1;
    }
    auto v = std::string_view( input.data(), input.size() );
    i64 input_value = 0;
    auto rslt = std::from_chars( v.begin(), v.end(), input_value );
    std::cout << taxicab( input_value ) << '\n';

    return 0;
}
