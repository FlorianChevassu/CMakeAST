#include "ASTBuilder.hpp"

#include "NodeFactory.hpp"
#include "FileParser.hpp"
#include "Error.hpp"
#include "Token.hpp"


template <cmake::language::ElementType TokenType>
void ASTBuilder::BuildNode(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
}

void ASTBuilder::BuildChildren(BasicNode::PolymorphicNode& i_node, const cmake::language::Token& i_token)
{
  for (auto& t : i_token.children)
  {
    this->BuildNode(i_node, t, t.type);
  }
}

Range ASTBuilder::ConvertRange(const cmake::language::Range& i_range)
{
  auto GetLine = [&](auto it)
  {
    return std::count(m_fileContent.begin(), it, '\n') + 1;
  };
  auto GetColumn = [&](auto it)
  {
    // find line first character
    auto rbegin = std::make_reverse_iterator(it);
    auto firstCharIt = std::find(rbegin, m_fileContent.rend(), '\n');
    return std::distance(rbegin, firstCharIt) + 1;
  };

  Range res;
  res.begin.line = GetLine(i_range.begin);
  res.begin.column = GetColumn(i_range.begin);
  res.end.line = GetLine(i_range.end);
  res.end.column = GetColumn(i_range.end);
  return res;
}

const std::string_view GetStringView(const cmake::language::Range& i_range)
{
  return std::string_view(&*i_range.begin, std::distance(i_range.begin, i_range.end));
}

template <BasicNode::Type NT>
void ASTBuilder::BuildClassicNode(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  auto node = NodeFactory::CreateNode<NT>();
  node->SetRange(this->ConvertRange(i_token.range));
  this->BuildChildren(node, i_token);
  i_parent->GetChildren().push_back(std::move(node));
}

template <>
void ASTBuilder::BuildNode<cmake::language::ElementType::Identifier>(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  auto& commandInvocation = i_parent.GetAs<BasicNode::Type::CommandInvocation>();
  commandInvocation.SetCommandName(std::string(GetStringView(i_token.range)));
}

template <>
void ASTBuilder::BuildNode<cmake::language::ElementType::FileElement>(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  this->BuildClassicNode<BasicNode::Type::FileElement>(i_parent, i_token);
}

template <>
void ASTBuilder::BuildNode<cmake::language::ElementType::CommandInvocation>(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  this->BuildClassicNode<BasicNode::Type::CommandInvocation>(i_parent, i_token);
}

template <>
void ASTBuilder::BuildNode<cmake::language::ElementType::SeparatedArguments>(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  // SeparatedArguments can have either
  // - only one argument
  // - a list of separated arguments, enclosed by ().
  //
  // -> If the first non space character we find is a (, then create an Arguments child
  // -> otherwise create an Argument child.
  auto it = std::find_if(i_token.range.begin, i_token.range.end, [](auto c)
    {
      return !std::isspace(c);
    });

  assert(it != i_token.range.end);
  if (it == i_token.range.end)
  {
    // We should have had an error from the parser !
    return;
  }

  if (*it == '(')
  {
    this->BuildClassicNode<BasicNode::Type::Arguments>(i_parent, i_token);
  }
  else
  {
    this->BuildClassicNode<BasicNode::Type::Argument>(i_parent, i_token);
  }
}

template <>
void ASTBuilder::BuildNode<cmake::language::ElementType::Argument>(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  this->BuildClassicNode<BasicNode::Type::Argument>(i_parent, i_token);
}

template <>
void ASTBuilder::BuildNode<cmake::language::ElementType::Arguments>(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  auto node = NodeFactory::CreateNode<BasicNode::Type::Arguments>();
  node->SetRange(this->ConvertRange(i_token.range));
  for (auto& t : i_token.children)
  {
    if (t.type == cmake::language::ElementType::Argument)
    {
      this->BuildNode<cmake::language::ElementType::Argument>(node, t);
    }
    else if (t.type == cmake::language::ElementType::Arguments
      || t.type == cmake::language::ElementType::SeparatedArguments)
    {
      // Check if the next character is an opening parenthesis.
      auto it = std::find_if(t.range.begin, t.range.end, [](auto c)
        {
          return !std::isspace(c);
        });

      if (it != t.range.end)
      {
        if (*it == '(')
        {
          this->BuildNode<cmake::language::ElementType::Arguments>(node, t);
        }
        else
        {
          for (auto& tchild : t.children)
          {
            this->BuildNode(node, tchild, tchild.type);
          }
        }
      }
    }
    else if (t.type == cmake::language::ElementType::Separation ||
      t.type == cmake::language::ElementType::Space)
    {
      // Ignore
    }
    else
    {
      assert(false);
    }
  }

  i_parent->GetChildren().push_back(std::move(node));
}

template <>
void ASTBuilder::BuildNode<cmake::language::ElementType::BracketComment>(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  this->BuildClassicNode<BasicNode::Type::BracketComment>(i_parent, i_token);
}

template <>
void ASTBuilder::BuildNode<cmake::language::ElementType::LineComment>(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  this->BuildClassicNode<BasicNode::Type::Comment>(i_parent, i_token);
}

template <>
void ASTBuilder::BuildNode<cmake::language::ElementType::LineEnding>(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token)
{
  // If the line ending contains a comment, create it.
  for (auto& t : i_token.children)
  {
    if (t.type == cmake::language::ElementType::LineComment)
    {
      this->BuildNode<cmake::language::ElementType::LineComment>(i_parent, t);
    }
  }
}

ASTBuilder::ASTBuilder(const std::string& i_fileContent)
  : m_fileContent(i_fileContent)
{
}

BasicNode::PolymorphicNode ASTBuilder::Build()
{
  cmake::language::Parser<cmake::language::ElementType::File> fileParser;
  auto token = fileParser.Parse(cmake::language::Range{ m_fileContent.begin(), m_fileContent.end() });

  if (!token)
  {
    // TODO: throw
    assert(false);
  }

  auto res = NodeFactory::CreateNode(BasicNode::Type::File);
  BuildChildren(res, *token);
  return res;
}

void ASTBuilder::BuildNode(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token, cmake::language::ElementType i_tokenType)
{
  switch (i_tokenType)
  {
  case cmake::language::ElementType::Argument:
    this->BuildNode<cmake::language::ElementType::Argument>(i_parent, i_token);
    break;
  case cmake::language::ElementType::Arguments:
    this->BuildNode<cmake::language::ElementType::Arguments>(i_parent, i_token);
    break;
  case cmake::language::ElementType::BracketArgument:
    this->BuildNode<cmake::language::ElementType::BracketArgument>(i_parent, i_token);
    break;
  case cmake::language::ElementType::BracketClose:
    this->BuildNode<cmake::language::ElementType::BracketClose>(i_parent, i_token);
    break;
  case cmake::language::ElementType::BracketComment:
    this->BuildNode<cmake::language::ElementType::BracketComment>(i_parent, i_token);
    break;
  case cmake::language::ElementType::BracketContent:
    this->BuildNode<cmake::language::ElementType::BracketContent>(i_parent, i_token);
    break;
  case cmake::language::ElementType::BracketOpen:
    this->BuildNode<cmake::language::ElementType::BracketOpen>(i_parent, i_token);
    break;
  case cmake::language::ElementType::CommandInvocation:
    this->BuildNode<cmake::language::ElementType::CommandInvocation>(i_parent, i_token);
    break;
  case cmake::language::ElementType::EscapeEncoded:
    this->BuildNode<cmake::language::ElementType::EscapeEncoded>(i_parent, i_token);
    break;
  case cmake::language::ElementType::EscapeIdentity:
    this->BuildNode<cmake::language::ElementType::EscapeIdentity>(i_parent, i_token);
    break;
  case cmake::language::ElementType::EscapeSemilicon:
    this->BuildNode<cmake::language::ElementType::EscapeSemilicon>(i_parent, i_token);
    break;
  case cmake::language::ElementType::EscapeSequence:
    this->BuildNode<cmake::language::ElementType::EscapeSequence>(i_parent, i_token);
    break;
  case cmake::language::ElementType::File:
    this->BuildNode<cmake::language::ElementType::File>(i_parent, i_token);
    break;
  case cmake::language::ElementType::FileElement:
    this->BuildNode<cmake::language::ElementType::FileElement>(i_parent, i_token);
    break;
  case cmake::language::ElementType::Identifier:
    this->BuildNode<cmake::language::ElementType::Identifier>(i_parent, i_token);
    break;
  case cmake::language::ElementType::LineComment:
    this->BuildNode<cmake::language::ElementType::LineComment>(i_parent, i_token);
    break;
  case cmake::language::ElementType::LineEnding:
    this->BuildNode<cmake::language::ElementType::LineEnding>(i_parent, i_token);
    break;
  case cmake::language::ElementType::NewLine:
    this->BuildNode<cmake::language::ElementType::NewLine>(i_parent, i_token);
    break;
  case cmake::language::ElementType::QuotedArgument:
    this->BuildNode<cmake::language::ElementType::QuotedArgument>(i_parent, i_token);
    break;
  case cmake::language::ElementType::QuotedContinuation:
    this->BuildNode<cmake::language::ElementType::QuotedContinuation>(i_parent, i_token);
    break;
  case cmake::language::ElementType::QuotedElement:
    this->BuildNode<cmake::language::ElementType::QuotedElement>(i_parent, i_token);
    break;
  case cmake::language::ElementType::SeparatedArguments:
    this->BuildNode<cmake::language::ElementType::SeparatedArguments>(i_parent, i_token);
    break;
  case cmake::language::ElementType::Separation:
    this->BuildNode<cmake::language::ElementType::Separation>(i_parent, i_token);
    break;
  case cmake::language::ElementType::Space:
    this->BuildNode<cmake::language::ElementType::Space>(i_parent, i_token);
    break;
  case cmake::language::ElementType::UnquotedArgument:
    this->BuildNode<cmake::language::ElementType::UnquotedArgument>(i_parent, i_token);
    break;
  case cmake::language::ElementType::UnquotedElement:
    this->BuildNode<cmake::language::ElementType::UnquotedElement>(i_parent, i_token);
    break;
  default:
    assert(false);
  }
}
