#include "BracketCommentASTBuilder.hpp"

#include "NodeFactory.hpp"


BasicNode::PolymorphicNode TokenToNode<cmake::language::ElementType::BracketComment>::Build(const cmake::language::Token& i_token)
{
  auto res = NodeFactory::CreateNode(BasicNode::Type::BracketComment);
  auto& commentNode = res.GetAs<BasicNode::Type::BracketComment>();


  // The content of the bracket comment is given by the children of the token.
  // It should have exactly 3 children: open, content and close.
  assert(i_token.children.size() == 3);
  const auto& content = i_token.children[1];

  commentNode.SetValue(this->GetString(content.range));

  commentNode.SetRange(this->ConvertRange(i_token.range));

  return res;
}
