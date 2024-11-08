// src/core/types/factories/StringTypeFactory.hpp
#ifndef STRING_TYPE_FACTORY_HPP
#define STRING_TYPE_FACTORY_HPP

#include "../Varchar2Type.hpp"
#include <concepts>

namespace db::types {

    class StringTypeFactory {
    public:
        StringTypeFactory() = delete;

        template<typename T>
        requires std::integral<T> || std::floating_point<T>
        static std::unique_ptr<DataType> createVarchar2(
            T maxLength,
            bool nullable = true
        ) {
            return std::make_unique<Varchar2Type>(
                nullable,
                static_cast<size_t>(maxLength)
            );
        }

        static std::unique_ptr<DataType> createDefaultVarchar2(
            bool nullable = true
        ) {
            return createVarchar2<size_t>(Varchar2Type::MAX_LENGTH, nullable);
        }
    };

} // namespace db::types

#endif // STRING_TYPE_FACTORY_HPP