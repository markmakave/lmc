#pragma once

#include <llvm/

namespace lmc {

class expression {
public:
    virtual ~expression() = default;
    virtual llvm::Value *codegen() = 0;
};

}
