#include <cstdio>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

#include <sysexits.h>

#include <boost/bind/bind.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include "utilities.hpp"

namespace io = boost::iostreams;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

// void print(const std::vector<char>& v) {
//     std::copy(v.begin(), v.end(), std::ostream_iterator<char>(std::cout, " "));
//     return;
// }

void _append(const std::vector<char>& str, std::vector<std::vector<char>> v) {
    v.push_back(str);
    return;
}

template <class ProgramDescriptor>
struct Program {
    ProgramDescriptor name;
    int weight;
    std::vector<ProgramDescriptor> children;

    Program() : name { ProgramDescriptor() }, weight( 0 ), children { std::vector<ProgramDescriptor>() } {}
};

typedef Program<std::vector<char>>  BasicProgram;

template <typename Iterator, typename ProgramDescriptor>
bool parse_prog(Iterator first, Iterator last,
                Program<ProgramDescriptor>& prog) {
    using ascii::space;
    using boost::phoenix::ref;
    using phoenix::push_back;
    using qi::_1;
    using qi::char_;
    using qi::int_;
    auto match = qi::phrase_parse(
        first, last,
        qi::lexeme[+(qi::alpha)][([&prog](const auto& attr) {
            prog.name.assign(attr.begin(), attr.end());
        })] >>
            char_('(') >> int_[ref(prog.weight) = _1] >> char_(')') >>
            -(ascii::string("->") >>
              qi::lexeme[+(qi::alpha)]
                        [phoenix::push_back(phoenix::ref(prog.children), _1)] >>
              *(char_(',') >> qi::lexeme[+(qi::alpha)][phoenix::push_back(
                                  phoenix::ref(prog.children), _1)])),
        space);
    return match;
}

// int main(void) {
//     std::vector<std::vector<char>> tokens;
//     using ProgramDescriptor = std::vector<char>;
//     Program<ProgramDescriptor> prog;
//     std::cout << std::boolalpha
//               << parse_prog(progs_dump.begin(), progs_dump.end(), prog)
//               << std::endl;
//     std::copy(prog.name.begin(), prog.name.end(),
//               std::ostream_iterator<char>(std::cout));
//     std::cout << std::endl;
//     printf("%d %lu\n", prog.weight, prog.children.size());
//     std::for_each(prog.children.begin(), prog.children.end(),
//                   [](const std::vector<char>& v) {
//                       std::copy(v.begin(), v.end(),
//                                 std::ostream_iterator<char>(std::cout));
//                       std::cout << '\n';
//                   });
//     return 0;
// }
int main( int argc, char *argv[] )
{
    if ( argc < 2 ) {
	fprintf( stderr, "Usage: %s <input file path>\n", argv[0] );
	return EX_USAGE;
    }
    const auto input_path = argv[1];
    io::mapped_file input_file;
    input_file.open( input_path );
    // TODO: Boost should give more diagnostics and all. Report those
    if ( !input_file.is_open() ) {
	fprintf( stderr, "Failed to open file %s.", input_path );
	return EX_NOINPUT;
    }
    
    std::string_view v( input_file.data(), input_file.size() );
    trim_right( v, " \t\r\v\n" );
    auto lines = split( v, '\n' );

    std::vector<BasicProgram> progs( lines.size() );
    std::transform( lines.begin(), lines.end(), progs.begin(), []( const auto& line ) {
	BasicProgram prog;
	parse_prog( line.begin(), line.end(), prog );
	return prog;
    } );

    // beautiful
    std::for_each( progs.begin(), progs.end(), []( const decltype(progs)::value_type& prog ) {
	auto& children = prog.children;
	std::for_each( children.begin(), children.end(), []( const std::vector<char>& name ) {
	    print_container( std::cerr, name );
	    return;
	} );
	return;
    } );
    return 0; 
}
