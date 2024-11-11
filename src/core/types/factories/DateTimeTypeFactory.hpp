#ifndef DATETIME_TYPE_FACTORY_HPP
#define DATETIME_TYPE_FACTORY_HPP

#include "../TimestampType.hpp"
#include "../DateType.hpp"

namespace db::types {

    class DateTimeTypeFactory {
    public:
        DateTimeTypeFactory() = delete;

        static std::unique_ptr<DataType> createDate(bool nullable = true) {
            return std::make_unique<DateType>(nullable);
        }

        static std::unique_ptr<DataType> createTimestamp(
            int precision = 6,
            bool withTimeZone = false,
            bool nullable = true) {
            return std::make_unique<TimestampType>(precision, withTimeZone, nullable);
        }

        // Factory method for timestamp with time zone
        static std::unique_ptr<DataType> createTimestampTZ(
            int precision = 6,
            bool nullable = true) {
            return createTimestamp(precision, true, nullable);
        }
    };

} // namespace db::types

#endif // DATETIME_TYPE_FACTORY_HPP