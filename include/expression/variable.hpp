#pragma once 

#include <string_view>

#include "expression.hpp"
#include "scope.hpp"

namespace lmc {

class variable : public expression {
    scope _scope;
    std::string_view _name;
};

}
