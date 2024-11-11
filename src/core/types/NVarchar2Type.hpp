// src/core/types/NVarchar2Type.hpp
#ifndef NVARCHAR2_TYPE_HPP
#define NVARCHAR2_TYPE_HPP

#include "DataType.hpp"
#include "exceptions/DataTypeException.hpp"
#include <string>
#include <codecvt>
#include <locale>

namespace db::types {

    class NVarchar2Type final : public DataType {
    public:
        static constexpr size_t MAX_LENGTH = 4000;  // Límite de Oracle para NVARCHAR2

        constexpr explicit NVarchar2Type(bool isNullable = true, size_t maxLength = MAX_LENGTH)
            : nullable(isNullable), maxLength(maxLength) {
            validateLength(maxLength);
        }

        [[nodiscard]] std::string getName() const override {
            return "NVARCHAR2(" + std::to_string(maxLength) + ")";
        }

        [[nodiscard]] size_t getSize() const override {
            return maxLength * 2;  // Cada carácter Unicode ocupa 2 bytes
        }

        [[nodiscard]] bool isNullable() const override {
            return nullable;
        }

        [[nodiscard]] std::unique_ptr<DataType> clone() const override {
            return std::make_unique<NVarchar2Type>(nullable, maxLength);
        }

        [[nodiscard]] constexpr size_t getMaxLength() const noexcept {
            return maxLength;
        }

        // Cuenta caracteres Unicode usando la misma lógica que NCHAR
        [[nodiscard]] static size_t getUnicodeLength(const std::string& str) noexcept {
            try {
                std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
                return converter.from_bytes(str).size();
            } catch (...) {
                return str.length();
            }
        }

        [[nodiscard]] bool isValidValue(const std::string& value) const noexcept {
            return getUnicodeLength(value) <= maxLength;
        }

        // A diferencia de NCHAR, NVARCHAR2 no hace padding
        [[nodiscard]] std::string formatValue(const std::string& value) const {
            if (!isValidValue(value)) {
                throw DataTypeException("Value exceeds maximum length in characters");
            }
            return value;  // Retorna el valor tal cual, sin padding
        }

        // Comparación directa sin ignorar espacios al final
        [[nodiscard]] static bool compareValues(
            const std::string& value1,
            const std::string& value2
        ) noexcept {
            return value1 == value2;  // Comparación exacta
        }

    private:
        bool nullable;
        size_t maxLength;

        constexpr void validateLength(size_t len) {
            if (len == 0) {
                throw DataTypeException("Length must be greater than 0");
            }
            if (len > MAX_LENGTH) {
                throw DataTypeException("Length cannot exceed " + std::to_string(MAX_LENGTH));
            }
        }
    };

} // namespace db::types

#endif // NVARCHAR2_TYPE_HPP