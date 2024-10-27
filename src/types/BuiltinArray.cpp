#include <compile/types/BuiltinArray.h>
#include <compile/TypeSpecializer.hpp>
#include <compile/TypeArgument.h>
#include <codegen/Value.h>
#include <codegen/FunctionBuilder.h>
#include <bind/Registry.hpp>
#include <utils/Array.hpp>

using namespace bind;
using namespace codegen;

namespace compile {
    BuiltinArray::BuiltinArray() : TemplateType("Array", 1) {
    }

    BuiltinArray::~BuiltinArray() {
    }

    bool BuiltinArray::onSpecialization(TypeSpecializer* specializer) {
        DataType* voidTp = Registry::GetType<void>();
        DataType* elemTp = specializer->getTypeArguments()[0].getType();

        specializer->addProperty("m_used", Registry::GetType<u32>()).accessFlags = 0;
        specializer->addProperty("m_capacity", Registry::GetType<u32>()).accessFlags = 0;
        specializer->addProperty("m_data", Registry::GetType<void*>()).accessFlags = 0;
        specializer->addMethod(
            "push",
            Registry::MethodSignature(voidTp, specializer->getSpecializedType(), { elemTp }),
            &BuiltinArray::push
        );

        return true;
    }

    void BuiltinArray::push(codegen::FunctionBuilder* fb) {
        Value self = fb->getThis();
        Value ele = fb->getArg(0);
        Value usedPtr = fb->ptrOffset(self, 0, Registry::GetType<u32>());
        Value capacityPtr = fb->ptrOffset(self, 4, Registry::GetType<u32>());
        Value dataPtr = fb->ptrOffset(self, 8, Registry::GetType<u32>());

        Value used = *usedPtr;
        Value capacity = *capacityPtr;

        fb->generateIf(!capacity, [&]() {
            // allocate array
        });

        fb->generateIf(used == capacity, [&]() {
            // expand array
            Value newCapacity = capacity + (capacity / fb->val(u32(4)));
            // ...
            fb->store(newCapacity, self, 4);
        });

        // Attempt to copy construct the element in the array
        Value eleSz = fb->val(u32(ele.getType()->getInfo().size));
        Value eleOffset = used * eleSz;
        Value data = *dataPtr;
        Value dest = fb->ptrOffset(data, eleOffset);
        fb->generateConstruction(dest, { ele });
    }

    DataType* BuiltinArray::Register() {
        BuiltinArray* arr = (BuiltinArray*)Registry::GetType<array_t>();
        if (arr) return arr;

        arr = new BuiltinArray();
        Registry::Add(arr, type_hash<array_t>());

        return arr;
    }
};