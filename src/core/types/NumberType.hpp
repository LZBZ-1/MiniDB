// src/core/types/NumberType.hpp
#ifndef NUMBER_TYPE_HPP
#define NUMBER_TYPE_HPP

#include "NumericType.hpp"
#include "exceptions/DataTypeException.hpp"
#include <string>
#include <format>

namespace db::types {

    class NumberType final : public NumericType {
    public:
        static constexpr size_t MAX_PRECISION = 38;
        static constexpr size_t MAX_SCALE = 127;

        constexpr NumberType(bool isNullable = true, size_t precision = MAX_PRECISION, size_t scale = 0)
            : NumericType(isNullable, precision, scale) {
            validatePrecisionScale(precision, scale);
        }

        [[nodiscard]] std::string getName() const override;
        [[nodiscard]] size_t getSize() const override;
        [[nodiscard]] std::unique_ptr<DataType> clone() const override;

        [[nodiscard]] constexpr bool isInteger() const noexcept { return scale == 0; }
        [[nodiscard]] bool isInRange(double value) const noexcept;

    private:
        constexpr void validatePrecisionScale(size_t p, size_t s) {
            if (p > MAX_PRECISION) {
                throw DataTypeException("Precision cannot exceed " + std::to_string(MAX_PRECISION));
            }
            if (s > MAX_SCALE) {
                throw DataTypeException("Scale cannot exceed " + std::to_string(MAX_SCALE));
            }
            if (s > p) {
                throw DataTypeException("Scale cannot be greater than precision");
            }
        }
    };

} // namespace db::types

#endif // NUMBER_TYPE_HPP