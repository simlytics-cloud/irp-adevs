#pragma once


/**
 * @class Coordinate
 * @brief Represents a two-dimensional coordinate in Cartesian space.
 *
 * The Coordinate class encapsulates an immutable (x, y) pair to describe a location
 * in a two-dimensional Cartesian coordinate system.
 *
 * This class is designed to be lightweight and is eligible for constexpr evaluation,
 * enabling compile-time computation in certain contexts.
 *
 * The class is immutable, as the x and y members are constants and cannot be modified
 * after object construction.
 *
 * Constructor:
 * - The default constructor is explicitly deleted, ensuring that coordinates must
 *   be explicitly initialized with valid x and y values.
 * - A constexpr constructor is provided to initialize x and y with user-specified
 *   double values.
 */
class Coordinate {
  public:
    const double x;
    const double y;

    Coordinate() = delete;

    constexpr Coordinate(double xIn, double yIn)
        : x(xIn), y(yIn) {}

};
