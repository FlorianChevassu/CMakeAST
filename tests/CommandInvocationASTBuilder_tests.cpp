#include "CommandInvocationASTBuilder.hpp"

#include "CommandInvocationParser.hpp"
#include "Node.hpp"

#include "Token.hpp"
#include "Error.hpp"

#include "catch2/catch.hpp"
__pragma(warning(disable:6319))

using namespace cmake::language;

TEST_CASE("CommandInvocationASTBuilder", "[ASTBuilder]")
{
  SECTION("With no arguments")
  {
    std::string_view script = "identifier()";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::CommandInvocation>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::CommandInvocation> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::CommandInvocation);
    auto commandInvocationNode = node.GetAs<BasicNode::Type::CommandInvocation>();
    REQUIRE(commandInvocationNode.GetCommandName() == "identifier");
    REQUIRE(node->GetChildren().size() == 0);
  }

  SECTION("With arguments")
  {
    std::string_view script = "identifier(arg1 arg2)";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::CommandInvocation>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::CommandInvocation> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::CommandInvocation);

    auto commandInvocationNode = node.GetAs<BasicNode::Type::CommandInvocation>();
    REQUIRE(commandInvocationNode.GetCommandName() == "identifier");
    REQUIRE(commandInvocationNode.GetChildren().size() == 1);
    REQUIRE(commandInvocationNode.GetChildren()[0]->GetType() == BasicNode::Type::Arguments);
  }
}
