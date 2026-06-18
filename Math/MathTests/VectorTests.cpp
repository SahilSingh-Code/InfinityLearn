//----------------------------------------------------------------------------------------------------------------------
// VectorTests.cpp
//
// Unit tests for InfinityLearn's vector types.
//----------------------------------------------------------------------------------------------------------------------

#include "Core/Error.h"                 // IL_SUPPRESS_ERROR_LOGGING_FOR_THIS_FILE
#include "Math/Vectors.h"               // Vectors
#include <catch2/catch_approx.hpp>      // Catch::Approx
#include <catch2/catch_test_macros.hpp> // TEST_CASE, REQUIRE, REQUIRE_FALSE, REQUIRE_THROWS
#include <cmath>                        // std::abs, std::exp, etc.
#include <limits>                       // std::numeric_limits

// Suppress error outputs from the math tests since we intentionally trigger some error conditions (e.g. log of
// non-positive numbers) to verify that checks are working correctly. This keeps the test output cleaner and more
// focused on the test results.
IL_SUPPRESS_ERROR_LOGGING_FOR_THIS_FILE();

// We just blanket use the InfinityLearn namespace in this test file since it's focused on testing InfinityLearn's
// vector types, and it would be cumbersome to keep qualifying everything with InfinityLearn::.
using namespace InfinityLearn;

//----------------------------------------------------------
// Test helpers to compare vectors against expected results
//----------------------------------------------------------

namespace
{
constexpr double kTolerance = 1e-12;

template <typename Vec>
void requireVectorApprox(const Vec& vector, std::initializer_list<double> expected, double tolerance = kTolerance)
{
    REQUIRE(vector.size() == expected.size());

    std::size_t i = 0;
    for (const double expected_value : expected)
    {
        REQUIRE(static_cast<double>(vector[i]) == Catch::Approx(expected_value).margin(tolerance));
        ++i;
    }
}

template <typename Vec>
void requireVectorExact(const Vec& vector,
                        std::initializer_list<typename std::remove_reference_t<Vec>::value_type> expected)
{
    REQUIRE(vector.size() == expected.size());

    std::size_t i = 0;
    for (const auto& expected_value : expected)
    {
        REQUIRE(vector[i] == expected_value);
        ++i;
    }
}
} // namespace

//----------------------------------------------------------------------------------------------------------------------
// Static vector construction and aliases
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("StaticVector default construction zero-initialized values", "[Core][Vector]")
{
    Vector3d vector;

    REQUIRE(vector.size() == 3);
    REQUIRE_FALSE(vector.empty());

    requireVectorApprox(vector, {0.0, 0.0, 0.0});
}

TEST_CASE("StaticVector initializer-list construction stores provided values", "[Core][Vector]")
{
    const Vector3d vector{1.0, 2.0, 3.0};

    REQUIRE(vector.size() == 3);
    requireVectorApprox(vector, {1.0, 2.0, 3.0});
}

TEST_CASE("StaticVector aliases have expected sizes and element types", "[Core][Vector]")
{
    Vector2f vector2f{1.0f, 2.0f};
    Vector3f vector3f{1.0f, 2.0f, 3.0f};
    Vector4f vector4f{1.0f, 2.0f, 3.0f, 4.0f};

    Vector2d vector2d{1.0, 2.0};
    Vector3d vector3d{1.0, 2.0, 3.0};
    Vector4d vector4d{1.0, 2.0, 3.0, 4.0};

    Vector2i vector2i{1, 2};
    Vector3i vector3i{1, 2, 3};
    Vector4i vector4i{1, 2, 3, 4};

    REQUIRE(vector2f.size() == 2);
    REQUIRE(vector3f.size() == 3);
    REQUIRE(vector4f.size() == 4);

    REQUIRE(vector2d.size() == 2);
    REQUIRE(vector3d.size() == 3);
    REQUIRE(vector4d.size() == 4);

    REQUIRE(vector2i.size() == 2);
    REQUIRE(vector3i.size() == 3);
    REQUIRE(vector4i.size() == 4);
}

TEST_CASE("StaticVector initializer-list construction asserts on wrong size", "[Core][Vector]")
{
    REQUIRE_THROWS([] { Vector3d vector{1.0, 2.0}; }());
    REQUIRE_THROWS([] { Vector3d vector{1.0, 2.0, 3.0, 4.0}; }());
}

//----------------------------------------------------------------------------------------------------------------------
// Dynamic vector construction, mutation, and capacity
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("DynamicVector default construction creates empty vector", "[Core][Vector]")
{
    VectorXd vector;

    REQUIRE(vector.size() == 0);
    REQUIRE(vector.empty());
}

TEST_CASE("DynamicVector size construction value-initializes values", "[Core][Vector]")
{
    VectorXd vector(5);

    REQUIRE(vector.size() == 5);
    REQUIRE_FALSE(vector.empty());

    requireVectorApprox(vector, {0.0, 0.0, 0.0, 0.0, 0.0});
}

TEST_CASE("DynamicVector fill-value construction initializes every value", "[Core][Vector]")
{
    VectorXd vector(4, 2.5);

    REQUIRE(vector.size() == 4);
    requireVectorApprox(vector, {2.5, 2.5, 2.5, 2.5});
}

TEST_CASE("DynamicVector initializer-list construction stores provided values", "[Core][Vector]")
{
    VectorXd vector{1.0, 2.0, 3.0, 4.0};

    REQUIRE(vector.size() == 4);
    requireVectorApprox(vector, {1.0, 2.0, 3.0, 4.0});
}

TEST_CASE("DynamicVector resize can grow with value-initialized entries", "[Core][Vector]")
{
    VectorXd vector{1.0, 2.0, 3.0};

    vector.resize(6);

    REQUIRE(vector.size() == 6);
    requireVectorApprox(vector, {1.0, 2.0, 3.0, 0.0, 0.0, 0.0});
}

TEST_CASE("DynamicVector resize can grow with explicit fill value", "[Core][Vector]")
{
    VectorXd vector{1.0, 2.0, 3.0};

    vector.resize(6, 9.0);

    REQUIRE(vector.size() == 6);
    requireVectorApprox(vector, {1.0, 2.0, 3.0, 9.0, 9.0, 9.0});
}

TEST_CASE("DynamicVector resize can shrink while preserving prefix", "[Core][Vector]")
{
    VectorXd vector{1.0, 2.0, 3.0, 4.0, 5.0};

    vector.resize(2);

    REQUIRE(vector.size() == 2);
    requireVectorApprox(vector, {1.0, 2.0});
}

TEST_CASE("DynamicVector reserve capacity and shrinkToFit behave consistently", "[Core][Vector]")
{
    VectorXd vector{1.0, 2.0, 3.0};

    vector.reserve(128);

    REQUIRE(vector.size() == 3);
    REQUIRE(vector.capacity() >= 128);

    vector.shrinkToFit();

    REQUIRE(vector.size() == 3);
    REQUIRE(vector.capacity() >= vector.size());
}

TEST_CASE("DynamicVector pushBack, emplaceBack, popBack, and clear work", "[Core][Vector]")
{
    VectorXd vector;

    vector.pushBack(1.0);
    vector.pushBack(2.0);

    double moved_value = 3.0;
    vector.pushBack(std::move(moved_value));

    double& emplaced = vector.emplaceBack(4.0);

    REQUIRE(emplaced == Catch::Approx(4.0));
    requireVectorApprox(vector, {1.0, 2.0, 3.0, 4.0});

    vector.popBack();
    requireVectorApprox(vector, {1.0, 2.0, 3.0});

    vector.clear();
    REQUIRE(vector.empty());

    REQUIRE_THROWS(vector.popBack());
}

TEST_CASE("DynamicVector integer alias stores integer values", "[Core][Vector]")
{
    VectorXi vector{1, 2, 3, 4};

    REQUIRE(vector.size() == 4);
    REQUIRE(vector[0] == 1);
    REQUIRE(vector[1] == 2);
    REQUIRE(vector[2] == 3);
    REQUIRE(vector[3] == 4);
}

//----------------------------------------------------------------------------------------------------------------------
// Accessors and raw data
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Vector operator[] and named accessors read and write values", "[Core][Vector]")
{
    Vector4d vector{1.0, 2.0, 3.0, 4.0};

    REQUIRE(vector[0] == Catch::Approx(1.0));
    REQUIRE(vector[1] == Catch::Approx(2.0));
    REQUIRE(vector[2] == Catch::Approx(3.0));
    REQUIRE(vector[3] == Catch::Approx(4.0));

    REQUIRE(vector.x() == Catch::Approx(1.0));
    REQUIRE(vector.y() == Catch::Approx(2.0));
    REQUIRE(vector.z() == Catch::Approx(3.0));
    REQUIRE(vector.w() == Catch::Approx(4.0));

    vector.x() = 10.0;
    vector.y() = 20.0;
    vector.z() = 30.0;
    vector.w() = 40.0;

    requireVectorApprox(vector, {10.0, 20.0, 30.0, 40.0});
}

TEST_CASE("Vector const named accessors read values", "[Core][Vector]")
{
    const Vector4d vector{1.0, 2.0, 3.0, 4.0};

    REQUIRE(vector.x() == Catch::Approx(1.0));
    REQUIRE(vector.y() == Catch::Approx(2.0));
    REQUIRE(vector.z() == Catch::Approx(3.0));
    REQUIRE(vector.w() == Catch::Approx(4.0));
}

TEST_CASE("Vector accessors assert on invalid indices", "[Core][Vector]")
{
    Vector3d static_vector{1.0, 2.0, 3.0};
    VectorXd dynamic_vector{1.0, 2.0, 3.0};

    REQUIRE_THROWS(static_vector[3]);
    REQUIRE_THROWS(dynamic_vector[3]);

    REQUIRE_THROWS(static_vector.w());
    REQUIRE_THROWS(dynamic_vector.w());
}

TEST_CASE("Vector data exposes mutable and const contiguous storage", "[Core][Vector]")
{
    Vector3d static_vector{1.0, 2.0, 3.0};

    double* static_data = static_vector.data();
    static_data[0] = 4.0;
    static_data[1] = 5.0;
    static_data[2] = 6.0;

    requireVectorApprox(static_vector, {4.0, 5.0, 6.0});

    const Vector3d const_static_vector{7.0, 8.0, 9.0};
    const double* const_static_data = const_static_vector.data();

    REQUIRE(const_static_data[0] == Catch::Approx(7.0));
    REQUIRE(const_static_data[1] == Catch::Approx(8.0));
    REQUIRE(const_static_data[2] == Catch::Approx(9.0));

    VectorXd dynamic_vector{1.0, 2.0, 3.0};

    double* dynamic_data = dynamic_vector.data();
    dynamic_data[0] = 10.0;
    dynamic_data[1] = 20.0;
    dynamic_data[2] = 30.0;

    requireVectorApprox(dynamic_vector, {10.0, 20.0, 30.0});
}

//----------------------------------------------------------------------------------------------------------------------
// Equality and approximate equality
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Vector equality compares sizes and values exactly", "[Core][Vector]")
{
    const Vector3d a{1.0, 2.0, 3.0};
    const Vector3d b{1.0, 2.0, 3.0};
    const Vector3d c{1.0, 2.0, 4.0};

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);

    const VectorXd x{1.0, 2.0};
    const VectorXd y{1.0, 2.0, 3.0};

    REQUIRE_FALSE(x == y);
}

TEST_CASE("Vector isApprox compares values within tolerance", "[Core][Vector]")
{
    const Vector3d a{1.0, 2.0, 3.0};
    const Vector3d b{1.0 + 1e-10, 2.0 - 1e-10, 3.0};
    const Vector3d c{1.0, 2.0, 3.01};

    REQUIRE(a.isApprox(b));
    REQUIRE_FALSE(a.isApprox(c));

    const VectorXd x{1.0, 2.0};
    const VectorXd y{1.0, 2.0, 3.0};

    REQUIRE_FALSE(x.isApprox(y));
}

//----------------------------------------------------------------------------------------------------------------------
// Arithmetic operators
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Vector addition and subtraction produce correct results", "[Core][Vector]")
{
    const Vector3d a{1.0, 2.0, 3.0};
    const Vector3d b{4.0, 5.0, 6.0};

    requireVectorApprox(a + b, {5.0, 7.0, 9.0});
    requireVectorApprox(b - a, {3.0, 3.0, 3.0});

    Vector3d c{1.0, 1.0, 1.0};
    c += a;
    requireVectorApprox(c, {2.0, 3.0, 4.0});

    c -= Vector3d{1.0, 1.0, 1.0};
    requireVectorApprox(c, {1.0, 2.0, 3.0});
}

TEST_CASE("Vector addition and subtraction assert on mismatched sizes", "[Core][Vector]")
{
    VectorXd a{1.0, 2.0};
    VectorXd b{1.0, 2.0, 3.0};

    REQUIRE_THROWS(a + b);
    REQUIRE_THROWS(a - b);
    REQUIRE_THROWS(a += b);
    REQUIRE_THROWS(a -= b);
}

TEST_CASE("Vector scalar multiplication works from right and left", "[Core][Vector]")
{
    const Vector3d vector{1.0, -2.0, 3.0};

    requireVectorApprox(vector * 2.0, {2.0, -4.0, 6.0});
    requireVectorApprox(2.0 * vector, {2.0, -4.0, 6.0});

    Vector3d mutable_vector{1.0, -2.0, 3.0};
    mutable_vector *= 3.0;
    requireVectorApprox(mutable_vector, {3.0, -6.0, 9.0});
}

TEST_CASE("Vector scalar division works and asserts on division by zero", "[Core][Vector]")
{
    const Vector3d vector{2.0, -4.0, 6.0};

    requireVectorApprox(vector / 2.0, {1.0, -2.0, 3.0});

    Vector3d mutable_vector{2.0, -4.0, 6.0};
    mutable_vector /= 2.0;
    requireVectorApprox(mutable_vector, {1.0, -2.0, 3.0});

    REQUIRE_THROWS(vector / 0.0);
    REQUIRE_THROWS(mutable_vector /= 0.0);
}

TEST_CASE("Vector unary negation produces elementwise negative", "[Core][Vector]")
{
    const Vector3d vector{1.0, -2.0, 3.0};

    requireVectorApprox(-vector, {-1.0, 2.0, -3.0});
}

//----------------------------------------------------------------------------------------------------------------------
// Dot, cross, projection, distances
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Vector dot product computes expected scalar", "[Core][Vector]")
{
    const Vector3d a{1.0, 2.0, 3.0};
    const Vector3d b{4.0, 5.0, 6.0};

    REQUIRE(a.dot(b) == Catch::Approx(32.0));

    const VectorXd x{1.0, 2.0};
    const VectorXd y{1.0, 2.0, 3.0};

    REQUIRE_THROWS(x.dot(y));
}

TEST_CASE("Vector cross product computes expected 3D result", "[Core][Vector]")
{
    const Vector3d x_axis{1.0, 0.0, 0.0};
    const Vector3d y_axis{0.0, 1.0, 0.0};
    const Vector3d z_axis{0.0, 0.0, 1.0};

    requireVectorApprox(x_axis.cross(y_axis), {0.0, 0.0, 1.0});
    requireVectorApprox(y_axis.cross(x_axis), {0.0, 0.0, -1.0});
    requireVectorApprox(x_axis.cross(z_axis), {0.0, -1.0, 0.0});
}

TEST_CASE("Vector cross product asserts for non-3D vectors", "[Core][Vector]")
{
    const VectorXd a{1.0, 2.0};
    const VectorXd b{3.0, 4.0};

    REQUIRE_THROWS(a.cross(b));
}

TEST_CASE("Vector projection onto vector computes expected result", "[Core][Vector]")
{
    const Vector3d vector{3.0, 4.0, 0.0};
    const Vector3d x_axis{1.0, 0.0, 0.0};
    const Vector3d y_axis{0.0, 1.0, 0.0};

    requireVectorApprox(vector.projectOnto(x_axis), {3.0, 0.0, 0.0});
    requireVectorApprox(vector.projectOnto(y_axis), {0.0, 4.0, 0.0});

    const Vector3d zero{0.0, 0.0, 0.0};
    REQUIRE_THROWS(vector.projectOnto(zero));
}

TEST_CASE("Vector projection onto plane computes expected result", "[Core][Vector]")
{
    const Vector3d vector{3.0, 4.0, 5.0};
    const Vector3d z_normal{0.0, 0.0, 1.0};

    requireVectorApprox(vector.projectOntoPlane(z_normal), {3.0, 4.0, 0.0});
}

TEST_CASE("Vector distance and squared distance compute expected values", "[Core][Vector]")
{
    const Vector3d a{1.0, 2.0, 3.0};
    const Vector3d b{4.0, 6.0, 3.0};

    REQUIRE(a.distanceTo(b) == Catch::Approx(5.0));
    REQUIRE(a.squaredDistanceTo(b) == Catch::Approx(25.0));
}

//----------------------------------------------------------------------------------------------------------------------
// Norms and reductions
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Vector norm, squaredNorm, length, and lengthSquared compute expected values", "[Core][Vector]")
{
    const Vector3d vector{3.0, 4.0, 12.0};

    REQUIRE(vector.squaredNorm() == Catch::Approx(169.0));
    REQUIRE(vector.norm() == Catch::Approx(13.0));
    REQUIRE(vector.lengthSquared() == Catch::Approx(169.0));
    REQUIRE(vector.length() == Catch::Approx(13.0));
}

TEST_CASE("Vector min, max, sum, product, and mean compute expected values", "[Core][Vector]")
{
    const Vector4d vector{1.0, -2.0, 3.0, 4.0};

    REQUIRE(vector.min() == Catch::Approx(-2.0));
    REQUIRE(vector.max() == Catch::Approx(4.0));
    REQUIRE(vector.sum() == Catch::Approx(6.0));
    REQUIRE(vector.product() == Catch::Approx(-24.0));
    REQUIRE(vector.mean() == Catch::Approx(1.5));
}

TEST_CASE("Vector reductions assert when mathematically undefined on empty dynamic vector", "[Core][Vector]")
{
    const VectorXd empty;

    REQUIRE_THROWS(empty.min());
    REQUIRE_THROWS(empty.max());
    REQUIRE_THROWS(empty.mean());
}

TEST_CASE("Vector normalize and normalized produce unit-length vectors", "[Core][Vector]")
{
    Vector3d vector{3.0, 4.0, 0.0};

    Vector3d normalized_copy = vector.normalized();

    requireVectorApprox(normalized_copy, {0.6, 0.8, 0.0});
    REQUIRE(normalized_copy.norm() == Catch::Approx(1.0));

    vector.normalize();

    requireVectorApprox(vector, {0.6, 0.8, 0.0});
    REQUIRE(vector.norm() == Catch::Approx(1.0));

    Vector3d zero{0.0, 0.0, 0.0};
    REQUIRE_THROWS(zero.normalize());
    REQUIRE_THROWS(zero.normalized());
}

//----------------------------------------------------------------------------------------------------------------------
// Fill utilities and factories
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Vector fill and setToZero modify all entries", "[Core][Vector]")
{
    Vector4d vector{1.0, 2.0, 3.0, 4.0};

    vector.fill(7.0);
    requireVectorApprox(vector, {7.0, 7.0, 7.0, 7.0});

    vector.setToZero();
    requireVectorApprox(vector, {0.0, 0.0, 0.0, 0.0});
}

TEST_CASE("StaticVector zeros and ones factories create expected vectors", "[Core][Vector]")
{
    const Vector3d zeros = Vector3d::zeros();
    const Vector3d ones = Vector3d::ones();

    requireVectorApprox(zeros, {0.0, 0.0, 0.0});
    requireVectorApprox(ones, {1.0, 1.0, 1.0});
}

TEST_CASE("DynamicVector zeros and ones factories create expected vectors", "[Core][Vector]")
{
    const VectorXd zeros = VectorXd::zeros(4);
    const VectorXd ones = VectorXd::ones(4);

    requireVectorApprox(zeros, {0.0, 0.0, 0.0, 0.0});
    requireVectorApprox(ones, {1.0, 1.0, 1.0, 1.0});
}

//----------------------------------------------------------------------------------------------------------------------
// Elementwise vector-vector operations
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Vector elementwise product, quotient, min, max, and pow are correct", "[Core][Vector]")
{
    const Vector3d a{2.0, 4.0, 8.0};
    const Vector3d b{1.0, 2.0, 3.0};

    requireVectorApprox(a.elementWise().product(b), {2.0, 8.0, 24.0});
    requireVectorApprox(a.elementWise().quotient(b), {2.0, 2.0, 8.0 / 3.0});
    requireVectorApprox(a.elementWise().min(b), {1.0, 2.0, 3.0});
    requireVectorApprox(a.elementWise().max(b), {2.0, 4.0, 8.0});
    requireVectorApprox(a.elementWise().pow(b), {2.0, 16.0, 512.0});
}

TEST_CASE("Vector elementwise vector-vector operations assert on mismatched sizes", "[Core][Vector]")
{
    const VectorXd a{1.0, 2.0};
    const VectorXd b{1.0, 2.0, 3.0};

    REQUIRE_THROWS(a.elementWise().product(b));
    REQUIRE_THROWS(a.elementWise().quotient(b));
    REQUIRE_THROWS(a.elementWise().min(b));
    REQUIRE_THROWS(a.elementWise().max(b));
    REQUIRE_THROWS(a.elementWise().pow(b));
}

TEST_CASE("Vector elementwise quotient asserts on division by zero", "[Core][Vector]")
{
    const Vector3d a{1.0, 2.0, 3.0};
    const Vector3d b{1.0, 0.0, 3.0};

    REQUIRE_THROWS(a.elementWise().quotient(b));
}

//----------------------------------------------------------------------------------------------------------------------
// Elementwise unary operations
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Vector elementwise abs, sqrt, square, cube, and inverse are correct", "[Core][Vector]")
{
    const Vector3d vector{-4.0, 9.0, 16.0};

    requireVectorApprox(vector.elementWise().abs(), {4.0, 9.0, 16.0});
    requireVectorApprox(vector.elementWise().square(), {16.0, 81.0, 256.0});
    requireVectorApprox(vector.elementWise().cube(), {-64.0, 729.0, 4096.0});

    const Vector3d positive{4.0, 9.0, 16.0};
    requireVectorApprox(positive.elementWise().sqrt(), {2.0, 3.0, 4.0});

    const Vector3d nonzero{2.0, 4.0, 8.0};
    requireVectorApprox(nonzero.elementWise().inverse(), {0.5, 0.25, 0.125});
}

TEST_CASE("Vector elementwise sqrt and inverse assert on invalid domains", "[Core][Vector]")
{
    const Vector3d with_negative{1.0, -1.0, 4.0};
    const Vector3d with_zero{1.0, 0.0, 4.0};

    REQUIRE_THROWS(with_negative.elementWise().sqrt());
    REQUIRE_THROWS(with_zero.elementWise().inverse());
}

TEST_CASE("Vector elementwise exp, log, and log10 are correct", "[Core][Vector]")
{
    const Vector3d vector{1.0, 10.0, 100.0};

    requireVectorApprox(vector.elementWise().log(), {std::log(1.0), std::log(10.0), std::log(100.0)});
    requireVectorApprox(vector.elementWise().log10(), {0.0, 1.0, 2.0});
    requireVectorApprox(vector.elementWise().exp(), {std::exp(1.0), std::exp(10.0), std::exp(100.0)}, 1e-8);
}

TEST_CASE("Vector elementwise log and log10 assert on invalid domains", "[Core][Vector]")
{
    const Vector3d with_zero{1.0, 0.0, 2.0};
    const Vector3d with_negative{1.0, -1.0, 2.0};

    REQUIRE_THROWS(with_zero.elementWise().log());
    REQUIRE_THROWS(with_zero.elementWise().log10());

    REQUIRE_THROWS(with_negative.elementWise().log());
    REQUIRE_THROWS(with_negative.elementWise().log10());
}

TEST_CASE("Vector elementwise trigonometric operations are correct", "[Core][Vector]")
{
    const Vector3d vector{0.0, 0.5, 1.0};

    requireVectorApprox(vector.elementWise().sin(), {std::sin(0.0), std::sin(0.5), std::sin(1.0)});
    requireVectorApprox(vector.elementWise().cos(), {std::cos(0.0), std::cos(0.5), std::cos(1.0)});
    requireVectorApprox(vector.elementWise().tan(), {std::tan(0.0), std::tan(0.5), std::tan(1.0)});
}

TEST_CASE("Vector elementwise scalar pow, clamp, min, and max are correct", "[Core][Vector]")
{
    const Vector3d vector{-2.0, 3.0, 5.0};

    requireVectorApprox(vector.elementWise().pow(2.0), {4.0, 9.0, 25.0});
    requireVectorApprox(vector.elementWise().clamp(0.0, 4.0), {0.0, 3.0, 4.0});
    requireVectorApprox(vector.elementWise().min(2.0), {-2.0, 2.0, 2.0});
    requireVectorApprox(vector.elementWise().max(2.0), {2.0, 3.0, 5.0});
}

//----------------------------------------------------------------------------------------------------------------------
// Finite / NaN checks
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Vector allFinite and hasNaN report correct states", "[Core][Vector]")
{
    const Vector3d finite{1.0, 2.0, 3.0};
    REQUIRE(finite.allFinite());
    REQUIRE_FALSE(finite.hasNaN());

    const Vector3d with_infinity{1.0, std::numeric_limits<double>::infinity(), 3.0};
    REQUIRE_FALSE(with_infinity.allFinite());
    REQUIRE_FALSE(with_infinity.hasNaN());

    const Vector3d with_nan{1.0, std::numeric_limits<double>::quiet_NaN(), 3.0};
    REQUIRE_FALSE(with_nan.allFinite());
    REQUIRE(with_nan.hasNaN());
}

//----------------------------------------------------------------------------------------------------------------------
// Large-vector smoke tests
//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("DynamicVector operations work on large vectors", "[Core][Vector]")
{
    constexpr std::size_t num_values = 100000;

    VectorXd a(num_values, 2.0);
    VectorXd b(num_values, 3.0);

    REQUIRE(a.size() == num_values);
    REQUIRE(b.size() == num_values);

    VectorXd c = a + b;
    REQUIRE(c.size() == num_values);

    REQUIRE(c[0] == Catch::Approx(5.0));
    REQUIRE(c[num_values / 2] == Catch::Approx(5.0));
    REQUIRE(c[num_values - 1] == Catch::Approx(5.0));

    c *= 2.0;

    REQUIRE(c[0] == Catch::Approx(10.0));
    REQUIRE(c[num_values / 2] == Catch::Approx(10.0));
    REQUIRE(c[num_values - 1] == Catch::Approx(10.0));

    REQUIRE(a.dot(b) == Catch::Approx(static_cast<double>(num_values) * 6.0));
    REQUIRE(a.sum() == Catch::Approx(static_cast<double>(num_values) * 2.0));
    REQUIRE(b.mean() == Catch::Approx(3.0));
}
