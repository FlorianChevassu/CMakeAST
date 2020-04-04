#include "ArgumentsASTBuilder.hpp"

#include "NodeFactory.hpp"


BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::Arguments>::Build(const cmake::language::Token& i_token)
{
  auto res = NodeFactory::CreateNode(BasicNode::Type::Arguments);

  // Recursively handle the token children.
  this->HandleChildren(res, i_token.children);

  res->SetRange(this->ConvertRange(i_token.range));

  return res;
}

void TokenToNode<cmake::language::ElementType::Arguments>::HandleChildren(BasicNode::PolymorphicNode& i_node, const std::vector<cmake::language::Token>& i_children)
{
  // An Arguments token represents arguments surrounded by (). It has
  //  - An Argument or QuotedArgument token
  //  - Optionally a SeparatedArguments token
  //
  // A SeparatedArguments token has a set of tokens in
  //  - Argumentor or QuotedArgument
  //  - SeparatedArguments
  //  - Arguments
  //  - Separation
  //  - Space ?
  for (auto& childToken : i_children)
  {
    switch (childToken.type)
    {
    case cmake::language::ElementType::Arguments:
      i_node->GetChildren().push_back(
        TokenToNode<cmake::language::ElementType::Arguments>{m_script}.Build(childToken)
        );
      break;
    case cmake::language::ElementType::QuotedArgument:
      i_node->GetChildren().push_back(
        TokenToNode<cmake::language::ElementType::Argument>{m_script}.Build(childToken)
        );
      break;
    case cmake::language::ElementType::Argument:
      i_node->GetChildren().push_back(
        TokenToNode<cmake::language::ElementType::Argument>{m_script}.Build(childToken)
        );
      break;
    case cmake::language::ElementType::SeparatedArguments:
      this->HandleChildren(i_node, childToken.children);
      break;
    default:
      // Ignore separations, spaces, etc.
      continue;
    }
  }
}
