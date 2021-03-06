#pragma once
#include "NodeFactory.hpp"

template <BasicNode::Type T>
BasicNode::PolymorphicNode NodeFactory::CreateNode()
{
  return new Node<T>();
}

BasicNode::PolymorphicNode NodeFactory::CreateNode(BasicNode::Type i_type)
{
  switch (i_type)
  {
  case BasicNode::Type::Argument:
    return NodeFactory::CreateNode<BasicNode::Type::Argument>();
  case BasicNode::Type::Arguments:
    return NodeFactory::CreateNode<BasicNode::Type::Arguments>();
  case BasicNode::Type::BracketComment:
    return NodeFactory::CreateNode<BasicNode::Type::BracketComment>();
  case BasicNode::Type::CommandInvocation:
    return NodeFactory::CreateNode<BasicNode::Type::CommandInvocation>();
  case BasicNode::Type::Comment:
    return NodeFactory::CreateNode<BasicNode::Type::Comment>();
  case BasicNode::Type::File:
    return NodeFactory::CreateNode<BasicNode::Type::File>();
  default:
    // Node type not supported...
    assert(false);
  }

  return {};
}
