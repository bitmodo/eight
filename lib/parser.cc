#include "eight/parser.h"
#include "eight/parser/token.h"

#include "eight/ast.h"
#include "eight/ast/statement.h"
#include "eight/ast/codeBlock.h"
#include "eight/ast/function.h"

#include <exception>
#include <fstream>
#include <memory>
#include <type_traits>

using namespace std;
using namespace Eight;

extern "C" Parse * ParseEightFile(const char * file) {
    return parseFile(file);
}

Parse * Eight::parseFile(const string file) {
    ifstream * stream = new ifstream(file);
    Parse * result = parse(file, stream);
    stream->close();
    
    return result;
}

Parse * Eight::parse(const string filename, const istream * in) {
    return new Parse(filename, in);
}

Parse::Parse(const string filename, const istream * in) : m_name(filename), m_in(move(in)) {}
