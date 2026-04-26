#pragma once
#include <interpreter/exprs/declare.hpp>
#include <interpreter/exprs/assign_val.hpp>
#include <interpreter/exprs/assign.hpp>
#include <interpreter/exprs/digitize.hpp>
#include <interpreter/exprs/do.hpp>
#include <interpreter/exprs/for.hpp>
#include <interpreter/exprs/logitize.hpp>
#include <interpreter/exprs/move.hpp>
#include <interpreter/exprs/rotatel.hpp>
#include <interpreter/exprs/rotater.hpp>
#include <interpreter/exprs/scope.hpp>
#include <interpreter/exprs/switch.hpp>
#include <interpreter/exprs/task.hpp>


static_assert(sizeof(ast::expr) == 40);

