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

#endif
