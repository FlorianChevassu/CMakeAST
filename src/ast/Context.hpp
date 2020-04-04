#pragma once

#include <string>

struct Context
{
    size_t line;
    size_t column;
};

struct Range
{
  Context begin;
  Context end;
};

