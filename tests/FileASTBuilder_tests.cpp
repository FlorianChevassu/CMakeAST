#include "FileASTBuilder.hpp"

#include "FileParser.hpp"
#include "Node.hpp"

#include "Token.hpp"
#include "Error.hpp"

#include "catch2/catch.hpp"
__pragma(warning(disable:6319))

using namespace cmake::language;

TEST_CASE("FileASTBuilder", "[ASTBuilder]")
{
  SECTION("One command")
  {
    std::string_view script = "identifier()";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::File>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::File> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::File);
    REQUIRE(node->GetChildren().size() == 1);
    REQUIRE(node->GetChildren()[0]->GetType() == BasicNode::Type::CommandInvocation);

    auto commandInvocationNode = node->GetChildren()[0].GetAs<BasicNode::Type::CommandInvocation>();
    REQUIRE(commandInvocationNode.GetCommandName() == "identifier");
  }

  SECTION("One comment")
  {
    std::string_view script = "# hello comment";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::File>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::File> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::File);
    REQUIRE(node->GetChildren().size() == 1);
    REQUIRE(node->GetChildren()[0]->GetType() == BasicNode::Type::Comment);

    auto commentNode = node->GetChildren()[0].GetAs<BasicNode::Type::Comment>();
    REQUIRE(commentNode.GetValue() == " hello comment");
    REQUIRE(commentNode.GetChildren().size() == 0);
  }

  SECTION("One command and one comment")
  {
    std::string_view script = "set() # hello comment";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::File>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::File> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::File);
    REQUIRE(node->GetChildren().size() == 2);
    REQUIRE(node->GetChildren()[0]->GetType() == BasicNode::Type::CommandInvocation);
    REQUIRE(node->GetChildren()[1]->GetType() == BasicNode::Type::Comment);

    auto commandInvocationNode = node->GetChildren()[0].GetAs<BasicNode::Type::CommandInvocation>();
    REQUIRE(commandInvocationNode.GetCommandName() == "set");

    auto commentNode = node->GetChildren()[1].GetAs<BasicNode::Type::Comment>();
    REQUIRE(commentNode.GetValue() == " hello comment");
    REQUIRE(commentNode.GetChildren().size() == 0);
  }

}
