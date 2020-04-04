#include "ASTBuilder.hpp"
#include "Node.hpp"

#include "catch2/catch.hpp"
__pragma(warning(disable:6319))

TEST_CASE("ASTBuilder", "[ASTBuilder]")
{
  SECTION("EmptyFile")
  {
    std::string_view script = "";
    REQUIRE_THROWS(ASTBuilder::Build(script));
  }

  SECTION("One command")
  {
    std::string_view script = "cmake_minimum_required(VERSION 3.15)";
    auto node = ASTBuilder::Build(script);
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::File);
    REQUIRE(node->GetChildren().size() == 1);

    auto& commandInvocationPtr = node->GetChildren()[0];
    REQUIRE(commandInvocationPtr->GetType() == BasicNode::Type::CommandInvocation);
    auto& range = commandInvocationPtr->GetRange();
    REQUIRE(range.begin.line == 1);
    REQUIRE(range.begin.column == 1);
    REQUIRE(range.end.line == 1);
    REQUIRE(range.end.column == 37);

    Node<BasicNode::Type::CommandInvocation>& commandInvocation = commandInvocationPtr.GetAs<BasicNode::Type::CommandInvocation>();
    REQUIRE(commandInvocation.GetCommandName() == "cmake_minimum_required");
  }


  SECTION("One command with things before and after")
  {
    std::string_view script = "  cmake_minimum_required(VERSION 3.15)  # end line comment";
    auto node = ASTBuilder::Build(script);
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::File);
    REQUIRE(node->GetChildren().size() == 2);

    {
      auto& commandInvocation = node->GetChildren()[0];
      REQUIRE(commandInvocation->GetType() == BasicNode::Type::CommandInvocation);
      auto& range = commandInvocation->GetRange();
      REQUIRE(range.begin.line == 1);
      REQUIRE(range.begin.column == 3);
      REQUIRE(range.end.line == 1);
      REQUIRE(range.end.column == 39);
    }

    {
      auto& comment = node->GetChildren()[1];
      REQUIRE(comment->GetType() == BasicNode::Type::Comment);
      auto& range = comment->GetRange();
      REQUIRE(range.begin.line == 1);
      REQUIRE(range.begin.column == 41);
      REQUIRE(range.end.line == 1);
      REQUIRE(range.end.column == 59);
    }
  }

  SECTION("Multi-line command")
  {
    std::string_view script = "set(a\n val)";
    auto node = ASTBuilder::Build(script);
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::File);
    REQUIRE(node->GetChildren().size() == 1);

    auto& commandInvocation = node->GetChildren()[0];
    REQUIRE(commandInvocation->GetType() == BasicNode::Type::CommandInvocation);
    auto& range = commandInvocation->GetRange();
    REQUIRE(range.begin.line == 1);
    REQUIRE(range.begin.column == 1);
    REQUIRE(range.end.line == 2);
    REQUIRE(range.end.column == 6);
  }

  SECTION("Bracket comment")
  {
    std::string_view script = "#[==[ text \n on \n multiple \n lines ]==]";
    auto node = ASTBuilder::Build(script);
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::File);
    REQUIRE(node->GetChildren().size() == 1);

    auto& bracketComment = node->GetChildren()[0];
    REQUIRE(bracketComment->GetType() == BasicNode::Type::BracketComment);
    auto& range = bracketComment->GetRange();
    REQUIRE(range.begin.line == 1);
    REQUIRE(range.begin.column == 1);
    REQUIRE(range.end.line == 4);
    REQUIRE(range.end.column == 12);
  }

  SECTION("Simple argument")
  {
    std::string_view script = "set(a)";
    auto node = ASTBuilder::Build(script);
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::File);
    REQUIRE(node->GetChildren().size() == 1);

    auto& commandInvocation = node->GetChildren()[0];
    REQUIRE(commandInvocation->GetType() == BasicNode::Type::CommandInvocation);
    REQUIRE(commandInvocation->GetChildren().size() == 1);

    auto& arguments = commandInvocation->GetChildren()[0];
    REQUIRE(arguments->GetType() == BasicNode::Type::Arguments);
    REQUIRE(arguments->GetChildren().size() == 1);

    auto& argument = arguments->GetChildren()[0];
    REQUIRE(argument->GetType() == BasicNode::Type::Argument);
    auto& argumentNode = argument.GetAs<BasicNode::Type::Argument>();
    REQUIRE(argumentNode.IsQuoted() == false);
    REQUIRE(argumentNode.IsBracket() == false);
    REQUIRE(argumentNode.GetValue() == "a");

    auto& range = argument->GetRange();
    REQUIRE(range.begin.line == 1);
    REQUIRE(range.begin.column == 5);
    REQUIRE(range.end.line == 1);
    REQUIRE(range.end.column == 6);
  }


  SECTION("Multiple argument")
  {
    std::string_view script = "set(a b (c d))";
    auto node = ASTBuilder::Build(script);
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::File);
    REQUIRE(node->GetChildren().size() == 1);

    auto& commandInvocation = node->GetChildren()[0];
    REQUIRE(commandInvocation->GetType() == BasicNode::Type::CommandInvocation);
    REQUIRE(commandInvocation->GetChildren().size() == 1);

    auto& arguments = commandInvocation->GetChildren()[0];
    REQUIRE(arguments->GetType() == BasicNode::Type::Arguments);
    REQUIRE(arguments->GetChildren().size() == 3);

    {
      // First argument
      auto& argument = arguments->GetChildren()[0];
      REQUIRE(argument->GetType() == BasicNode::Type::Argument);

      auto& argumentNode = argument.GetAs<BasicNode::Type::Argument>();
      REQUIRE(argumentNode.IsQuoted() == false);
      REQUIRE(argumentNode.IsBracket() == false);
      REQUIRE(argumentNode.GetValue() == "a");

      auto& range = argument->GetRange();
      REQUIRE(range.begin.line == 1);
      REQUIRE(range.begin.column == 5);
      REQUIRE(range.end.line == 1);
      REQUIRE(range.end.column == 6);
    }

    {
      // Second argument
      auto& argument = arguments->GetChildren()[1];
      REQUIRE(argument->GetType() == BasicNode::Type::Argument);
      auto& argumentNode = argument.GetAs<BasicNode::Type::Argument>();
      REQUIRE(argumentNode.IsQuoted() == false);
      REQUIRE(argumentNode.IsBracket() == false);
      REQUIRE(argumentNode.GetValue() == "b");

      auto& range = argument->GetRange();
      REQUIRE(range.begin.line == 1);
      REQUIRE(range.begin.column == 7);
      REQUIRE(range.end.line == 1);
      REQUIRE(range.end.column == 8);
    }

    {
      // third argument : Arguments
      auto& argument = arguments->GetChildren()[2];
      REQUIRE(argument->GetType() == BasicNode::Type::Arguments);
      REQUIRE(argument->GetChildren().size() == 2);


      auto& range = argument->GetRange();
      REQUIRE(range.begin.line == 1);
      REQUIRE(range.begin.column == 9);
      REQUIRE(range.end.line == 1);
      REQUIRE(range.end.column == 14);
    }
  }

  SECTION("Bad script throws")
  {
    std::string_view script = "set(";
    REQUIRE_THROWS(ASTBuilder::Build(script));
  }
}
