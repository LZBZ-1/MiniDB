// src/core/types/CharType.hpp
#ifndef CHAR_TYPE_HPP
#define CHAR_TYPE_HPP

#include "DataType.hpp"
#include "exceptions/DataTypeException.hpp"
#include <string>
#include <algorithm>

namespace db::types {

    class CharType final : public DataType {
    public:
        static constexpr size_t MAX_LENGTH = 2000;  // Límite de Oracle para CHAR
        static constexpr char PADDING_CHAR = ' ';   // Carácter de relleno

        constexpr explicit CharType(bool isNullable = true, size_t length = 1)
            : nullable(isNullable), length(length) {
            validateLength(length);
        }

        [[nodiscard]] std::string getName() const override {
            return "CHAR(" + std::to_string(length) + ")";
        }

        [[nodiscard]] size_t getSize() const override {
            return length;
        }

        [[nodiscard]] bool isNullable() const override {
            return nullable;
        }

        [[nodiscard]] std::unique_ptr<DataType> clone() const override {
            return std::make_unique<CharType>(nullable, length);
        }

        [[nodiscard]] constexpr size_t getLength() const noexcept {
            return length;
        }

        // Formatea el valor según las reglas de CHAR (relleno con espacios)
        [[nodiscard]] std::string formatValue(const std::string& value) const {
            if (!isValidValue(value)) {
                throw DataTypeException("Value exceeds maximum length");
            }

            std::string formatted = value;
            // Rellenar con espacios hasta la longitud fija
            formatted.resize(length, PADDING_CHAR);
            return formatted;
        }

        [[nodiscard]] bool isValidValue(const std::string& value) const noexcept {
            return value.length() <= length;
        }

        // Compara dos valores CHAR ignorando espacios al final
        [[nodiscard]] static bool compareValues(const std::string& value1, const std::string& value2) noexcept {
            auto it1 = value1.rbegin();
            auto it2 = value2.rbegin();

            // Saltar espacios al final de ambas cadenas
            while (it1 != value1.rend() && *it1 == PADDING_CHAR) ++it1;
            while (it2 != value2.rend() && *it2 == PADDING_CHAR) ++it2;

            // Comparar el resto de los caracteres
            return std::string(value1.begin(), it1.base()) ==
                   std::string(value2.begin(), it2.base());
        }

    private:
        bool nullable;
        size_t length;

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

#endif // CHAR_TYPE_HPP