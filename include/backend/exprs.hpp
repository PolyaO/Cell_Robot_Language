#pragma once
#include <backend/exprs/assign.hpp>
#include <backend/exprs/digitize.hpp>
#include <backend/exprs/do.hpp>
#include <backend/exprs/for.hpp>
#include <backend/exprs/logitize.hpp>
#include <backend/exprs/move.hpp>
#include <backend/exprs/rotatel.hpp>
#include <backend/exprs/rotater.hpp>
#include <backend/exprs/scope.hpp>
#include <backend/exprs/switch.hpp>
#include <backend/exprs/task.hpp>

static_assert(sizeof(ast::expr) == 40);

