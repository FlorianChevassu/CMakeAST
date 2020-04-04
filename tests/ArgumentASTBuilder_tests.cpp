#include "ArgumentASTBuilder.hpp"

#include "ArgumentParser.hpp"
#include "QuotedArgumentParser.hpp"
#include "Token.hpp"
#include "Error.hpp"

#include "Node.hpp"

#include "catch2/catch.hpp"
__pragma(warning(disable:6319))

using namespace cmake::language;

TEST_CASE("ArgumentASTBuilder", "[ASTBuilder]")
{
  SECTION("Simple argument")
  {
    std::string_view script = "arg";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::Argument>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::Argument> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::Argument);
    auto argumentNode = node.GetAs<BasicNode::Type::Argument>();
    REQUIRE(argumentNode.GetValue() == "arg");
    REQUIRE(argumentNode.IsBracket() == false);
    REQUIRE(argumentNode.IsQuoted() == false);
    REQUIRE(node->GetChildren().size() == 0);
  }

  SECTION("Quoted argument")
  {
    std::string_view script = "\"arg\"";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::Argument>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::Argument> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::Argument);
    auto argumentNode = node.GetAs<BasicNode::Type::Argument>();
    REQUIRE(argumentNode.GetValue() == "arg");
    REQUIRE(argumentNode.IsBracket() == false);
    REQUIRE(argumentNode.IsQuoted() == true);
    REQUIRE(node->GetChildren().size() == 0);
  }

  SECTION("Bracket argument")
  {
    std::string_view script = "[=[arg]=]";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::Argument>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::Argument> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::Argument);
    auto argumentNode = node.GetAs<BasicNode::Type::Argument>();
    REQUIRE(argumentNode.GetValue() == "arg");
    REQUIRE(argumentNode.IsBracket() == true);
    REQUIRE(argumentNode.IsQuoted() == false);
    REQUIRE(node->GetChildren().size() == 0);
  }

  SECTION("Simple argument with variable dereferencing")
  {
    std::string_view script = "arg${var}";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::Argument>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::Argument> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::Argument);
    auto argumentNode = node.GetAs<BasicNode::Type::Argument>();
    REQUIRE(argumentNode.GetValue() == "arg${var}");
    REQUIRE(node->GetChildren().size() == 0);
  }
}
