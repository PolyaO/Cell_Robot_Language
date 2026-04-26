#pragma once
#include "interpreter/rvals/rval.hpp"
#include <interpreter/rvals/res.hpp>
#include <interpreter/rvals/ref.hpp>
#include <interpreter/rvals/env.hpp>
#include <interpreter/rvals/idx.hpp>
#include <interpreter/rvals/binary.hpp>
#include <interpreter/rvals/unary.hpp>
#include <interpreter/rvals/reduce.hpp>
#include <interpreter/rvals/extend.hpp>
static_assert(sizeof(ast::rval) == 56);

