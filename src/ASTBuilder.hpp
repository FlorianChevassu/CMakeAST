#pragma once
#include "Node.hpp"
#include "ElementType.hpp"
#include "Token.hpp"

class ASTBuilder
{
public:
  ASTBuilder(const std::string& i_fileContent);
  BasicNode::PolymorphicNode Build();

protected:
  template <cmake::language::ElementType TokenType>
  void BuildNode(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token);
  void BuildNode(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token, cmake::language::ElementType i_tokenType);
  void BuildChildren(BasicNode::PolymorphicNode& i_node, const cmake::language::Token& i_token);

  template <BasicNode::Type NT>
  void BuildClassicNode(BasicNode::PolymorphicNode& i_parent, const cmake::language::Token& i_token);

  Range ConvertRange(const cmake::language::Range& i_range);
private:
  const std::string& m_fileContent;
};
