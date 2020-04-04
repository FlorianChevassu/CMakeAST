#include "FileElementASTBuilder.hpp"

#include "NodeFactory.hpp"

BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::FileElement>::Build(const cmake::language::Token& i_token)
{
  return this->BuildClassicNode<BasicNode::Type::FileElement>(i_token);
}
