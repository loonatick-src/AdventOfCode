#include <cstdio>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>

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
// TODO: consider refactoring to use Boost.Graphs
namespace io = boost::iostreams;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

void _append(const std::vector<char>& str, std::vector<std::vector<char>> v) {
    v.push_back(str);
    return;
}

template <class ProgramDescriptor>
struct Program {
    typedef ProgramDescriptor program_descriptor_type;
    ProgramDescriptor name;
    int weight;
    std::vector<ProgramDescriptor> children;

    // default ctor
    Program() : name { ProgramDescriptor() },
		weight( 0 ),
		children { std::vector<ProgramDescriptor>() } {}
    Program( const ProgramDescriptor& descr ) : name { descr },
						weight( 0 ),
						children { std::vector<ProgramDescriptor>() } {}
    bool operator==( const Program& other ) const noexcept {
	return this->name == other.name;
    }
};
typedef Program<std::vector<char>> BasicProgram;

template <class V>
struct SHash {
    auto operator()( const V& v ) const {
	const auto& name = v.name;
	return boost::hash_range( name.begin(), name.end() );
    }
};

template <typename Iterator, typename ProgramDescriptor>
bool parse_prog(Iterator first, Iterator last,
                Program<ProgramDescriptor>& prog) {
    // yes, I think that this is overkill, but boost is so cool.
    // makes me want to make my own parser combinator lib that gives
    // me more control so I'm not returning `vector<char>`.  all the
    // time.
    using ascii::space;
    using boost::phoenix::ref;
    using phoenix::push_back;
    using qi::_1;
    using qi::char_;
    using qi::int_;
    // beautiful
    // idiomatic
    // algebraic
    auto match = qi::phrase_parse(
				  first, last,
				  qi::lexeme[ +( qi::alpha ) ][ ( [&prog]( const auto& attr ) {
				      prog.name.assign( attr.begin(), attr.end() );
				  } ) ]
				      >> char_('(') >> int_[ ref(prog.weight) = _1 ]
				      >> char_(')') >> -( ascii::string( "->" )
							 >> qi::lexeme[ +( qi::alpha ) ][ phoenix::push_back( phoenix::ref( prog.children ), _1 ) ]
							  >> *( char_(',')
								>> qi::lexeme[ +( qi::alpha ) ][ phoenix::push_back( phoenix::ref(prog.children), _1 ) ] ) ),
				  space );
    return match;
}

struct ProgramProperties {
    int in_degree;
    int out_degree;
    int weight;
    int total_weight;

    ProgramProperties() : in_degree(0), out_degree(0), weight(0), total_weight(0) {}
    ProgramProperties( int weight ) : in_degree(0), out_degree(0), weight(weight), total_weight(0) {}
    template <class ProgramDescriptor>
    ProgramProperties( const Program<ProgramDescriptor>& prog ) : in_degree(0), out_degree( prog.children.size() ), weight( prog.weight ), total_weight(0) {}
};

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

    // the mapped file is going to stick around for the entire duration
    // of the program, so no need to copy things around.
    std::string_view v( input_file.data(), input_file.size() );
    trim_right( v, " \t\r\v\n" );
    auto lines = split( v, '\n' );

    std::unordered_map<BasicProgram, ProgramProperties, SHash<BasicProgram>> progs;
    std::transform( lines.begin(), lines.end(), std::inserter( progs, progs.begin() ), []( const auto& line ) {
	BasicProgram prog;
	parse_prog( line.begin(), line.end(), prog );
	ProgramProperties prop( prog );
	return std::make_pair( prog, prop );
    } );
    
    std::for_each( progs.begin(), progs.end(), [&] ( const auto& p ) {
	const auto& prog = p.first;
	const auto& children = prog.children;
	std::for_each( children.begin(), children.end(), [&]( const auto& child ) {
	    const BasicProgram child_prog( child );
	    auto child_it = progs.find( child_prog );
	    child_it->second.in_degree++;
	} );
    } );
    auto root = std::find_if( progs.begin(), progs.end(), []( const auto& p ) {
	return p.second.in_degree == 0;
    } );
    if ( root != progs.end() ) {
	auto name = root->first.name;
	print_container( std::cout, name );
    } else {
	std::cerr << "You done fucked up\n";
	return 1;
    }
    return 0;
}
