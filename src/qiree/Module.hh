//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Module.hh
//---------------------------------------------------------------------------//
#pragma once

#include <memory>

#include "Assert.hh"
#include "Types.hh"

namespace llvm
{
class Module;
class Function;
}  // namespace llvm

namespace qiree
{
//---------------------------------------------------------------------------//
/*!
 * Load a QIR LLVM module.
 */
class Module
{
  public:
    //!@{
    //! \name Type aliases
    using UPModule = std::unique_ptr<llvm::Module>;
    //!@}

  public:
    // Default empty constructor
    Module();
    // Externally defined defaults
    ~Module();
    Module(Module&&);
    Module& operator=(Module&&);
    // Prevent copying
    Module(Module const&) = delete;
    Module& operator=(Module const&) = delete;

    // Construct from an externally created LLVM module
    explicit Module(UPModule&& module);

    // Construct with an LLVM IR file (bitcode or disassembled)
    explicit Module(std::string const& filename);

    // Construct with an LLVM IR file (bitcode or disassembled) and entry point
    Module(std::string const& filename, std::string const& entrypoint);

    // Process entry point attributes
    EntryPointAttrs load_entry_point_attrs() const;

    // Translate module attributes into flags
    ModuleFlags load_module_flags() const;

    //! True if the module has been constructed (and not moved)
    explicit operator bool() const { return static_cast<bool>(module_); }

    //!@{
    //! \name Mutators

    //! Release ownership of the embedded module
    UPModule release() { return std::move(module_); }

    //! Access the embedded module (maybe null)
    llvm::Module* get() { return module_.get(); }

    // Access the quantum kernel function
    inline llvm::Function* entrypoint();

  private:
    UPModule module_;
    llvm::Function* entrypoint_{nullptr};
};

//---------------------------------------------------------------------------//
/*!
 * Access the quantum kernel function.
 */
llvm::Function* Module::entrypoint()
{
    QIREE_ASSERT(*this);
    return entrypoint_;
}

//---------------------------------------------------------------------------//
}  // namespace qiree
