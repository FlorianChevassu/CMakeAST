#include "FileASTBuilder.hpp"

#include "NodeFactory.hpp"

BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::File>::Build(const cmake::language::Token& i_token)
{
  auto res = NodeFactory::CreateNode(BasicNode::Type::File);
  this->BuildChildren(res, i_token);
  return res;
}
