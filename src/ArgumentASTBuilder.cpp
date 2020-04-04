#include "ArgumentASTBuilder.hpp"

#include "NodeFactory.hpp"


BasicNode::PolymorphicNode ArgumentTokenToNode::Build(const cmake::language::Token& i_token)
{
  auto res = NodeFactory::CreateNode(BasicNode::Type::Argument);
  auto& argumentNode = res.GetAs<BasicNode::Type::Argument>();

  argumentNode.SetRange(this->ConvertRange(i_token.range));

  assert(i_token.children.size() == 1);
  const auto& child = i_token.children[0];

  switch (child.type)
  {
  case cmake::language::ElementType::QuotedArgument:
  {
    argumentNode.SetIsQuoted(true);
    argumentNode.SetIsBracket(false);
    auto rangeWithoutQuotes = i_token.range;
    if (argumentNode.IsQuoted())
    {
      rangeWithoutQuotes.begin += 1;
      rangeWithoutQuotes.end -= 1;
    }
    argumentNode.SetValue(this->GetString(rangeWithoutQuotes));
  }
  break;
  case cmake::language::ElementType::UnquotedArgument:
  {
    argumentNode.SetIsQuoted(false);
    argumentNode.SetIsBracket(false);
    argumentNode.SetValue(this->GetString(i_token.range));
  }
  break;
  case cmake::language::ElementType::BracketArgument:
  {
    argumentNode.SetIsQuoted(false);
    argumentNode.SetIsBracket(true);

    // The content of the bracket argument is given by the children of "child".
    // It should have exactly 3 children: open, content and close.
    assert(child.children.size() == 3);
    const auto& content = child.children[1];

    argumentNode.SetValue(this->GetString(content.range));
  }
  break;
  default:
    // This is not a valid child...
    assert(false);
  }


  // For now, we do not create "variable nodes" and such.

  return res;
}

BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::Argument>::Build(const cmake::language::Token& i_token)
{
  return ArgumentTokenToNode::Build(i_token);
}

BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::QuotedArgument>::Build(const cmake::language::Token& i_token)
{
  return ArgumentTokenToNode::Build(i_token);
}

BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::UnquotedArgument>::Build(const cmake::language::Token& i_token)
{
  return ArgumentTokenToNode::Build(i_token);
}
