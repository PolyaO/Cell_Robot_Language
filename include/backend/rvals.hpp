#pragma once
#include <backend/rvals/res.hpp>
#include <backend/rvals/ref.hpp>
#include <backend/rvals/env.hpp>
#include <backend/rvals/idx.hpp>
#include <backend/rvals/binary.hpp>
#include <backend/rvals/unary.hpp>
#include <backend/rvals/reduce.hpp>
#include <backend/rvals/extend.hpp>
#include <backend/rvals/var/var.hpp>
static_assert(sizeof(ast::rval) == 72);

