// src/core/types/factories/StringTypeFactory.hpp
#ifndef STRING_TYPE_FACTORY_HPP
#define STRING_TYPE_FACTORY_HPP

#include "../Varchar2Type.hpp"
#include "../NVarchar2Type.hpp"
#include "../CharType.hpp"
#include "../NCharType.hpp"
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

        template<typename T>
        requires std::integral<T> || std::floating_point<T>
        static std::unique_ptr<DataType> createChar(
            T length = 1,
            bool nullable = true
        ) {
            return std::make_unique<CharType>(
                nullable,
                static_cast<size_t>(length)
            );
        }

        static std::unique_ptr<DataType> createDefaultChar(
            bool nullable = true
        ) {
            return createChar<size_t>(1, nullable);
        }

        static std::unique_ptr<DataType> createDefaultVarchar2(
            bool nullable = true
        ) {
            return createVarchar2<size_t>(Varchar2Type::MAX_LENGTH, nullable);
        }
        template<typename T>
        requires std::integral<T> || std::floating_point<T>
        static std::unique_ptr<DataType> createNChar(
            T length = 1,
            bool nullable = true
        ) {
            return std::make_unique<NCharType>(
                nullable,
                static_cast<size_t>(length)
            );
        }

        static std::unique_ptr<DataType> createDefaultNChar(
            bool nullable = true
        ) {
            return createNChar<size_t>(1, nullable);
        }

        template<typename T>
        requires std::integral<T> || std::floating_point<T>
        static std::unique_ptr<DataType> createNVarchar2(
            T maxLength,
            bool nullable = true
        ) {
            return std::make_unique<NVarchar2Type>(
                nullable,
                static_cast<size_t>(maxLength)
            );
        }

        static std::unique_ptr<DataType> createDefaultNVarchar2(
            bool nullable = true
        ) {
            return createNVarchar2<size_t>(NVarchar2Type::MAX_LENGTH, nullable);
        }
    };

} // namespace db::types

#endif // STRING_TYPE_FACTORY_HPP