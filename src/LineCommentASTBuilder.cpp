#include "LineCommentASTBuilder.hpp"

#include "NodeFactory.hpp"


BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::LineComment>::Build(const cmake::language::Token& i_token)
{
  auto res = NodeFactory::CreateNode(BasicNode::Type::Comment);
  auto& commentNode = res.GetAs<BasicNode::Type::Comment>();

  auto valueRange = i_token.range;
  ++valueRange.begin;
  commentNode.SetValue(this->GetString(valueRange));

  commentNode.SetRange(this->ConvertRange(i_token.range));

  return res;
}
