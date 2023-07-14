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
};
typedef Program<std::vector<char>>  BasicProgram;

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

struct Degrees {
    int in_degree;
    int out_degree;

    Degrees() : in_degree(0), out_degree(0) {}
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
    
    std::string_view v( input_file.data(), input_file.size() );
    trim_right( v, " \t\r\v\n" );
    auto lines = split( v, '\n' );

    std::vector<BasicProgram> progs( lines.size() );
    std::transform( lines.begin(), lines.end(), progs.begin(), []( const auto& line ) {
	BasicProgram prog;
	parse_prog( line.begin(), line.end(), prog );
	return prog;
    } );

    using PD = BasicProgram::program_descriptor_type;
    std::unordered_map<PD, Degrees, Hash<PD>> degree_property_map;
    for ( const auto& prog: progs ) {
	const auto& name = prog.name;
	const auto& children = prog.children;
	auto it = degree_property_map.find( name );
	if ( it == degree_property_map.end() ) {
	    it = degree_property_map.insert( it, std::make_pair( name, Degrees() ) );
	}
	(it->second).out_degree += children.size();
	for ( const auto& child: children ) {
	    auto it_child = degree_property_map.find( child );
	    if ( it_child == degree_property_map.end() ) {
		it_child = degree_property_map.insert( it_child, std::make_pair( child, Degrees() ) );
	    }
	    (it_child->second).in_degree++;
	}
    }
    auto it = std::find_if( degree_property_map.begin(), degree_property_map.end(), []( const std::pair<PD, Degrees>& p ) {
	return p.second.in_degree == 0;
    } );
    if ( it != degree_property_map.end() ) {
	auto name = it->first;
	print_container( std::cout, name );
    } else {
	std::cerr << "You done fucked up\n";
	return 1;
    }

    return 0; 
}
