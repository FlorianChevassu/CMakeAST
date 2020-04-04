#pragma once
#include "ASTBuilder.hpp"

class ArgumentTokenToNode : public TokenToNodeBase
{
public:
  ArgumentTokenToNode(const std::string_view& i_script) : TokenToNodeBase(i_script) {}

  BasicNode::PolymorphicNode Build(const cmake::language::Token& i_token);
};

template <>
class TokenToNode<cmake::language::ElementType::Argument> : public ArgumentTokenToNode
{
public:
  TokenToNode(const std::string_view& i_script) : ArgumentTokenToNode(i_script) {}

  BasicNode::PolymorphicNode Build(const cmake::language::Token& i_token);
};

template <>
class TokenToNode<cmake::language::ElementType::QuotedArgument> : public ArgumentTokenToNode
{
public:
  TokenToNode(const std::string_view& i_script) : ArgumentTokenToNode(i_script) {}

  BasicNode::PolymorphicNode Build(const cmake::language::Token& i_token);
};

template <>
class TokenToNode<cmake::language::ElementType::UnquotedArgument> : public ArgumentTokenToNode
{
public:
  TokenToNode(const std::string_view& i_script) : ArgumentTokenToNode(i_script) {}

  BasicNode::PolymorphicNode Build(const cmake::language::Token& i_token);
};
