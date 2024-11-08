// src/core/types/factories/NumericTypeFactory.hpp
#ifndef NUMERIC_TYPE_FACTORY_HPP
#define NUMERIC_TYPE_FACTORY_HPP

#include "../NumberType.hpp"
#include <concepts>

namespace db::types {

    class NumericTypeFactory {
    public:
        // Prevenir instanciación
        NumericTypeFactory() = delete;

        template<typename T>
        requires std::integral<T> || std::floating_point<T>
        static std::unique_ptr<DataType> createNumber(
            bool nullable = true,
            T precision = NumberType::MAX_PRECISION,
            T scale = 0
        ) {
            return std::make_unique<NumberType>(
                nullable,
                static_cast<size_t>(precision),
                static_cast<size_t>(scale)
            );
        }

        template<typename T>
        requires std::integral<T> || std::floating_point<T>
        static std::unique_ptr<DataType> createInteger(
            bool nullable = true
        ) {
            return createNumber<T>(nullable, NumberType::MAX_PRECISION, 0);
        }

        template<typename T>
        requires std::integral<T> || std::floating_point<T>
        static std::unique_ptr<DataType> createDecimal(
            T precision,
            T scale,
            bool nullable = true
        ) {
            return createNumber<T>(nullable, precision, scale);
        }
    };

} // namespace db::types

#endif // NUMERIC_TYPE_FACTORY_HPP