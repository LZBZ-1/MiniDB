// tests/core/types/NumberTypeTest.cpp
#include "NumberTypeTest.hpp"
#include <stdexcept>
#include <cmath>

namespace db::types::test {
    TEST_F(NumberTypeTest, ConstructorShouldInitializeWithValidParameters) {
        NumberType number{true, 10, 2};

        EXPECT_EQ(number.getPrecision(), 10);
        EXPECT_EQ(number.getScale(), 2);
        EXPECT_TRUE(number.isNullable());
    }

    TEST_F(NumberTypeTest, GetNameShouldReturnCorrectFormatForIntegers) {
        NumberType integer{true, 10, 0};
        EXPECT_EQ(integer.getName(), "NUMBER(10)");
    }

    TEST_F(NumberTypeTest, GetNameShouldReturnCorrectFormatForDecimals) {
        NumberType decimal{true, 10, 2};
        EXPECT_EQ(decimal.getName(), "NUMBER(10,2)");
    }

    TEST_F(NumberTypeTest, ConstructorShouldThrowOnInvalidPrecision) {
        const size_t invalidPrecision = NumberType::MAX_PRECISION + 1;
        try {
            NumberType invalid{true, invalidPrecision, 0};
            FAIL() << "Expected DataTypeException";
        } catch (const DataTypeException &e) {
            SUCCEED();
        } catch (...) {
            FAIL() << "Expected DataTypeException, got different exception";
        }
    }

    TEST_F(NumberTypeTest, ConstructorShouldThrowOnInvalidScale) {
        const size_t invalidScale = NumberType::MAX_SCALE + 1;
        try {
            NumberType invalid{true, 10, invalidScale};
            FAIL() << "Expected DataTypeException";
        } catch (const DataTypeException &e) {
            SUCCEED();
        } catch (...) {
            FAIL() << "Expected DataTypeException, got different exception";
        }
    }

    TEST_F(NumberTypeTest, ConstructorShouldThrowWhenScaleExceedsPrecision) {
        try {
            NumberType invalid{true, 10, 11};
            FAIL() << "Expected DataTypeException";
        } catch (const DataTypeException &e) {
            SUCCEED();
        } catch (...) {
            FAIL() << "Expected DataTypeException, got different exception";
        }
    }

    TEST_F(NumberTypeTest, GetSizeShouldReturnCorrectValue) {
        struct TestCase {
            size_t precision;
            size_t scale;
            size_t expectedSize;
        };

        const TestCase testCases[] = {
            {5, 2, 4}, // 1 + ceil(5/2)
            {10, 2, 6}, // 1 + ceil(10/2)
            {15, 2, 9} // 1 + ceil(15/2)
        };

        for (const auto &tc: testCases) {
            NumberType number{true, tc.precision, tc.scale};
            EXPECT_EQ(number.getSize(), tc.expectedSize)
            << "Failed for precision=" << tc.precision;
        }
    }

    TEST_F(NumberTypeTest, CloneShouldCreateExactCopy) {
        NumberType original{true, 10, 2};
        auto cloned = original.clone();

        const auto *numberClone = dynamic_cast<const NumberType *>(cloned.get());
        ASSERT_NE(numberClone, nullptr);

        EXPECT_EQ(numberClone->getPrecision(), original.getPrecision());
        EXPECT_EQ(numberClone->getScale(), original.getScale());
        EXPECT_EQ(numberClone->isNullable(), original.isNullable());
    }

    TEST_F(NumberTypeTest, IsIntegerShouldReturnCorrectValue) {
        NumberType integer{true, 10, 0};
        NumberType decimal{true, 10, 2};

        EXPECT_TRUE(integer.isInteger());
        EXPECT_FALSE(decimal.isInteger());
    }

    TEST_F(NumberTypeTest, IsInRangeShouldWorkCorrectly) {
        NumberType number{true, 3, 1}; // NUMBER(3,1)

        struct TestCase {
            double value;
            bool expectedResult;
            std::string description;
        };

        const TestCase testCases[] = {
            {99.9, true, "Valor máximo permitido"},
            {-99.9, true, "Valor mínimo permitido"},
            {0.0, true, "Cero"},
            {50.5, true, "Valor positivo medio"},
            {-50.5, true, "Valor negativo medio"},
            {99.8, true, "Justo bajo el máximo"},
            {-99.8, true, "Justo sobre el mínimo"},
            {100.0, false, "Excede máximo permitido"},
            {-100.0, false, "Excede mínimo permitido"},
            {150.5, false, "Muy por encima del rango"},
            {-150.5, false, "Muy por debajo del rango"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(number.isInRange(tc.value), tc.expectedResult)
                << "Failed for " << tc.description
                << " (value=" << tc.value << ")";
        }
    }

    TEST_F(NumberTypeTest, IsInRangeShouldHandleEdgeCases) {
        struct TestConfig {
            size_t precision;
            size_t scale;
            std::string description;
        };

        const TestConfig configs[] = {
            {1, 0, "Solo dígitos del 0-9"},
            {2, 1, "Un decimal"},
            {3, 2, "Dos decimales"},
            {5, 0, "Número grande sin decimales"},
            {5, 3, "Tres decimales"}
        };

        for (const auto& config : configs) {
            NumberType number{true, config.precision, config.scale};

            // Calculamos los límites esperados
            double powerFactor = std::pow(10.0, static_cast<double>(config.precision - config.scale));
            double maxValue = powerFactor - std::pow(10.0, -static_cast<double>(config.scale));

            SCOPED_TRACE("Testing " + config.description +
                        " NUMBER(" + std::to_string(config.precision) +
                        "," + std::to_string(config.scale) + ")");

            // Prueba valores límite
            EXPECT_TRUE(number.isInRange(maxValue))
                << "Máximo valor permitido: " << maxValue;
            EXPECT_TRUE(number.isInRange(-maxValue))
                << "Mínimo valor permitido: " << -maxValue;

            // Prueba valores justo fuera del rango
            EXPECT_FALSE(number.isInRange(maxValue + std::pow(10.0, -static_cast<double>(config.scale))))
                << "Valor justo por encima del máximo";
            EXPECT_FALSE(number.isInRange(-maxValue - std::pow(10.0, -static_cast<double>(config.scale))))
                << "Valor justo por debajo del mínimo";

            // Prueba valores dentro del rango
            EXPECT_TRUE(number.isInRange(0.0)) << "Cero siempre debe estar en rango";
            if (config.scale > 0) {
                double smallestDecimal = std::pow(10.0, -static_cast<double>(config.scale));
                EXPECT_TRUE(number.isInRange(smallestDecimal))
                    << "Mínimo valor decimal positivo: " << smallestDecimal;
                EXPECT_TRUE(number.isInRange(-smallestDecimal))
                    << "Mínimo valor decimal negativo: " << -smallestDecimal;
            }
        }
    }


    TEST_F(NumberTypeTest, FactoryMethodsShouldCreateValidInstances) {
        // Especificamos explícitamente los tipos para evitar problemas de deducción
        auto defaultNumber = NumericTypeFactory::createNumber<size_t>(true);
        auto integer = NumericTypeFactory::createInteger<size_t>(true);
        auto decimal = NumericTypeFactory::createDecimal<size_t>(5, 2, true);

        EXPECT_EQ(defaultNumber->getName(), "NUMBER(38)");

        const auto *integerType = dynamic_cast<const NumberType *>(integer.get());
        ASSERT_NE(integerType, nullptr);
        EXPECT_TRUE(integerType->isInteger());

        EXPECT_EQ(decimal->getName(), "NUMBER(5,2)");
    }
} // namespace db::types::test
