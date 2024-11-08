// src/core/types/Varchar2Type.hpp
#ifndef VARCHAR2_TYPE_HPP
#define VARCHAR2_TYPE_HPP

#include "DataType.hpp"
#include "exceptions/DataTypeException.hpp"

namespace db::types {

    class Varchar2Type final : public DataType {
    public:
        static constexpr size_t MAX_LENGTH = 4000;  // Límite de Oracle para VARCHAR2

        constexpr explicit Varchar2Type(bool isNullable = true, size_t maxLength = MAX_LENGTH)
            : nullable(isNullable), maxLength(maxLength) {
            validateLength(maxLength);
        }

        [[nodiscard]] std::string getName() const override {
            return "VARCHAR2(" + std::to_string(maxLength) + ")";
        }

        [[nodiscard]] size_t getSize() const override {
            return maxLength;
        }

        [[nodiscard]] bool isNullable() const override {
            return nullable;
        }

        [[nodiscard]] std::unique_ptr<DataType> clone() const override {
            return std::make_unique<Varchar2Type>(nullable, maxLength);
        }

        [[nodiscard]] constexpr size_t getMaxLength() const noexcept {
            return maxLength;
        }

        [[nodiscard]] bool isValidValue(const std::string& value) const noexcept {
            return value.length() <= maxLength;
        }

    private:
        bool nullable;
        size_t maxLength;

        constexpr void validateLength(size_t length) {
            if (length == 0) {
                throw DataTypeException("Length must be greater than 0");
            }
            if (length > MAX_LENGTH) {
                throw DataTypeException("Length cannot exceed " + std::to_string(MAX_LENGTH));
            }
        }
    };

} // namespace db::types

#endif // VARCHAR2_TYPE_HPP