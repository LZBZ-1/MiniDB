// src/core/types/NCharType.hpp
#ifndef NCHAR_TYPE_HPP
#define NCHAR_TYPE_HPP

#include "DataType.hpp"
#include "exceptions/DataTypeException.hpp"
#include <string>
#include <string_view>
#include <codecvt>
#include <locale>

namespace db::types {

    class NCharType final : public DataType {
    public:
        static constexpr size_t MAX_LENGTH = 1000;  // Límite de Oracle para NCHAR
        static constexpr char32_t PADDING_CHAR = U' ';  // Espacio Unicode

        constexpr explicit NCharType(bool isNullable = true, size_t length = 1)
            : nullable(isNullable), length(length) {
            validateLength(length);
        }

        [[nodiscard]] std::string getName() const override {
            return "NCHAR(" + std::to_string(length) + ")";
        }

        [[nodiscard]] size_t getSize() const override {
            return length * 2;  // Cada carácter Unicode ocupa 2 bytes
        }

        [[nodiscard]] bool isNullable() const override {
            return nullable;
        }

        [[nodiscard]] std::unique_ptr<DataType> clone() const override {
            return std::make_unique<NCharType>(nullable, length);
        }

        [[nodiscard]] constexpr size_t getLength() const noexcept {
            return length;
        }

        // Mé-to-do mejorado para contar caracteres Unicode
        [[nodiscard]] static size_t getUnicodeLength(const std::string& str) noexcept {
            try {
                std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
                return converter.from_bytes(str).size();
            } catch (...) {
                // En caso de error de conversión, retornamos la longitud del string
                return str.length();
            }
        }

        [[nodiscard]] std::string formatValue(const std::string& value) const {
            if (!isValidValue(value)) {
                throw DataTypeException("Value exceeds maximum length in characters");
            }

            std::string result = value;
            size_t currentLength = getUnicodeLength(result);

            // Agregar espacios Unicode hasta alcanzar la longitud requerida
            while (currentLength < length) {
                result += " ";  // Espacio UTF-8
                currentLength++;
            }

            return result;
        }

        [[nodiscard]] bool isValidValue(const std::string& value) const noexcept {
            return getUnicodeLength(value) <= length;
        }

        // Compara dos valores NCHAR ignorando espacios al final
        [[nodiscard]] static bool compareValues(
            const std::string& value1,
            const std::string& value2
        ) noexcept {
            std::string_view v1(value1);
            std::string_view v2(value2);

            // Eliminar espacios al final
            while (!v1.empty() && v1.back() == ' ') v1.remove_suffix(1);
            while (!v2.empty() && v2.back() == ' ') v2.remove_suffix(1);

            return v1 == v2;
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

#endif // NCHAR_TYPE_HPP