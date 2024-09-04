//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/BlockAnnotator.cc
//---------------------------------------------------------------------------//
#include "BlockAnnotator.hh"

#include <iostream>
#include <unordered_map>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>
#include <llvm/Transforms/BasicBlockUtils.h>

using llvm::dyn_cast;
using std::cout;
using std::endl;

namespace qiree
{
namespace
{
class BlockIdFinder
{
  public:
    explicit BlockIdFinder(llvm::Function& func)
    {
        for (llvm::BasicBlock& block : func)
        {
            ids_.insert({&block, ids_.size()});
        }
    }

    size_type operator()(llvm::BasicBlock const* bb)
    {
        QIREE_EXPECT(bb);
        auto result = ids_.find(bb);
        QIREE_ENSURE(result != ids_.end());
        return result->second;
    }

  private:
    std::unordered_map<llvm::BasicBlock const*, size_type> ids_;
};

}  // namespace

//---------------------------------------------------------------------------//
// INLINE DEFINITIONS
//---------------------------------------------------------------------------//
/*!
 * Construct with defaults.
 */
BlockAnnotator::BlockAnnotator(Module* module) : module_{module->get()}
{
    QIREE_EXPECT(module && module_);
}

//---------------------------------------------------------------------------//
/*!
 * Annotate a function with begin/end block instructions.
 */
void BlockAnnotator::operator()(llvm::Function* func)
{
    QIREE_EXPECT(func);

    // Create a map of basic blocks to IDs
    BlockIdFinder find_block_id{*func};

#if 0
    // Vector of "passthrough" blocks
    constexpr size_type not_applic = -1;
    std::vector<size_type> passthrough(func->size(), not_applic);
    for (llvm::BasicBlock& block : *func)
    {
        if (block.size() != 1)
        {
            // Not an empty block
            continue;
        }
        auto* bi = dyn_cast<llvm::BranchInst>(term);
        if (!bi)
        {
            // Not a branch instruction
            continue;
        }
        if (bi->isConditional())
        {
            // Conditional, presumably on input args?
            continue;
        }
    }
#endif
    {
        llvm::DomTreeUpdater dtu(DT, DomTreeUpdater::UpdateStrategy::Lazy);
    }

    for (llvm::BasicBlock& block : *func)
    {
        // Decorate top of block
        cout << "Function " << find_block_id(&block) << ": " << block.size()
             << " instructions\n";
        llvm::Instruction const* term = block.getTerminator();
        if (auto* bi = dyn_cast<llvm::BranchInst>(term))
        {
            if (bi->isConditional())
            {
                auto* true_bb = bi->getSuccessor(0);
                auto* false_bb = bi->getSuccessor(1);
                // auto* cond = bi->getCondition();
                cout << " - Conditional: true = " << find_block_id(true_bb)
                     << ", false = " << find_block_id(false_bb) << endl;
            }
            else
            {
                cout << " - Unconditional" << endl;
            }
        }
        else if (auto* ri = dyn_cast<llvm::ReturnInst>(term))
        {
            cout << " - Return" << endl;
        }
        else if (term)
        {
            cout << " - Invalid terminator type '" << term->getOpcodeName()
                 << '\'' << endl;
        }
        else
        {
            cout << " - No terminator????\n";
        }
    }
}

//---------------------------------------------------------------------------//
}  // namespace qiree
