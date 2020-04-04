#include "CommandInvocationASTBuilder.hpp"

#include "NodeFactory.hpp"

BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::CommandInvocation>::Build(const cmake::language::Token& i_token)
{
  auto res = NodeFactory::CreateNode(BasicNode::Type::CommandInvocation);

  Range finalRange = this->ConvertRange(i_token.range);
  // A CommandInvocation has
  //  - An Identifier token
  //  - Optionally an Arguments token.
  for (auto& childToken : i_token.children)
  {
    switch (childToken.type)
    {
    case cmake::language::ElementType::Identifier:
      finalRange.begin = this->ConvertRange(childToken.range).begin;
      res.GetAs<BasicNode::Type::CommandInvocation>().SetCommandName(GetString(childToken.range));
      break;
    case cmake::language::ElementType::Arguments:
      res->GetChildren().push_back(
        TokenToNode<cmake::language::ElementType::Arguments>{m_script}.Build(childToken)
        );
      break;
    }
  }

  if (res->GetChildren().empty())
  {
    // Add empty Arguments node containing the parentheses.
    auto arguments = NodeFactory::CreateNode(BasicNode::Type::Arguments);
    auto argumentsRange = finalRange;
    argumentsRange.begin.column += res.GetAs<BasicNode::Type::CommandInvocation>().GetCommandName().size();
    arguments->SetRange(argumentsRange);
    res->GetChildren().push_back(std::move(arguments));
  }

  res->SetRange(finalRange);
  return res;
}
