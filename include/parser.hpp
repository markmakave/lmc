#pragma once

#include <string_view>
#include <memory>

#include "scope.hpp"

namespace lmc {

class parser {
public:

    parser(lexer& source) : _source(source) {}

    // std::unique_ptr<statement> parse();

protected:

    lexer _source;
};

}
