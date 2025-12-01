#ifndef PATHRENDER_COLOR_HPP_
#define PATHRENDER_COLOR_HPP_

#include "PathRender/core/export.hpp"
#include <string>

namespace PathRender {
    /**
    * @class Color
    * @brief Represents a color in RGB format.
    *  The Color class encapsulates a color defined by its red, green, and blue components.
    *  Each component is a double value ranging from 0.0 to 1.0, representing the intensity of the
    * color. The class provides constructors for initializing colors with double or integer values.
    */
    class PATHRENDER_EXPORT Color {
    public:
        double r; ///< Red component of the color (0.0 to 1.0)
        double g; ///< Green component of the color (0.0 to 1.0)
        double b; ///< Blue component of the color (0.0 to 1.0)
        
        /**
        * @brief Default constructor that initializes the color to black (0, 0, 0).
        */
        Color();

        /**
        * @brief Constructor that initializes the color with specified red, green, and blue values.
        * @param red The red component of the color (0.0 to 1.0).
        * @param green The green component of the color (0.0 to 1.0).
        * @param blue The blue component of the color (0.0 to 1.0).
        */
        Color(double red, double green, double blue);

        /**
        * @brief Constructor that initializes the color with specified red, green, and blue values as
        * integers.
        * @param red The red component of the color (0 to 255).
        * @param green The green component of the color (0 to 255).
        * @param blue The blue component of the color (0 to 255).
        * This constructor converts the integer values to double in the range of 0.0 to 1.0.
        */
        Color(int red, int green, int blue);

        /**
        * @brief Copy constructor that initializes the color from another Color object.
        * @param other The Color object to copy from.
        */
        Color(const Color& other);

        Color operator*(Color other) const;

        Color operator*(double scalar) const;

        Color operator+(Color other) const;

        Color operator+=(Color other);

        Color& clamp();

        std::string to_string() const;
    };

} // namespace PathRender
#endif // PATHRENDER_COLOR_HPP_