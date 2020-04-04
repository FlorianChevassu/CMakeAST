#include "ArgumentsASTBuilder.hpp"

#include "ArgumentsParser.hpp"
#include "Token.hpp"
#include "Error.hpp"

#include "Node.hpp"

#include "catch2/catch.hpp"
__pragma(warning(disable:6319))

using namespace cmake::language;

TEST_CASE("ArgumentsASTBuilder", "[ASTBuilder]")
{
  SECTION("One argument")
  {
    std::string_view script = "arg";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::Arguments>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::Arguments> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::Arguments);
    REQUIRE(node->GetChildren().size() == 1);
    REQUIRE(node->GetChildren()[0]->GetType() == BasicNode::Type::Argument);

    auto& argumentNode = node->GetChildren()[0].GetAs<BasicNode::Type::Argument>();
    REQUIRE(argumentNode.IsQuoted() == false);
    REQUIRE(argumentNode.GetValue() == "arg");
  }

  SECTION("Multiple argument")
  {
    std::string_view script = "arg1 \"arg2\" arg3";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::Arguments>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::Arguments> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::Arguments);
    REQUIRE(node->GetChildren().size() == 3);

    {
      REQUIRE(node->GetChildren()[0]->GetType() == BasicNode::Type::Argument);
      auto& argumentNode = node->GetChildren()[0].GetAs<BasicNode::Type::Argument>();
      REQUIRE(argumentNode.IsQuoted() == false);
      REQUIRE(argumentNode.GetValue() == "arg1");
    }

    {
      REQUIRE(node->GetChildren()[1]->GetType() == BasicNode::Type::Argument);
      auto& argumentNode = node->GetChildren()[1].GetAs<BasicNode::Type::Argument>();
      REQUIRE(argumentNode.IsQuoted() == true);
      REQUIRE(argumentNode.GetValue() == "arg2");
    }

    {
      REQUIRE(node->GetChildren()[2]->GetType() == BasicNode::Type::Argument);
      auto& argumentNode = node->GetChildren()[2].GetAs<BasicNode::Type::Argument>();
      REQUIRE(argumentNode.IsQuoted() == false);
      REQUIRE(argumentNode.GetValue() == "arg3");
    }
  }

  SECTION("Sub-arguments")
  {
    std::string_view script = "arg1 (\"arg2\" arg3)";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::Arguments>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::Arguments> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::Arguments);
    REQUIRE(node->GetChildren().size() == 2);

    {
      REQUIRE(node->GetChildren()[0]->GetType() == BasicNode::Type::Argument);
      auto& argumentNode = node->GetChildren()[0].GetAs<BasicNode::Type::Argument>();
      REQUIRE(argumentNode.IsQuoted() == false);
      REQUIRE(argumentNode.GetValue() == "arg1");
    }

    REQUIRE(node->GetChildren()[1]->GetType() == BasicNode::Type::Arguments);
    auto& argumentsNode = node->GetChildren()[1].GetAs<BasicNode::Type::Arguments>();
    REQUIRE(argumentsNode.GetChildren().size() == 2);

    {
      REQUIRE(argumentsNode.GetChildren()[0]->GetType() == BasicNode::Type::Argument);
      auto& argumentNode = argumentsNode.GetChildren()[0].GetAs<BasicNode::Type::Argument>();
      REQUIRE(argumentNode.IsQuoted() == true);
      REQUIRE(argumentNode.GetValue() == "arg2");
    }

    {
      REQUIRE(argumentsNode.GetChildren()[1]->GetType() == BasicNode::Type::Argument);
      auto& argumentNode = argumentsNode.GetChildren()[1].GetAs<BasicNode::Type::Argument>();
      REQUIRE(argumentNode.IsQuoted() == false);
      REQUIRE(argumentNode.GetValue() == "arg3");
    }
  }
}
