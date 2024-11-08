// src/core/types/DataType.hpp
#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

#include <string>
#include <memory>

namespace db::types {  // Namespace unificado en C++17+

    class DataType {
    public:
        virtual ~DataType() = default;
        [[nodiscard]] virtual std::string getName() const = 0;
        [[nodiscard]] virtual size_t getSize() const = 0;
        [[nodiscard]] virtual bool isNullable() const = 0;
        [[nodiscard]] virtual std::unique_ptr<DataType> clone() const = 0;
    };

} // namespace db::types

#endif // DATA_TYPE_HPP