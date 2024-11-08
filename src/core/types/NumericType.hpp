// src/core/types/NumericType.hpp
#ifndef NUMERIC_TYPE_HPP
#define NUMERIC_TYPE_HPP

#include "DataType.hpp"

namespace db::types {

    class NumericType : public DataType {
    protected:
        bool nullable;
        size_t precision;
        size_t scale;

    public:
        // Constructor implementado inline
        constexpr NumericType(bool isNullable, size_t precision = 0, size_t scale = 0) noexcept
            : nullable(isNullable), precision(precision), scale(scale) {}

        // Funciones simples implementadas inline
        [[nodiscard]] bool isNullable() const override { return nullable; }
        [[nodiscard]] constexpr size_t getPrecision() const noexcept { return precision; }
        [[nodiscard]] constexpr size_t getScale() const noexcept { return scale; }
    };

} // namespace db::types

#endif // NUMERIC_TYPE_HPP