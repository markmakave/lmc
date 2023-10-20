#pragma once

#include <string_view>
#include <memory>

#include "scope.hpp"

namespace lmc {

class parser {
public:
    
    class expression {
    public:
        virtual ~expression() = default;
    };

    class variable : public expression {
        scope _scope;
        std::string_view _name;
    };

    class binary : public expression {
        std::unique_ptr<expression> lhs, rhs;
    };

    class call : public expression {

    };

public:

    parser() {}

protected:

};

}
