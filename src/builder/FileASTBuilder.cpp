#include "FileASTBuilder.hpp"

#include "NodeFactory.hpp"

BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::File>::Build(const cmake::language::Token& i_token)
{
  auto res = NodeFactory::CreateNode(BasicNode::Type::File);

  for (auto& fileElementToken : i_token.children)
  {
    assert(fileElementToken.type == cmake::language::ElementType::FileElement);

    for (auto& concreteToken : fileElementToken.children)
    {
      switch (concreteToken.type)
      {
      case cmake::language::ElementType::BracketComment:
      case cmake::language::ElementType::CommandInvocation:
        res->GetChildren().push_back(
          this->BuildNode(m_script, concreteToken, concreteToken.type)
          );
      case cmake::language::ElementType::LineEnding:
        for (auto& lineEndingChildToken : concreteToken.children)
        {
          if (lineEndingChildToken.type == cmake::language::ElementType::LineComment)
          {
            res->GetChildren().push_back(
              this->BuildNode(m_script, lineEndingChildToken, lineEndingChildToken.type)
              );
          }
        }
      }
    }
  }
  return res;
}
