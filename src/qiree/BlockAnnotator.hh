//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/BlockAnnotator.hh
//---------------------------------------------------------------------------//
#pragma once

#include "Module.hh"

namespace llvm
{
class Module;
class Function;
class Value;
}  // namespace llvm

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Annotate each block with start/stop instructions.
 */
class BlockAnnotator
{
  public:
    //!@{
    //! \name Type aliases
    //!@}

  public:
    // Construct with a module
    explicit BlockAnnotator(Module*);

    // Annotate a function in the module
    void operator()(llvm::Function* func);

  private:
    llvm::Module* module_{nullptr};
    // llvm::Function* begin_block_func_{nullptr};
};

//---------------------------------------------------------------------------//
}  // namespace qiree
