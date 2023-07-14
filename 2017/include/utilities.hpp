#ifndef AoC_UTILITIES
#define AoC_UTILITIES

#include <vector>
#include <string_view>

template <class StringType, typename NeedleType>
void trim_right( StringType &haystack, NeedleType& needle )
{
    auto trim_pos = haystack.size() - haystack.find_last_not_of( needle ) - 1;
    haystack.remove_suffix( std::min( trim_pos,  haystack.size() ) );
    return;
}

void split_mut( std::vector<std::string_view>& tokens, const std::string_view& str, const char delim );

std::vector<std::string_view> split( const std::string_view& str, const char delim );

template <class IO, class Container, typename DelimType>
void print_container( IO& io, const Container& container,  const DelimType& delim )
{
    using T = typename Container::value_type;
    std::copy( container.begin(), container.end(), std::ostream_iterator<T>( io, delim ) );
    return;
}

template <class IO, class Container>
void print_container( IO& io, const Container& container )
{
    using T = typename Container::value_type;
    std::copy( container.begin(), container.end(), std::ostream_iterator<T>( io ) );
    return;
}

int report_errors( std::errc );

#endif

