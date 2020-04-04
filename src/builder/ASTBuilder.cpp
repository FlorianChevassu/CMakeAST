#include "ASTBuilder.hpp"

#include "NodeFactory.hpp"
#include "FileParser.hpp"
#include "Error.hpp"
#include "Token.hpp"

BasicNode::PolymorphicNode ASTBuilder::Build(std::string_view i_script)
{
  auto token = cmake::language::Parser<cmake::language::ElementType::File>{}.Parse(cmake::language::Range{ i_script.begin(), i_script.end() });

  if (!token.has_value())
  {
    throw ParsingException(token.error());
  }

  return TokenToNode<cmake::language::ElementType::File>{i_script}.Build(token.value());
}

BasicNode::PolymorphicNode TokenToNodeBase::BuildNode(std::string_view i_script, const cmake::language::Token& i_token, cmake::language::ElementType i_tokenType)
{
  switch (i_tokenType)
  {
  case cmake::language::ElementType::Argument:
    return TokenToNode<cmake::language::ElementType::Argument>{i_script}.Build(i_token);
  case cmake::language::ElementType::Arguments:
    return TokenToNode<cmake::language::ElementType::Arguments>{i_script}.Build(i_token);
  case cmake::language::ElementType::BracketComment:
    return TokenToNode<cmake::language::ElementType::BracketComment>{i_script}.Build(i_token);
  case cmake::language::ElementType::CommandInvocation:
    return TokenToNode<cmake::language::ElementType::CommandInvocation>{i_script}.Build(i_token);
  case cmake::language::ElementType::File:
    return TokenToNode<cmake::language::ElementType::File>{i_script}.Build(i_token);
  case cmake::language::ElementType::LineComment:
    return TokenToNode<cmake::language::ElementType::LineComment>{i_script}.Build(i_token);
  case cmake::language::ElementType::QuotedArgument:
    return TokenToNode<cmake::language::ElementType::QuotedArgument>{i_script}.Build(i_token);
  case cmake::language::ElementType::UnquotedArgument:
    return TokenToNode<cmake::language::ElementType::UnquotedArgument>{i_script}.Build(i_token);
  default:
    assert(false);
    return nullptr;
  }
}

void TokenToNodeBase::BuildChildren(BasicNode::PolymorphicNode& i_node, const cmake::language::Token& i_token)
{
  for (auto& t : i_token.children)
  {
    i_node->GetChildren().push_back(
      this->BuildNode(m_script, t, t.type)
      );
  }
}

Range TokenToNodeBase::ConvertRange(const cmake::language::Range& i_range)
{
  auto GetLine = [&](auto it)
  {
    return std::count(m_script.begin(), it, '\n') + 1;
  };
  auto GetColumn = [&](auto it)
  {
    // find line first character
    auto rbegin = std::make_reverse_iterator(it);
    auto firstCharIt = std::find(rbegin, m_script.rend(), '\n');
    return std::distance(rbegin, firstCharIt) + 1;
  };

  Range res;
  res.begin.line = GetLine(i_range.begin);
  res.begin.column = GetColumn(i_range.begin);
  res.end.line = GetLine(i_range.end);
  res.end.column = GetColumn(i_range.end);
  return res;
}

const std::string_view TokenToNodeBase::GetStringView(const cmake::language::Range& i_range)
{
  return std::string_view(&*i_range.begin, std::distance(i_range.begin, i_range.end));
}

const std::string TokenToNodeBase::GetString(const cmake::language::Range& i_range)
{
  return std::string(&*i_range.begin, std::distance(i_range.begin, i_range.end));
}

