#include "LineCommentASTBuilder.hpp"

#include "LineCommentParser.hpp"
#include "Node.hpp"

#include "Token.hpp"
#include "Error.hpp"

#include "catch2/catch.hpp"
__pragma(warning(disable:6319))

using namespace cmake::language;

TEST_CASE("LineCommentASTBuilder", "[ASTBuilder]")
{
  SECTION("Simple case")
  {
    std::string_view script = "# this is a comment";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::LineComment>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::LineComment> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::Comment);

    auto commentNode = node.GetAs<BasicNode::Type::Comment>();
    REQUIRE(commentNode.GetValue() == " this is a comment");
    REQUIRE(node->GetChildren().size() == 0);
  }
}
