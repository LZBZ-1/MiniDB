// src/core/types/exceptions/DataTypeException.hpp
#ifndef DATA_TYPE_EXCEPTION_HPP
#define DATA_TYPE_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <source_location>

namespace db::types {

    class DataTypeException : public std::runtime_error {
    public:
        explicit DataTypeException(
            const std::string& message,
            const std::source_location& location = std::source_location::current()
        ) : std::runtime_error(
                std::string(location.file_name()) + ":" +
                std::to_string(location.line()) + " - " +
                location.function_name() + ": " + message
            ),
            location_(location) {}

        [[nodiscard]] const std::source_location& where() const noexcept {
            return location_;
        }

    private:
        std::source_location location_;
    };

} // namespace db::types

#endif // DATA_TYPE_EXCEPTION_HPP