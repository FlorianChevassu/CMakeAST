#pragma once
#include "ASTBuilder.hpp"

template <>
class TokenToNode<cmake::language::ElementType::File> : public TokenToNodeBase
{
public:
  TokenToNode(const std::string_view& i_script) : TokenToNodeBase(i_script) {}

  BasicNode::PolymorphicNode Build(const cmake::language::Token& i_token);
};
