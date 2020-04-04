#pragma once
#include "Node.hpp"

class NodeFactory
{
public:
  template <BasicNode::Type T>
  static BasicNode::PolymorphicNode CreateNode();

  static BasicNode::PolymorphicNode CreateNode(BasicNode::Type i_type);
};
