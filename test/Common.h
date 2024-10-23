#include <catch2/catch_test_macros.hpp>
#include <parse/Context.h>
#include <parse/Tokens.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/types.h>
#include <tokenize/TokenizedSource.h>
#include <tokenize/Resource.h>
#include <tokenize/TokenSet.h>
#include <utils/String.h>

using namespace parse;

CATCH_REGISTER_ENUM(NodeType,
    NodeType::Node,
    NodeType::ArrayLiteralNode,
    NodeType::ArrayTypeNode,
    NodeType::ArrowFunctionExpressionNode,
    NodeType::AwaitExpressionNode,
    NodeType::BinaryExpressionNode,
    NodeType::BooleanLiteralNode,
    NodeType::BreakStatementNode,
    NodeType::CallExpressionNode,
    NodeType::CastExpressionNode,
    NodeType::CatchStatementNode,
    NodeType::ClassConstructorNode,
    NodeType::ClassDestructorNode,
    NodeType::ClassMethodNode,
    NodeType::ClassNode,
    NodeType::ClassOperatorMethodNode,
    NodeType::ClassPropertyGetterNode,
    NodeType::ClassPropertyNode,
    NodeType::ClassPropertySetterNode,
    NodeType::ConditionalExpressionNode,
    NodeType::ContinueStatementNode,
    NodeType::DeclarationStatementNode,
    NodeType::DeleteStatementNode,
    NodeType::DoWhileLoopNode,
    NodeType::EmptyStatementNode,
    NodeType::EnumFieldNode,
    NodeType::EnumNode,
    NodeType::ExportStatementNode,
    NodeType::ExpressionNode,
    NodeType::ExpressionSequenceNode,
    NodeType::ExpressionStatementNode,
    NodeType::ForLoopNode,
    NodeType::FunctionExpressionNode,
    NodeType::FunctionNode,
    NodeType::FunctionTypeNode,
    NodeType::IdentifierNode,
    NodeType::IdentifierTypeSpecifierNode,
    NodeType::IfStatementNode,
    NodeType::ImportAllStatementNode,
    NodeType::ImportSelectStatementNode,
    NodeType::IndexExpressionNode,
    NodeType::MemberExpressionNode,
    NodeType::ModuleNode,
    NodeType::NewExpressionNode,
    NodeType::NullLiteralNode,
    NodeType::NumberLiteralNode,
    NodeType::ObjectLiteralNode,
    NodeType::ObjectLiteralPropertyNode,
    NodeType::ObjectTypeNode,
    NodeType::ParameterListNode,
    NodeType::ReturnStatementNode,
    NodeType::SizeOfExpressionNode,
    NodeType::StatementBlockNode,
    NodeType::StatementNode,
    NodeType::StringLiteralNode,
    NodeType::SwitchCaseNode,
    NodeType::SwitchStatementNode,
    NodeType::TemplateStringLiteralNode,
    NodeType::ThisExpressionNode,
    NodeType::ThrowStatementNode,
    NodeType::TopLevelNode,
    NodeType::TryCatchStatementNode,
    NodeType::TypedAssignableNode,
    NodeType::TypeInfoExpressionNode,
    NodeType::TypeNode,
    NodeType::TypeParameterListNode,
    NodeType::TypeParameterNode,
    NodeType::TypeSpecifierNode,
    NodeType::UnaryExpressionNode,
    NodeType::WhileLoopNode
);

CATCH_REGISTER_ENUM(OperatorType,
    OperatorType::Invalid,
    OperatorType::Add,
    OperatorType::Sub,
    OperatorType::Mul,
    OperatorType::Div,
    OperatorType::Mod,
    OperatorType::Exp,
    OperatorType::AddEq,
    OperatorType::SubEq,
    OperatorType::MulEq,
    OperatorType::DivEq,
    OperatorType::ModEq,
    OperatorType::ExpEq,
    OperatorType::BitwiseAnd,
    OperatorType::BitwiseOr,
    OperatorType::BitwiseXOr,
    OperatorType::BitwiseInvert,
    OperatorType::ShiftLeft,
    OperatorType::ShiftRight,
    OperatorType::LogicalAnd,
    OperatorType::LogicalOr,
    OperatorType::BitwiseAndEq,
    OperatorType::BitwiseOrEq,
    OperatorType::BitwiseXOrEq,
    OperatorType::ShiftLeftEq,
    OperatorType::ShiftRightEq,
    OperatorType::LogicalAndEq,
    OperatorType::LogicalOrEq,
    OperatorType::Not,
    OperatorType::Assign,
    OperatorType::Equality,
    OperatorType::Inequality,
    OperatorType::LessThan,
    OperatorType::LessThanEq,
    OperatorType::GreaterThan,
    OperatorType::GreaterThanEq,
    OperatorType::Dereference,
    OperatorType::Call,
    OperatorType::Index,
    OperatorType::Cast,
    OperatorType::Negate,
    OperatorType::PreIncrement,
    OperatorType::PostIncrement,
    OperatorType::PreDecrement,
    OperatorType::PostDecrement
);

CATCH_REGISTER_ENUM(TypeSpecifierNode::Modifier,
    TypeSpecifierNode::Modifier::None,
    TypeSpecifierNode::Modifier::Array,
    TypeSpecifierNode::Modifier::Pointer
);

namespace Catch {
    template<>
    struct StringMaker<utils::String> {
        static std::string convert( utils::String const& value ) {
            return utils::String::Format("\"%s\"", value.c_str());
        }
    };
}

void setupTest();

class TestContext {
    public:
        TestContext(const char* source);
        ~TestContext();

        parse::Context* ctx;
        tokenize::TokenSet* tokenSet;
        tokenize::Resource* src;
        tokenize::TokenizedSource* tokenizedSrc;
};