#include "BracketCommentASTBuilder.hpp"

#include "LineCommentParser.hpp"
#include "Node.hpp"

#include "Token.hpp"
#include "Error.hpp"

#include "catch2/catch.hpp"
__pragma(warning(disable:6319))

using namespace cmake::language;

TEST_CASE("BracketCommentASTBuilder", "[ASTBuilder]")
{
  SECTION("Simple case")
  {
    std::string_view script = "#[[ this is a comment\n on 2 lines ]]";
    cmake::language::Range r{ script.begin(), script.end() };
    auto token = Parser<ElementType::BracketComment>{}.Parse(r);
    TokenToNode<cmake::language::ElementType::BracketComment> builder(script);
    auto node = builder.Build(token.value());
    REQUIRE(node);
    REQUIRE(node->GetType() == BasicNode::Type::BracketComment);

    auto commentNode = node.GetAs<BasicNode::Type::BracketComment>();
    REQUIRE(commentNode.GetValue() == " this is a comment\n on 2 lines ");
    REQUIRE(node->GetChildren().size() == 0);
  }
}
