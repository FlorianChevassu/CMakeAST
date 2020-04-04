#pragma once
#include "Node.hpp"
#include "ElementType.hpp"
#include "Token.hpp"
#include "Error.hpp"

class ParsingException : public std::exception
{
public:
  explicit ParsingException(cmake::language::Error i_error)
    : m_error(i_error)
  {
  }

  const char* what() const override
  {
    // TODO: We should add children error messages...
    return m_error.message.c_str();
  }

  cmake::language::Error m_error;
};

class ASTBuilder
{
public:
  static BasicNode::PolymorphicNode Build(std::string_view i_script);
};

class TokenToNodeBase
{
public:
  TokenToNodeBase(std::string_view i_script) : m_script(i_script) {}

protected:
  BasicNode::PolymorphicNode BuildNode(std::string_view i_script, const cmake::language::Token& i_token, cmake::language::ElementType i_tokenType);
  void BuildChildren(BasicNode::PolymorphicNode& i_node, const cmake::language::Token& i_token);
  static const std::string_view GetStringView(const cmake::language::Range& i_range);
  static const std::string GetString(const cmake::language::Range& i_range);

  template <BasicNode::Type NT>
  BasicNode::PolymorphicNode BuildClassicNode(const cmake::language::Token& i_token)
  {
    auto node = NodeFactory::CreateNode<NT>();
    node->SetRange(this->ConvertRange(i_token.range));
    this->BuildChildren(node, i_token);
    return node;
  }

  Range ConvertRange(const cmake::language::Range& i_range);
protected:
  std::string_view m_script;
};

template <cmake::language::ElementType TokenType>
class TokenToNode : public TokenToNodeBase
{
public:
  TokenToNode(const std::string_view& i_script) : TokenToNodeBase(i_script) {}

  BasicNode::PolymorphicNode Build(const cmake::language::Token& i_token);
};
