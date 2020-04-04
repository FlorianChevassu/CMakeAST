#pragma once

#include "Context.hpp"

#include "polymorphic_value.h"

#include <string>
#include <vector>

class Visitor;

class BasicNode
{
public:
  virtual ~BasicNode() = default;

  enum class Type
  {
    CommandInvocation, // set(a b c)
    Argument, // a, "a"
    Arguments, // (a b c)
    BracketComment, // #[[ comment ]]
    Comment, // # comment
    File
  };

  virtual BasicNode* Clone() const = 0;
  virtual Type GetType() const = 0;

  Range& GetRange()
  {
    return m_range;
  }

  const Range& GetRange() const
  {
    return m_range;
  }

  void SetRange(Range i_range)
  {
    m_range = i_range;
  }

  virtual void Accept(Visitor& i_visitor) = 0;
  virtual void Accept(Visitor& i_visitor) const = 0;

protected:
  template <typename T>
  friend void AcceptImpl(const T& self, Visitor& i_visitor);

public:
  struct NodeCopy;
  struct PolymorphicNode;

  std::vector<PolymorphicNode>& GetChildren()
  {
    return m_children;
  }

  const std::vector<PolymorphicNode>& GetChildren() const
  {
    return m_children;
  }

private:
  Range m_range;
  std::vector<PolymorphicNode> m_children;
};

template <class Derived>
class NodeBase : public BasicNode
{
private:
  Derived& GetThis() { return static_cast<Derived&>(*this); }
  const Derived& GetThis() const { return static_cast<const Derived&>(*this); }

public:
  BasicNode* Clone() const override
  {
    return new Derived(GetThis());
  }

  BasicNode::Type GetType() const override
  {
    return Derived::NodeType;
  }
};

template <BasicNode::Type T>
class Node : public NodeBase<Node<T>>
{
public:
  static const BasicNode::Type NodeType = T;

  void Accept(Visitor& i_visitor) override
  {
    return AcceptImpl(*this, i_visitor);
  }

  void Accept(Visitor& i_visitor) const override
  {
    return AcceptImpl(*this, i_visitor);
  }
};

struct BasicNode::PolymorphicNode : public jbcoe::polymorphic_value<BasicNode>
{
private:
  struct NodeCopy
  {
    BasicNode* operator()(const BasicNode& i_other) const
    {
      return i_other.Clone();
    }
  };

public:
  PolymorphicNode(BasicNode* i_ptr = nullptr)
    : jbcoe::polymorphic_value<BasicNode>(i_ptr, NodeCopy{})
  {
  }

  template <BasicNode::Type T >
  Node<T>& GetAs()
  {
    return dynamic_cast<Node<T>&>(**this);
  }

  template <BasicNode::Type T>
  const Node<T>& GetAs() const
  {
    return dynamic_cast<const Node<T>&>(**this);
  }
};


#include "Node.tpp"
