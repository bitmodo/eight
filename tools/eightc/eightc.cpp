#include "eight/parser.h"
#include "eight/ast.h"
#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include "eight/exception.h"

#include "llvm/Support/raw_ostream.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char ** argv) {
    if (argc != 2) {
        cerr << "Must specify one parameter";
        return EXIT_FAILURE;
    }

    ifstream *file = new ifstream(argv[1]);
    Eight::Parse *parse = Eight::parse(argv[1], file);
    std::vector<Eight::AST*> root;
    try {
        root = parse->root();
    } catch (Eight::syntax_error &e) {
        cerr << e.message() << endl;
        return EXIT_FAILURE;
    } catch (exception &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    delete parse;

    Eight::Sema sema;
    for (auto &node : root) {
        sema.analyze(node);
    }

    // if (root.empty()) {
    //     cerr << "Unable to parse file" << endl;

    //     return EXIT_FAILURE;
    // }

    Eight::Visitor visitor(argv[1]);

    for (const auto &node : root) {
        visitor.visit(node);
    }

    for (const auto &node : root) {
        delete node;
    }

    visitor.module->print(llvm::errs(), nullptr);

    return EXIT_SUCCESS;
}
