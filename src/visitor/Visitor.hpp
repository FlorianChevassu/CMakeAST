#pragma once

#include "Node.hpp"

class Visitor
{
public:
  virtual ~Visitor() {}

  // CommandInvocation
  virtual void Visit(const Node<BasicNode::Type::CommandInvocation>& i_node) {};
  virtual void BeforeVisitChildren(const Node<BasicNode::Type::CommandInvocation>& i_node) {};
  virtual void AfterVisitChildren(const Node<BasicNode::Type::CommandInvocation>& i_node) {};

  virtual void Visit(Node<BasicNode::Type::CommandInvocation>& i_node) {};
  virtual void BeforeVisitChildren(Node<BasicNode::Type::CommandInvocation>& i_node) {};
  virtual void AfterVisitChildren(Node<BasicNode::Type::CommandInvocation>& i_node) {};

  // Argument
  virtual void Visit(const Node<BasicNode::Type::Argument>& i_node) {};
  virtual void BeforeVisitChildren(const Node<BasicNode::Type::Argument>& i_node) {};
  virtual void AfterVisitChildren(const Node<BasicNode::Type::Argument>& i_node) {};

  virtual void Visit(Node<BasicNode::Type::Argument>& i_node) {};
  virtual void BeforeVisitChildren(Node<BasicNode::Type::Argument>& i_node) {};
  virtual void AfterVisitChildren(Node<BasicNode::Type::Argument>& i_node) {};

  // Arguments
  virtual void Visit(const Node<BasicNode::Type::Arguments>& i_node) {};
  virtual void BeforeVisitChildren(const Node<BasicNode::Type::Arguments>& i_node) {};
  virtual void AfterVisitChildren(const Node<BasicNode::Type::Arguments>& i_node) {};

  virtual void Visit(Node<BasicNode::Type::Arguments>& i_node) {};
  virtual void BeforeVisitChildren(Node<BasicNode::Type::Arguments>& i_node) {};
  virtual void AfterVisitChildren(Node<BasicNode::Type::Arguments>& i_node) {};

  // BracketComment
  virtual void Visit(const Node<BasicNode::Type::BracketComment>& i_node) {};
  virtual void BeforeVisitChildren(const Node<BasicNode::Type::BracketComment>& i_node) {};
  virtual void AfterVisitChildren(const Node<BasicNode::Type::BracketComment>& i_node) {};

  virtual void Visit(Node<BasicNode::Type::BracketComment>& i_node) {};
  virtual void BeforeVisitChildren(Node<BasicNode::Type::BracketComment>& i_node) {};
  virtual void AfterVisitChildren(Node<BasicNode::Type::BracketComment>& i_node) {};

  // Comment
  virtual void Visit(const Node<BasicNode::Type::Comment>& i_node) {};
  virtual void BeforeVisitChildren(const Node<BasicNode::Type::Comment>& i_node) {};
  virtual void AfterVisitChildren(const Node<BasicNode::Type::Comment>& i_node) {};

  virtual void Visit(Node<BasicNode::Type::Comment>& i_node) {};
  virtual void BeforeVisitChildren(Node<BasicNode::Type::Comment>& i_node) {};
  virtual void AfterVisitChildren(Node<BasicNode::Type::Comment>& i_node) {};

  // File
  virtual void Visit(const Node<BasicNode::Type::File>& i_node) {};
  virtual void BeforeVisitChildren(const Node<BasicNode::Type::File>& i_node) {};
  virtual void AfterVisitChildren(const Node<BasicNode::Type::File>& i_node) {};

  virtual void Visit(Node<BasicNode::Type::File>& i_node) {};
  virtual void BeforeVisitChildren(Node<BasicNode::Type::File>& i_node) {};
  virtual void AfterVisitChildren(Node<BasicNode::Type::File>& i_node) {};
};
