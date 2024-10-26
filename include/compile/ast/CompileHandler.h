#pragma once
#include <compile/types.h>
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace bind {
    class DataType;
};

namespace compile {
    class Context;
    class CompileHandler : public parse::INodeVisitor {
        public:
            CompileHandler(Context* ctx);
            virtual ~CompileHandler();

            virtual void visit(parse::ArrayLiteralNode* node);
            virtual void visit(parse::ArrayTypeNode* node);
            virtual void visit(parse::ArrowFunctionExpressionNode* node);
            virtual void visit(parse::AwaitExpressionNode* node);
            virtual void visit(parse::BinaryExpressionNode* node);
            virtual void visit(parse::BooleanLiteralNode* node);
            virtual void visit(parse::BreakStatementNode* node);
            virtual void visit(parse::CallExpressionNode* node);
            virtual void visit(parse::CastExpressionNode* node);
            virtual void visit(parse::CatchStatementNode* node);
            virtual void visit(parse::ClassConstructorNode* node);
            virtual void visit(parse::ClassDestructorNode* node);
            virtual void visit(parse::ClassMethodNode* node);
            virtual void visit(parse::ClassNode* node);
            virtual void visit(parse::ClassOperatorMethodNode* node);
            virtual void visit(parse::ClassPropertyGetterNode* node);
            virtual void visit(parse::ClassPropertyNode* node);
            virtual void visit(parse::ClassPropertySetterNode* node);
            virtual void visit(parse::ConditionalExpressionNode* node);
            virtual void visit(parse::ContinueStatementNode* node);
            virtual void visit(parse::DeclarationStatementNode* node);
            virtual void visit(parse::DeleteStatementNode* node);
            virtual void visit(parse::DoWhileLoopNode* node);
            virtual void visit(parse::EmptyStatementNode* node);
            virtual void visit(parse::EnumFieldNode* node);
            virtual void visit(parse::EnumNode* node);
            virtual void visit(parse::ExportStatementNode* node);
            virtual void visit(parse::ExpressionNode* node);
            virtual void visit(parse::ExpressionSequenceNode* node);
            virtual void visit(parse::ExpressionStatementNode* node);
            virtual void visit(parse::ForLoopNode* node);
            virtual void visit(parse::FunctionExpressionNode* node);
            virtual void visit(parse::FunctionNode* node);
            virtual void visit(parse::FunctionTypeNode* node);
            virtual void visit(parse::IdentifierNode* node);
            virtual void visit(parse::IdentifierTypeSpecifierNode* node);
            virtual void visit(parse::IfStatementNode* node);
            virtual void visit(parse::IndexExpressionNode* node);
            virtual void visit(parse::ImportAllStatementNode* node);
            virtual void visit(parse::ImportSelectStatementNode* node);
            virtual void visit(parse::MemberExpressionNode* node);
            virtual void visit(parse::ModuleNode* node);
            virtual void visit(parse::NewExpressionNode* node);
            virtual void visit(parse::NullLiteralNode* node);
            virtual void visit(parse::NumberLiteralNode* node);
            virtual void visit(parse::ObjectLiteralNode* node);
            virtual void visit(parse::ObjectLiteralPropertyNode* node);
            virtual void visit(parse::ObjectTypeNode* node);
            virtual void visit(parse::ParameterListNode* node);
            virtual void visit(parse::ReturnStatementNode* node);
            virtual void visit(parse::SizeOfExpressionNode* node);
            virtual void visit(parse::StatementBlockNode* node);
            virtual void visit(parse::StatementNode* node);
            virtual void visit(parse::StringLiteralNode* node);
            virtual void visit(parse::SwitchCaseNode* node);
            virtual void visit(parse::SwitchStatementNode* node);
            virtual void visit(parse::TemplateStringLiteralNode* node);
            virtual void visit(parse::ThisExpressionNode* node);
            virtual void visit(parse::ThrowStatementNode* node);
            virtual void visit(parse::TopLevelNode* node);
            virtual void visit(parse::TryCatchStatementNode* node);
            virtual void visit(parse::TypedAssignableNode* node);
            virtual void visit(parse::TypeInfoExpressionNode* node);
            virtual void visit(parse::TypeNode* node);
            virtual void visit(parse::TypeParameterListNode* node);
            virtual void visit(parse::TypeParameterNode* node);
            virtual void visit(parse::TypeSpecifierNode* node);
            virtual void visit(parse::UnaryExpressionNode* node);
            virtual void visit(parse::WhileLoopNode* node);
        
        protected:
            friend class Context;
            Context* m_ctx;

            Array<bind::DataType*> m_resolvedTypeStack;
    };
};