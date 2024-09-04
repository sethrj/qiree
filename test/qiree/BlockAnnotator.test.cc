//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/BlockAnnotator.test.cc
//---------------------------------------------------------------------------//
#include "qiree/BlockAnnotator.hh"

#include "qiree/Module.hh"
#include "qiree_test.hh"

namespace qiree
{
namespace test
{
//---------------------------------------------------------------------------//

class BlockAnnotatorTest : public ::qiree::test::Test
{
  protected:
    void SetUp() override {}
};

//---------------------------------------------------------------------------//
TEST_F(BlockAnnotatorTest, teleport)
{
    Module m(this->test_data_path("teleport.ll"));

    BlockAnnotator annotate_block{&m};
    annotate_block(m.entrypoint());
    annotate_block(m.entrypoint());
}

//---------------------------------------------------------------------------//
TEST_F(BlockAnnotatorTest, if_result)
{
    Module m(this->test_data_path("if_result.ll"));

    BlockAnnotator annotate_block{&m};
    annotate_block(m.entrypoint());
    annotate_block(m.entrypoint());
}

//---------------------------------------------------------------------------//
}  // namespace test
}  // namespace qiree
