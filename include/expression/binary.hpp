#pragma once 

#include <memory>

#include "expression.hpp"

namespace lmc {

class binary : public expression {
    std::unique_ptr<expression> lhs, rhs;
};

}
