// src/core/types/factories/DateTypeFactory.hpp
#ifndef DATE_TYPE_FACTORY_HPP
#define DATE_TYPE_FACTORY_HPP

#include "../DateType.hpp"

namespace db::types {

    class DateTypeFactory {
    public:
        DateTypeFactory() = delete;

        static std::unique_ptr<DataType> createDate(bool nullable = true) {
            return std::make_unique<DateType>(nullable);
        }
    };

} // namespace db::types

#endif // DATE_TYPE_FACTORY_HPP