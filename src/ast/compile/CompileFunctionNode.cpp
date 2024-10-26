#include <compile/ast/CompileHandler.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <compile/Symbol.h>
#include <compile/Module.h>
#include <codegen/FunctionBuilder.h>
#include <parse/declarations/FunctionNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <tokenize/SourceLocation.h>
#include <bind/Function.h>
#include <bind/FunctionType.h>
#include <utils/Array.hpp>

using namespace parse;
using namespace bind;
using namespace codegen;

namespace compile {
    void CompileHandler::visit(FunctionNode* node) {
        CompiledFunction* cf = m_ctx->getCurrentFunction();
        FunctionBuilder* fb = cf->getBuilder();
        m_ctx->enterNode(node);

        struct ParamInfo {
            String name;
            DataType* type;
            const SourceLocation& nameLoc;
        };

        Array<ParamInfo> params(node->parameters->parameters.size());
        Array<DataType*> paramTypes(params.size());
        for (u32 i = 0;i < node->parameters->parameters.size();i++) {
            TypedAssignableNode* param = node->parameters->parameters[i];

            param->type->acceptVisitor(this);
            DataType* argTp = m_resolvedTypeStack.pop();

            params.push({
                param->name->text,
                argTp,
                param->name->getLocation()
            });

            paramTypes.push(argTp);
        }

        node->returnType->acceptVisitor(this);
        DataType* retTp = m_resolvedTypeStack.pop();

        bool signatureExisted = false;
        FunctionType* sig = Registry::Signature(retTp, paramTypes.data(), paramTypes.size(), &signatureExisted);

        bool wasDefined = false;
        Symbol* found = nullptr;
        Function* decl = nullptr;

        Array<Symbol*> existing;
        cf->getCurrentScope()->findSymbols(node->name->text, existing);
        for (u32 i = 0;i < existing.size();i++) {
            Symbol* s = existing[i];
            if (s->getType() != Symbol::Type::Function) continue;

            Function* fn = s->getValue().func;
            if (!fn->getSignature()->isEqualTo(sig)) continue;

            if (s->hasDefinitionLocation()) {
                m_ctx->enterNode(node->name);
                m_ctx->logError(
                    "Function '%s' with signature '%s' has already been defined",
                    node->name->text.c_str(),
                    sig->getName().c_str()
                );
                m_ctx->logInfo("Previous definition is here");
                CompileMessage& ref = m_ctx->getLogs().last();
                ref.location = s->getDefinitionLocation();
                m_ctx->exitNode();
                wasDefined = true;
                break;                    
            }

            found = s;
            decl = fn;
        }

        if (!wasDefined) {
            if (!signatureExisted) m_ctx->onTypeGenerated(sig);

            if (!decl) {
                decl = new bind::Function(node->name->text, sig, m_ctx->getModule()->getNamespace());
                m_ctx->onFunctionGenerated(decl);
                found = m_ctx->getGlobalScope()->getSymbols().add(node->name->text, decl);
                found->setDeclarationLocation(node->name->getLocation());
            }

            if (node->body) {
                CompiledFunction* def = new CompiledFunction(m_ctx, decl);
                found->setDefinitionLocation(node->name->getLocation());

                m_ctx->enterFunction(def);
                node->body->acceptVisitor(this);
                m_ctx->exitFunction();
            }
        } else {
            if (!signatureExisted) {
                bind::Registry::Remove(sig);
                delete sig;
            }
        }

        m_ctx->exitNode();
    }
};