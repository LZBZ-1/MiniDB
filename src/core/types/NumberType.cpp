// src/core/types/NumberType.cpp
#include "NumberType.hpp"
#include <cmath>

namespace db::types {

    std::string NumberType::getName() const {
        if (scale == 0) {
            return "NUMBER(" + std::to_string(precision) + ")";
        }
        return "NUMBER(" + std::to_string(precision) + "," + std::to_string(scale) + ")";
    }

    size_t NumberType::getSize() const {
        return 1 + static_cast<size_t>(std::ceil(precision / 2.0));
    }

    std::unique_ptr<DataType> NumberType::clone() const {
        return std::make_unique<NumberType>(nullable, precision, scale);
    }

    bool NumberType::isInRange(double value) const noexcept {
        // Para NUMBER(p,s), el rango es:
        // max = 10^(p-s) - 10^(-s)
        // min = -(10^(p-s) - 10^(-s))

        // Calculamos 10^(p-s)
        const double maxIntegerPart = std::pow(10.0, static_cast<double>(precision - scale));

        // Calculamos 10^(-s)
        const double minFraction = std::pow(10.0, -static_cast<double>(scale));

        // El máximo valor permitido
        const double maxValue = maxIntegerPart - minFraction;

        // El mínimo es el negativo del máximo
        const double minValue = -maxValue;

        // Ajustamos el valor a la escala correcta para la comparación
        const double scaledValue = std::round(value * std::pow(10.0, scale)) / std::pow(10.0, scale);

        return scaledValue >= minValue && scaledValue <= maxValue;
    }

} // namespace db::types