#include "ASTBuilder.hpp"
#include "Node.hpp"

#include "catch2/catch.hpp"
__pragma(warning(disable:6319))

TEST_CASE("Visitor", "[Visitor]")
{
  SECTION("Const visitor")
  {
    std::string_view script = "cmake_minimum_required(VERSION 3.15)";
    const auto node = ASTBuilder::Build(script);

    class MyVisitor : public Visitor
    {
    public:
#define VISIT_CONST(NODETYPE) void Visit(const Node<BasicNode::Type::NODETYPE>& i_node) override { ++nb##NODETYPE; }
#define VISIT_NON_CONST(NODETYPE) void Visit(Node<BasicNode::Type::NODETYPE>& i_node) override {}
#define VISIT(NODETYPE) size_t nb##NODETYPE = 0;\
        VISIT_CONST(NODETYPE)\
        VISIT_NON_CONST(NODETYPE)

      VISIT(CommandInvocation)
        VISIT(Argument)
        VISIT(Arguments)
        VISIT(BracketComment)
        VISIT(Comment)
        VISIT(File)
    };
#undef VISIT_CONST
#undef VISIT_NON_CONST
#undef VISIT

    MyVisitor v;
    node->Accept(v);

    REQUIRE(v.nbCommandInvocation == 1);
    REQUIRE(v.nbArguments == 1);
    REQUIRE(v.nbArgument == 2);
    REQUIRE(v.nbBracketComment == 0);
    REQUIRE(v.nbComment == 0);
    REQUIRE(v.nbFile == 1);
  }

  SECTION("Non-const visitor")
  {
    std::string_view script = "cmake_minimum_required(VERSION 3.15)";
    auto node = ASTBuilder::Build(script);

    class MyVisitor : public Visitor
    {
    public:
#define VISIT_CONST(NODETYPE) void Visit(const Node<BasicNode::Type::NODETYPE>& i_node) override {}
#define VISIT_NON_CONST(NODETYPE) void Visit(Node<BasicNode::Type::NODETYPE>& i_node) override { ++nb##NODETYPE; }
#define VISIT(NODETYPE) size_t nb##NODETYPE = 0;\
        VISIT_CONST(NODETYPE)\
        VISIT_NON_CONST(NODETYPE)

      VISIT(CommandInvocation)
        VISIT(Argument)
        VISIT(Arguments)
        VISIT(BracketComment)
        VISIT(Comment)
        VISIT(File)
    };
#undef VISIT_CONST
#undef VISIT_NON_CONST
#undef VISIT

    MyVisitor v;
    node->Accept(v);

    REQUIRE(v.nbCommandInvocation == 1);
    REQUIRE(v.nbArguments == 1);
    REQUIRE(v.nbArgument == 2);
    REQUIRE(v.nbBracketComment == 0);
    REQUIRE(v.nbComment == 0);
    REQUIRE(v.nbFile == 1);
  }
}
