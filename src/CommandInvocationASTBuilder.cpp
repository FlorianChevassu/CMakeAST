#include "CommandInvocationASTBuilder.hpp"

#include "NodeFactory.hpp"

BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::CommandInvocation>::Build(const cmake::language::Token& i_token)
{
  auto res = NodeFactory::CreateNode(BasicNode::Type::CommandInvocation);

  // A CommandInvocation has
  //  - An Identifier token
  //  - Optionally an Arguments token.
  for (auto& childToken : i_token.children)
  {
    switch (childToken.type)
    {
    case cmake::language::ElementType::Identifier:
      res.GetAs<BasicNode::Type::CommandInvocation>().SetCommandName(GetString(childToken.range));
      break;
    case cmake::language::ElementType::Arguments:
      res->GetChildren().push_back(
        TokenToNode<cmake::language::ElementType::Arguments>{m_script}.Build(childToken)
        );
      break;
    }
  }

  res->SetRange(this->ConvertRange(i_token.range));
  return res;
}
