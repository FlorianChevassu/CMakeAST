#include "Visitor.hpp"

template <typename T>
void AcceptImpl(T& self, Visitor& i_visitor)
{
  i_visitor.Visit(self);

  i_visitor.BeforeVisitChildren(self);
  for (auto& child : self.GetChildren())
  {
    child->Accept(i_visitor);
  }
  i_visitor.AfterVisitChildren(self);
}

template <typename T>
void AcceptImpl(const T& self, Visitor& i_visitor)
{
  i_visitor.Visit(self);

  i_visitor.BeforeVisitChildren(self);
  for (auto& child : self.GetChildren())
  {
    child->Accept(i_visitor);
  }
  i_visitor.AfterVisitChildren(self);
}


// We can create specific implementations if we need to store
// aditional information. For example:
template <>
class Node<BasicNode::Type::File>
  : public NodeBase<Node<BasicNode::Type::File>>
{
public:
  static const BasicNode::Type NodeType = BasicNode::Type::File;
  
  Node(std::string i_filePath = "") : m_filepath(i_filePath)
  {

  }
  const std::string& GetFilePath() const { return m_filepath; }


  void Accept(Visitor& i_visitor) override
  {
    return AcceptImpl(*this, i_visitor);
  }

  void Accept(Visitor& i_visitor) const override
  {
    return AcceptImpl(*this, i_visitor);
  }

private:
  std::string m_filepath;
};

template <>
class Node<BasicNode::Type::CommandInvocation>
  : public NodeBase<Node<BasicNode::Type::CommandInvocation>>
{
public:
  static const BasicNode::Type NodeType = BasicNode::Type::CommandInvocation;

  Node(std::string i_commandName = "") : m_commandName(i_commandName)
  {
  }
  const std::string& GetCommandName() const { return m_commandName; }
  void SetCommandName(std::string i_commandName) { m_commandName = i_commandName; }

  void Accept(Visitor& i_visitor) override
  {
    return AcceptImpl(*this, i_visitor);
  }

  void Accept(Visitor& i_visitor) const override
  {
    return AcceptImpl(*this, i_visitor);
  }
private:
  std::string m_commandName;
};

template <>
class Node<BasicNode::Type::Argument>
  : public NodeBase<Node<BasicNode::Type::Argument>>
{
public:
  static const BasicNode::Type NodeType = BasicNode::Type::Argument;

  bool IsQuoted() const { return m_isQuoted; }
  void SetIsQuoted(bool i_isQuoted) { m_isQuoted = i_isQuoted; }

  bool IsBracket() const { return m_isBracket; }
  void SetIsBracket(bool i_isBracket) { m_isBracket = i_isBracket; }

  std::string GetValue() const { return m_value; }
  void SetValue(const std::string& i_value) { m_value = i_value; }


  void Accept(Visitor& i_visitor) override
  {
    return AcceptImpl(*this, i_visitor);
  }

  void Accept(Visitor& i_visitor) const override
  {
    return AcceptImpl(*this, i_visitor);
  }

private:
  bool m_isQuoted = false;
  bool m_isBracket = false;
  std::string m_value;
};

template <>
class Node<BasicNode::Type::Comment>
  : public NodeBase<Node<BasicNode::Type::Comment>>
{
public:
  static const BasicNode::Type NodeType = BasicNode::Type::Comment;

  std::string GetValue() const { return m_value; }
  void SetValue(const std::string& i_value) { m_value = i_value; }


  void Accept(Visitor& i_visitor) override
  {
    return AcceptImpl(*this, i_visitor);
  }

  void Accept(Visitor& i_visitor) const override
  {
    return AcceptImpl(*this, i_visitor);
  }

private:
  std::string m_value;
};

template <>
class Node<BasicNode::Type::BracketComment>
  : public NodeBase<Node<BasicNode::Type::BracketComment>>
{
public:
  static const BasicNode::Type NodeType = BasicNode::Type::BracketComment;

  std::string GetValue() const { return m_value; }
  void SetValue(const std::string& i_value) { m_value = i_value; }


  void Accept(Visitor& i_visitor) override
  {
    return AcceptImpl(*this, i_visitor);
  }

  void Accept(Visitor& i_visitor) const override
  {
    return AcceptImpl(*this, i_visitor);
  }

private:
  std::string m_value;
};
