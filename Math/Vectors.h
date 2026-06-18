//-----------------------------------------------------------------------------------------------------------------------
// Vectors.h
//
// Core vector utilities for InfinityLearn.
//-----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <Core/Error.h>           // IL_CHECK
#include <Core/Parallelization.h> // Parallel::Range, Parallel::parallelFor, etc
#include <algorithm>              // std::clamp, std::max, std::min
#include <cmath>                  // std::abs, std::cos, std::exp, std::isfinite, std::isnan, std::log, std::log10,
                                  // std::pow, std::sin, std::sqrt, std::tan
#include <initializer_list>       // std::initializer_list
#include <type_traits>            //std::is_floating_point_v<
#include <utility>                // std::move, std::forward
#include <vector>                 // std::vector

namespace InfinityLearn
{

//-----------------------------------------------------------------------------------------------------------------------
// The base vector class that all other vector types will inherit from.
//
// A base vector class that only contains the operations we want in a storage agnostic fashion.
// I will be using CRTP structure to extend this structure to have both static sized vectors and dynamic sized vectors,
// and the user will interact with the derived classes instead of this base class directly. This allows us to have a
// common interface for both static and dynamic vectors, while still allowing for the performance benefits of static
// vectors when the size is known at compile time. The Derived template parameter is the type of the derived class that
// inherits from this base class, and the T template parameter is the type of the elements in the vector (e.g. float,
// double, int, etc.).
//-----------------------------------------------------------------------------------------------------------------------
template <typename Derived, typename T>
class VectorBase
{
   public:
    //------------------
    // Basic Operators
    //------------------

    /// <summary>
    /// Equality operator that checks if two vectors are equal.
    /// </summary>
    /// <param name="other">The other vector to compare against</param>
    /// <returns>True if the vectors are equal, false otherwise</returns>
    bool operator==(const Derived& other) const;


    //-----------------
    // Accessors
    //-----------------

    /// <summary>
    /// Access element at index.
    /// This is bounds checked, and will throw an exception if the index is out of bounds. The original vector is not
    /// modified by this operation.
    /// </summary>
    /// <param name="index">The index to access at</param>
    /// <returns>The value of the vector at that index</returns>
    T& operator[](std::size_t index);

    /// <summary>
    /// Const accessor for element at index.
    /// This is bounds checked, and will throw an exception if the index is out of bounds. The original vector is not
    /// modified by this operation.
    /// </summary>
    /// <param name="index">The index to access at</param>
    /// <returns>The value of the vector at that index</returns>
    const T& operator[](std::size_t index) const;

    /// <summary>
    /// Return a pointer to the underlying data array of the vector. This allows for interoperability with APIs that
    /// expect a raw pointer to an array of values. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>A pointer to the underlying data</returns>
    T* data();

    /// <summary>
    /// Return a const pointer to the underlying data array of the vector. This allows for interoperability with APIs
    /// that expect a raw pointer to an array of values. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>A pointer to the underlying data</returns>
    const T* data() const;


    //-------------------
    // Named Accessors
    //-------------------

    /// <summary>
    /// Return the x value of the vector. This is a named accessor for the first element of the vector, and is only
    /// valid if the size of the vector is at least 1. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>The x value of the vector</returns>
    T& x();

    /// <summary>
    /// Return the y value of the vector. This is a named accessor for the second element of the vector, and is only
    /// valid if the size of the vector is at least 2. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>The y value of the vector</returns>
    T& y();

    /// <summary>
    /// Return the z value of the vector. This is a named accessor for the third element of the vector, and is only
    /// valid if the size of the vector is at least 3. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>The z value of the vector</returns>
    T& z();

    /// <summary>
    /// Return the w value of the vector. This is a named accessor for the fourth element of the vector, and is only
    /// valid if the size of the vector is at least 4. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>The w value of the vector</returns>
    T& w();

    /// <summary>
    /// Return the x value of the vector. This is a named accessor for the first element of the vector, and is only
    /// valid if the size of the vector is at least 1. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>The x value of the vector</returns>
    const T& x() const;

    /// <summary>
    /// Return the y value of the vector. This is a named accessor for the second element of the vector, and is only
    /// valid if the size of the vector is at least 2. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>The y value of the vector</returns>
    const T& y() const;

    /// <summary>
    /// Return the z value of the vector. This is a named accessor for the third element of the vector, and is only
    /// valid if the size of the vector is at least 3. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>The z value of the vector</returns>
    const T& z() const;

    /// <summary>
    /// Return the w value of the vector. This is a named accessor for the fourth element of the vector, and is only
    /// valid if the size of the vector is at least 4. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>The w value of the vector</returns>
    const T& w() const;

    //-----------------------
    // Basic Math Operations
    //-----------------------

    /// <summary>
    /// Add two vectors together element wise and return the result as a new vector. The original vectors are not
    /// modified.
    /// </summary>
    /// <param name="other">The other vector to be added.</param>
    /// <returns>The resulting vector after addition</returns>
    Derived operator+(const Derived& other) const;

    /// <summary>
    /// Add two vectors together element wise and store the result in this vector. The other vector is not modified.
    /// </summary>
    /// <param name="other">The other vector to be accumulated into this one.</param>
    /// <returns>A reference to this vector after the operation</returns>
    Derived& operator+=(const Derived& other);

    /// <summary>
    /// Subtract two vectors element wise and return the result as a new vector. The original vectors are not modified.
    /// </summary>
    /// <param name="other">The other vector to be subtracted.</param>
    /// <returns>The resulting vector after subtraction</returns>
    Derived operator-(const Derived& other) const;

    /// <summary>
    /// Subtract two vectors element wise and store the result in this vector. The other vector is not modified.
    /// </summary>
    /// <param name="other">The other vector to be subtracted.</param>
    /// <returns>A reference to this vector after the operation</returns>
    Derived& operator-=(const Derived& other);

    /// <summary>
    /// Multiply a vector by a scalar value and return the result as a new vector. The original vector is not modified.
    /// </summary>
    /// <param name="scalar">The scalar value to multiply by.</param>
    /// <returns>The resulting vector after multiplication</returns>
    Derived operator*(const T& scalar) const;

    /// <summary>
    /// Divide a vector by a scalar value and return the result as a new vector. The original vector is not modified.
    /// </summary>
    /// <param name="scalar">The scalar value to divide by.</param>
    /// <returns>The resulting vector after division</returns>
    Derived operator/(const T& scalar) const;

    /// <summary>
    /// Scale a vector element wise by a scalar value.
    /// </summary>
    /// <param name="scalar">The scalar value to be multiplied by</param>
    /// <returns>A reference to this vector after the operation</returns>
    Derived& operator*=(const T& scalar);

    /// <summary>
    /// Scale a vector element wise by the reciprocal of a scalar value.
    /// </summary>
    /// <param name="scalar">The scalar value to be divided by</param>
    /// <returns>A reference to this vector after the operation</returns>
    Derived& operator/=(const T& scalar);

    /// <summary>
    /// Negate a vector element wise and return the result as a new vector. The original vector is not modified.
    /// </summary>
    /// <returns></returns>
    Derived operator-() const;


    //-------------------------
    // Complex Math Operations
    //-------------------------

    /// <summary>
    /// Compute the dot product of this vector with another vector and return the result as a scalar value. The original
    /// vectors are not modified.
    /// </summary>
    /// <param name="other">The other vector to compute the dot product with</param>
    /// <returns>The dot product of the two vectors</returns>
    T dot(const Derived& other) const;

    /// <summary>
    /// Compute the cross product of this vector with another vector and return the result as a new vector. The original
    /// vectors are not modified. Is only valid for 3D vectors, and will throw otherwise.
    /// </summary>
    /// <param name="other">The other vector to compute the cross product with</param>
    /// <returns></returns>
    Derived cross(const Derived& other) const;

    /// <summary>
    /// Project this vector onto another vector and return the result as a new vector. The original vectors are not
    /// modified.
    /// </summary>
    /// <param name="other">The other vector to project onto</param>
    /// <returns>The projection of this vector onto the other vector</returns>
    Derived projectOnto(const Derived& other) const;

    /// <summary>
    /// Project this vector onto a plane defined by a normal vector and return the result as a new vector. The original
    /// vectors are not modified.
    /// </summary>
    /// <param name="planeNormal">The normal vector of the plane to project onto</param>
    /// <returns>The projection of this vector onto the plane</returns>
    Derived projectOntoPlane(const Derived& planeNormal) const;

    /// <summary>
    /// Find the distance between this vector and another vector and return it as a scalar value. The original vectors
    /// are not modified by this operation.
    /// </summary>
    /// <param name="other">The vector to which the distance is calculated</param>
    /// <returns>The distance between the two vectors</returns>
    double distanceTo(const Derived& other) const;

    /// <summary>
    /// Find the squared distance between this vector and another vector and return it as a scalar value. The original
    /// vectors are not modified by this operation.
    /// </summary>
    /// <param name="other">The vector to which the squared distance is calculated</param>
    /// <returns>The squared distance between the two vectors</returns>
    T squaredDistanceTo(const Derived& other) const;


    //-------------------------------------
    // Vector Properties and Manipulations
    //-------------------------------------

    /// <summary>
    /// Get the size of the vector.
    /// </summary>
    /// <returns>The size of the vector</returns>
    std::size_t size() const;

    /// <summary>
    /// Check if the vector is empty.
    /// </summary>
    /// <returns>True if the vector is empty, false otherwise</returns>
    bool empty() const;

    /// <summary>
    /// Find the L2 norm of this vector and return it as a scalar value. The original vector is not modified.
    /// </summary>
    /// <returns>The L2 norm</returns>
    double norm() const;

    /// <summary>
    /// Find the squared L2 norm of this vector and return it as a scalar value. The original vector is not modified.
    /// </summary>
    /// <returns>The squared L2 norm</returns>
    T squaredNorm() const;

    /// <summary>
    /// Find the length of this vector and return it as a scalar value. The original vector is not modified. This is the
    /// same as the norm, but is provided as a separate function for clarity and convenience.
    /// </summary>
    /// <returns>The length of the vector</returns>
    double length() const;

    /// <summary>
    /// Find the squared length of this vector and return it as a scalar value. The original vector is not modified.
    /// This is the same as the squared norm, but is provided as a separate function for clarity and convenience.
    /// </summary>
    /// <returns>The squared length of the vector</returns>
    double lengthSquared() const;

    /// <summary>
    /// Find the largest element in the vector and return it as a scalar value. The original vector is not modified.
    /// </summary>
    /// <returns>The largest element in the vector</returns>
    T max() const;

    /// <summary>
    /// Find the smallest element in the vector and return it as a scalar value. The original vector is not modified.
    /// </summary>
    /// <returns>The smallest element in the vector</returns>
    T min() const;

    /// <summary>
    /// Normalize the vector in place, scaling it to have a norm of 1. The original vector is modified by this
    /// operation.
    /// </summary>
    void normalize();

    /// <summary>
    /// Return a normalized copy of the vector, scaling it to have a norm of 1. The original vector is not modified.
    /// </summary>
    /// <returns>The normalized vector</returns>
    Derived normalized() const;

    /// <summary>
    /// Calculate the sum of all the entries in this vector and return it as a scalar value. The original vector is not
    /// modified.
    /// </summary>
    /// <returns>The sum of the vector entries</returns>
    T sum() const;

    /// <summary>
    /// Find the product of all the entries in this vector and return it as a scalar value. The original vector is not
    /// modified.
    /// </summary>
    /// <returns>The product of the vector entries</returns>
    T product() const;

    /// <summary>
    /// Find the mean of all the entries in this vector and return it as a scalar value. The original vector is not
    /// modified.
    /// </summary>
    /// <returns>The mean of the vector entries</returns>
    T mean() const;


    //----------------------
    // Fill utilities
    //----------------------

    /// <summary>
    /// Fill the vector with a constant value.
    /// </summary>
    /// <param name="value">The value to be filled</param>
    void fill(const T& value);


    /// <summary>
    /// Zero out all the entries in the vector.
    /// </summary>
    void setToZero();

    //------------------------
    // Elementwise Operations
    //------------------------

    /// <summary>
    /// A view into the elements of a vector for elementwise operations.
    /// </summary>
    class ElementWiseView
    {
       public:
        explicit ElementWiseView(const Derived& v) : m_v(v)
        {
        }

        /// <summary>
        /// Compute the elementwise product of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise product of the two vectors</returns>
        Derived product(const Derived& other) const;

        /// <summary>
        /// Compute the elementwise quotient of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise quotient of the two vectors</returns>
        Derived quotient(const Derived& other) const;

        /// <summary>
        /// Compute the elementwise minimum of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise minimum of the two vectors</returns>
        Derived min(const Derived& other) const;

        /// <summary>
        /// Compute the elementwise maximum of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise maximum of the two vectors</returns>
        Derived max(const Derived& other) const;

        /// <summary>
        /// Compute the elementwise power of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise power of the two vectors</returns>
        Derived pow(const Derived& other) const;

        /// <summary>
        /// Compute the elementwise absolute value of this vector.
        /// </summary>
        /// <returns>The elementwise absolute value of the vector</returns>
        Derived abs() const;

        /// <summary>
        /// Compute the elementwise square root of this vector.
        /// </summary>
        /// <returns>The elementwise square root of the vector</returns>
        Derived sqrt() const;

        /// <summary>
        /// Compute the elementwise square of this vector.
        /// </summary>
        /// <returns>The elementwise square of the vector</returns>
        Derived square() const;

        /// <summary>
        /// Compute the elementwise cube of this vector.
        /// </summary>
        /// <returns>The elementwise cube of the vector</returns>
        Derived cube() const;

        /// <summary>
        /// Compute the elementwise reciprocal of this vector.
        /// </summary>
        /// <returns>The elementwise reciprocal of the vector</returns>
        Derived inverse() const;

        /// <summary>
        /// Compute the elementwise exponential of this vector.
        /// </summary>
        /// <returns>The elementwise exponential of the vector</returns>
        Derived exp() const;

        /// <summary>
        /// Compute the elementwise natural logarithm of this vector.
        /// </summary>
        /// <returns>The elementwise natural logarithm of the vector</returns>
        Derived log() const;

        /// <summary>
        /// Compute the elementwise base-10 logarithm of this vector.
        /// </summary>
        /// <returns>The elementwise base-10 logarithm of the vector</returns>
        Derived log10() const;

        /// <summary>
        /// Compute the elementwise sine of this vector. The sine function is applied to each element of the vector
        /// independently, and the result is a new vector containing the sine of each element. The original vector is
        /// not modified by this operation.
        /// </summary>
        /// <returns>The elementwise sine of the vector</returns>
        Derived sin() const;

        /// <summary>
        /// Compute the elementwise cosine of this vector. The cosine function is applied to each element of the vector
        /// independently, and the result is a new vector containing the cosine of each element. The original vector is
        /// not modified by this operation.
        /// </summary>
        /// <returns>The elementwise cosine of the vector</returns>
        Derived cos() const;

        /// <summary>
        /// Compute the elementwise tangent of this vector. The tangent function is applied to each element of the
        /// vector independently, and the result is a new vector containing the tangent of each element. The original
        /// vector is not modified by this operation.
        /// </summary>
        /// <returns>The elementwise tangent of the vector</returns>
        Derived tan() const;

        /// <summary>
        /// Compute the elementwise power of this vector with a scalar exponent.
        /// </summary>
        /// <param name="exponent">The exponent to raise each element to</param>
        /// <returns>The elementwise power of the vector</returns>
        Derived pow(T exponent) const;

        /// <summary>
        /// Clamp each element of this vector to be within the specified minimum and maximum values. The original vector
        /// is not modified by this operation, and the returned vector is a new vector containing the clamped values.
        /// </summary>
        /// <param name="minValue">The minimum value to clamp to</param>
        /// <param name="maxValue">The maximum value to clamp to</param>
        /// <returns>The clamped vector</returns>
        Derived clamp(T minValue, T maxValue) const;

        /// <summary>
        /// Compute the elementwise minimum of this vector with a scalar value, returning a new vector where each
        /// element is the minimum of the corresponding element in this vector and the scalar value. The original vector
        /// is not modified by this operation.
        /// </summary>
        /// <param name="value">The scalar value to compare against</param>
        /// <returns>The elementwise minimum of the vector</returns>
        Derived min(T value) const;

        /// <summary>
        /// Compute the elementwise maximum of this vector with a scalar value, returning a new vector where each
        /// element is the maximum of the corresponding element in this vector and the scalar value. The original vector
        /// is not modified by this operation.
        /// </summary>
        /// <param name="value">The scalar value to compare against</param>
        /// <returns>The elementwise maximum of the vector</returns>
        Derived max(T value) const;

       private:
        const Derived& m_v;
    };

    /// <summary>
    /// Get a view into the elements of this vector for elementwise operations.
    /// </summary>
    /// <returns>An ElementWiseView representation of this vector</returns>
    ElementWiseView elementWise() const;

    //----------------------
    // General utilities
    //----------------------

    /// <summary>
    /// Check if this vector is approximately equal to another vector within a given tolerance. The original vectors are
    /// not modified by this operation.
    /// </summary>
    /// <param name="other">The other vector to compare against</param>
    /// <param name="tolerance">The tolerance within which to consider the vectors equal</param>
    /// <returns>True if the vectors are approximately equal, false otherwise</returns>
    bool isApprox(const Derived& other, double tolerance = 1e-9) const;

    /// <summary>
    /// Check if all elements in the vector are finite (not NaN or infinite).
    /// </summary>
    /// <returns>True if all elements are finite, false otherwise</returns>
    bool allFinite() const;

    /// <summary>
    /// Check if any element in the vector is NaN (not a number).
    /// </summary>
    /// <returns>True if any element is NaN, false otherwise</returns>
    bool hasNaN() const;


   protected:
    //------------------------------
    // Constructors And Destructors
    //------------------------------

    /// <summary>
    /// Default Constructor
    /// </summary>
    VectorBase() = default;

    /// <summary>
    /// A default destructor that will clean up the vector when it goes out of scope.
    /// </summary>
    ~VectorBase() = default;

    /// <summary>
    /// Default copy constructor that will copy the values from another vector into this vector. The original vector is
    /// not modified by this operation, and the returned vector is a new vector containing the copied values. Since this
    /// class only contains a fixed-size array, the default copy constructor is sufficient for copying the vector.
    /// </summary>
    /// <param name="other">The vector to copy from</param>
    VectorBase(const VectorBase&) = default;

    /// <summary>
    /// Default move constructor that will move the values from another vector into this vector. The other vector will
    /// be left in a valid but unspecified state. Since this class only contains a fixed-size array, the default move
    /// constructor is sufficient for moving the vector.
    /// </summary>
    /// <param name="other">The vector to move from</param>
    VectorBase(VectorBase&&) noexcept = default;


    /// <summary>
    /// Default copy assignment operator that will copy the values from another vector into this vector. The original
    /// vector is not modified by this operation, and the returned vector is a new vector containing the copied values.
    /// Since this class only contains a fixed-size array, the default copy assignment operator is sufficient for
    /// copying the vector.
    /// </summary>
    /// <param name="other">The vector to copy from</param>
    /// <returns></returns>
    VectorBase& operator=(const VectorBase&) = default;


    /// <summary>
    /// Default move assignment operator that will move the values from another vector into this vector. The other
    /// vector will be left in a valid but unspecified state. Since this class only contains a fixed-size array, the
    /// default move assignment operator is sufficient for moving the vector.
    /// </summary>
    /// <param name="other">The vector to move from</param>
    /// <returns></returns>
    VectorBase& operator=(VectorBase&&) noexcept = default;


    /// <summary>
    /// Get a reference to the derived class instance.
    /// </summary>
    /// <returns>A reference to the derived class instance</returns>
    Derived& derived()
    {
        return static_cast<Derived&>(*this);
    }

    /// <summary>
    /// Get a const reference to the derived class instance.
    /// </summary>
    /// <returns>A const reference to the derived class instance</returns>
    const Derived& derived() const
    {
        return static_cast<const Derived&>(*this);
    }
};

/// <summary>
/// Multiply a vector by a scalar value.
/// </summary>
/// <param name="scalar">The scalar value to multiply by</param>
/// <param name="v">The vector to multiply</param>
/// <returns>The resulting vector after multiplication</returns>
template <typename Derived, typename T>
Derived operator*(const T& scalar, const VectorBase<Derived, T>& v)
{
    return static_cast<const Derived&>(v) * scalar;
}


//----------------------
// Derived Vector Types
//----------------------

// This is the class that describes static vectors, ones where we know the size at compile time. This class inherits
// from the VectorBase class, and provides the necessary implementations that the base class assumes exists. The
// template parameters are T, which is the type of the elements in the vector (e.g. float, double, int, etc.), and N,
// which is the size of the vector (i.e. the number of elements). The static vector will have a fixed size of N, and
// will store its data in a fixed-size array of type T. The user can create a static vector by specifying the type and
// size as template parameters, and can initialize it using an initializer list or by default construction (which will
// initialize all elements to zero).
template <typename T, std::size_t N>
class StaticVector : public VectorBase<StaticVector<T, N>, T>
{
   public:
    /// <summary>
    /// Plain constructor that initializes all the elements to zero
    /// </summary>
    StaticVector() : m_data{}
    {
    }

    /// <summary>
    /// Constructor that initializes the vector with the given values. The number of values provided must match the size
    /// of the vector.
    /// </summary>
    /// <param name="values">The values to initialize the vector with.</param>
    StaticVector(std::initializer_list<T> values) : m_data{}
    {
        IL_CHECK(values.size() == N, "StaticVector initializer list must match vector size.");
        const T* valuesData = values.begin();
        Parallel::isolatedParallelFor(0, N, [&](const Parallel::Index& i) { m_data[i] = valuesData[i]; });
    }

    /// <summary>
    /// Find the size of the vector
    /// </summary>
    /// <returns>The size of the vector</returns>
    std::size_t size() const
    {
        return N;
    }

    /// <summary>
    /// Find a coefficient of the vector at a given index. This is a bounds checked operation, and will throw an
    /// exception if the index is out of bounds. The original vector is not modified by this operation.
    /// </summary>
    /// <param name="index">The index to look at.</param>
    /// <returns>The value of the vector at that index.</returns>
    T& coeff(std::size_t index)
    {
        IL_CHECK(index < N, "Attempting to index a vector outside of its size.");
        return m_data[index];
    }

    /// <summary>
    /// Find a const coefficient of the vector at a given index. This is a bounds checked operation, and will throw an
    /// exception if the index is out of bounds. The original vector is not modified by this operation.
    /// </summary>
    /// <param name="index">The index to look at.</param>
    /// <returns>A const reference to that value.</returns>
    const T& coeff(std::size_t index) const
    {
        IL_CHECK(index < N, "Attempting to index a vector outside of its size.");
        return m_data[index];
    }

    /// <summary>
    /// Return a pointer to the underlying data array of the vector. This allows for interoperability with APIs that
    /// expect a raw pointer to an array of values. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>A pointer to the raw underlying data.</returns>
    T* dataImpl()
    {
        return m_data;
    }

    /// <summary>
    /// Return a const pointer to the underlying data array of the vector. This allows for interoperability with APIs
    /// that expect a raw pointer to an array of values. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>A const pointer to the underlying data.</returns>
    const T* dataImpl() const
    {
        return m_data;
    }

    /// <summary>
    /// Create a copy of this vector with all entries set to zero. The original vector is not modified, and the returned
    /// vector is a new vector with all entries set to zero.
    /// </summary>
    /// <returns>A vector with the same length as the current one, but all entries set to 0.</returns>
    static StaticVector zeros()
    {
        return StaticVector{};
    }

    /// <summary>
    /// Create a copy of this vector with all entries set to one. The original vector is not modified, and the returned
    /// vector is a new vector with all entries set to one.
    /// </summary>
    /// <returns>A vector with the same length as the current one, but all entries set to 1.</returns>
    static StaticVector ones()
    {
        StaticVector result;
        result.fill(T{1});
        return result;
    }

    /// <summary>
    /// Equality operator that checks if two static vectors are exactly equal.
    /// </summary>
    /// <param name="other">The vector to compare against.</param>
    /// <returns>True if both vectors have identical entries.</returns>
    bool operator==(const StaticVector& other) const
    {
        return VectorBase<StaticVector<T, N>, T>::operator==(other);
    }

    /// <summary>
    /// Inequality operator that checks if two static vectors are not exactly equal.
    /// </summary>
    /// <param name="other">The vector to compare against.</param>
    /// <returns>True if the vectors differ.</returns>
    bool operator!=(const StaticVector& other) const
    {
        return !(*this == other);
    }

   private:
    T m_data[N];
};

// This is the class that describes dynamic vectors, ones where the size is determined at runtime. This class inherits
// from the VectorBase class, and provides the necessary implementations that the base class assumes exists. The
// template parameter T is the type of the elements in the vector (e.g. float, double, int, etc.). The dynamic vector
// stores its data in a std::vector<T>, and can grow or shrink after construction.
template <typename T>
class DynamicVector : public VectorBase<DynamicVector<T>, T>
{
   public:
    /// <summary>
    /// Default constructor that creates an empty dynamic vector.
    /// </summary>
    DynamicVector() = default;

    /// <summary>
    /// Constructor that creates a dynamic vector with a requested size. The elements are value-initialized.
    /// </summary>
    /// <param name="size">The number of elements in the vector.</param>
    explicit DynamicVector(std::size_t size) : m_data(size)
    {
    }

    /// <summary>
    /// Constructor that creates a dynamic vector with a requested size, and initializes each element to a requested
    /// value.
    /// </summary>
    /// <param name="size">The number of elements in the vector.</param>
    /// <param name="value">The value used to initialize every element.</param>
    DynamicVector(std::size_t size, const T& value) : m_data(size)
    {
        Parallel::isolatedParallelFor(0, size, [&](const Parallel::Index& i) { m_data[i] = value; });
    }

    /// <summary>
    /// Constructor that initializes the vector with the given values. The size of the vector will match the number of
    /// values provided.
    /// </summary>
    /// <param name="values">The values to initialize the vector with.</param>
    DynamicVector(std::initializer_list<T> values) : m_data(values.size())
    {
        const T* valuesData = values.begin();
        Parallel::isolatedParallelFor(0, values.size(), [&](const Parallel::Index& i) { m_data[i] = valuesData[i]; });
    }

    /// <summary>
    /// Find the size of the vector.
    /// </summary>
    /// <returns>The number of elements in the vector.</returns>
    std::size_t size() const
    {
        return m_data.size();
    }

    /// <summary>
    /// Resize the vector to a new size. If the vector grows, new elements are value-initialized. If the vector shrinks,
    /// elements past the new size are removed.
    /// </summary>
    /// <param name="newSize">The new number of elements in the vector.</param>
    void resize(std::size_t newSize)
    {
        m_data.resize(newSize);
    }

    /// <summary>
    /// Resize the vector to a new size. If the vector grows, new elements are initialized to the provided value. If the
    /// vector shrinks, elements past the new size are removed.
    /// </summary>
    /// <param name="newSize">The new number of elements in the vector.</param>
    /// <param name="value">The value used to initialize any new elements.</param>
    void resize(std::size_t newSize, const T& value)
    {
        const std::size_t oldSize = m_data.size();
        m_data.resize(newSize);

        if (newSize > oldSize)
        {
            Parallel::isolatedParallelFor(oldSize, newSize, [&](const Parallel::Index& i) { m_data[i] = value; });
        }
    }

    /// <summary>
    /// Reserve enough storage for at least the requested number of elements. This changes the capacity of the vector
    /// but does not change its size.
    /// </summary>
    /// <param name="newCapacity">The requested storage capacity.</param>
    void reserve(std::size_t newCapacity)
    {
        m_data.reserve(newCapacity);
    }

    /// <summary>
    /// Find the current storage capacity of the vector.
    /// </summary>
    /// <returns>The number of elements that can be stored without reallocating.</returns>
    std::size_t capacity() const
    {
        return m_data.capacity();
    }

    /// <summary>
    /// Request that the vector reduce its capacity to fit its current size. This may release unused memory, but the
    /// standard library is not required to honor the request.
    /// </summary>
    void shrinkToFit()
    {
        m_data.shrink_to_fit();
    }

    /// <summary>
    /// Add a value to the end of the vector by copying it.
    /// </summary>
    /// <param name="value">The value to append.</param>
    void pushBack(const T& value)
    {
        m_data.push_back(value);
    }

    /// <summary>
    /// Add a value to the end of the vector by moving it.
    /// </summary>
    /// <param name="value">The value to append.</param>
    void pushBack(T&& value)
    {
        m_data.push_back(std::move(value));
    }

    /// <summary>
    /// Construct a new element directly at the end of the vector.
    /// </summary>
    /// <typeparam name="Args">The argument types used to construct the element.</typeparam>
    /// <param name="args">The arguments forwarded to the element constructor.</param>
    /// <returns>A reference to the newly constructed element.</returns>
    template <typename... Args>
    T& emplaceBack(Args&&... args)
    {
        return m_data.emplace_back(std::forward<Args>(args)...);
    }

    /// <summary>
    /// Remove the last element from the vector.
    /// </summary>
    void popBack()
    {
        IL_CHECK(!m_data.empty(), "Cannot pop from an empty DynamicVector.");
        m_data.pop_back();
    }

    /// <summary>
    /// Remove all elements from the vector. This changes the size to zero, but may leave the capacity unchanged.
    /// </summary>
    void clear()
    {
        m_data.clear();
    }

    /// <summary>
    /// Find a coefficient of the vector at a given index. This is a bounds checked operation, and will throw an
    /// exception if the index is out of bounds. The original vector is not modified by this operation.
    /// </summary>
    /// <param name="index">The index to look at.</param>
    /// <returns>The value of the vector at that index.</returns>
    T& coeff(std::size_t index)
    {
        IL_CHECK(index < m_data.size(), "Attempting to index a vector outside of its size.");
        return m_data[index];
    }

    /// <summary>
    /// Find a const coefficient of the vector at a given index. This is a bounds checked operation, and will throw an
    /// exception if the index is out of bounds. The original vector is not modified by this operation.
    /// </summary>
    /// <param name="index">The index to look at.</param>
    /// <returns>A const reference to that value.</returns>
    const T& coeff(std::size_t index) const
    {
        IL_CHECK(index < m_data.size(), "Attempting to index a vector outside of its size.");
        return m_data[index];
    }

    /// <summary>
    /// Return a pointer to the underlying data array of the vector. This allows for interoperability with APIs that
    /// expect a raw pointer to an array of values. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>A pointer to the raw underlying data.</returns>
    T* dataImpl()
    {
        return m_data.data();
    }

    /// <summary>
    /// Return a const pointer to the underlying data array of the vector. This allows for interoperability with APIs
    /// that expect a raw pointer to an array of values. The original vector is not modified by this operation.
    /// </summary>
    /// <returns>A const pointer to the underlying data.</returns>
    const T* dataImpl() const
    {
        return m_data.data();
    }

    /// <summary>
    /// Create a copy of this vector with all entries set to zero. The original vector is not modified, and the returned
    /// vector is a new vector with all entries set to zero.
    /// </summary>
    /// <returns>A vector with the same length as the current one, but all entries set to 0.</returns>
    static DynamicVector zeros(std::size_t size)
    {
        return DynamicVector(size);
    }


    /// <summary>
    /// Create a copy of this vector with all entries set to one. The original vector is not modified, and the returned
    /// vector is a new vector with all entries set to one.
    /// </summary>
    /// <returns>A vector with the same length as the current one, but all entries set to 1.</returns>
    static DynamicVector ones(std::size_t size)
    {
        DynamicVector result(size);
        result.fill(T{1});
        return result;
    }

    /// <summary>
    /// Equality operator that checks if two dynamic vectors are exactly equal.
    /// </summary>
    /// <param name="other">The vector to compare against.</param>
    /// <returns>True if both vectors have the same size and identical entries.</returns>
    bool operator==(const DynamicVector& other) const
    {
        return VectorBase<DynamicVector<T>, T>::operator==(other);
    }

    /// <summary>
    /// Inequality operator that checks if two dynamic vectors are not exactly equal.
    /// </summary>
    /// <param name="other">The vector to compare against.</param>
    /// <returns>True if the vectors differ.</returns>
    bool operator!=(const DynamicVector& other) const
    {
        return !(*this == other);
    }

   private:
    std::vector<T> m_data;
};


//---------------------------------
// Aliases for common vector types
//---------------------------------
using Vector2f = StaticVector<float, 2>;
using Vector3f = StaticVector<float, 3>;
using Vector4f = StaticVector<float, 4>;

using Vector2d = StaticVector<double, 2>;
using Vector3d = StaticVector<double, 3>;
using Vector4d = StaticVector<double, 4>;

using Vector2i = StaticVector<int, 2>;
using Vector3i = StaticVector<int, 3>;
using Vector4i = StaticVector<int, 4>;

using VectorXf = DynamicVector<float>;
using VectorXd = DynamicVector<double>;
using VectorXi = DynamicVector<int>;

//-----------------------------------------------------------------------------------------------------------------------
// VectorBase method definitions
//-----------------------------------------------------------------------------------------------------------------------

template <typename Derived, typename T>
bool VectorBase<Derived, T>::operator==(const Derived& other) const
{
    if (size() != other.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < size(); ++i)
    {
        if (derived().coeff(i) != other.coeff(i))
        {
            return false;
        }
    }

    return true;
}

template <typename Derived, typename T>
T& VectorBase<Derived, T>::operator[](std::size_t index)
{
    return derived().coeff(index);
}

template <typename Derived, typename T>
const T& VectorBase<Derived, T>::operator[](std::size_t index) const
{
    return derived().coeff(index);
}

template <typename Derived, typename T>
T* VectorBase<Derived, T>::data()
{
    return derived().dataImpl();
}

template <typename Derived, typename T>
const T* VectorBase<Derived, T>::data() const
{
    return derived().dataImpl();
}

template <typename Derived, typename T>
T& VectorBase<Derived, T>::x()
{
    return derived().coeff(0);
}

template <typename Derived, typename T>
T& VectorBase<Derived, T>::y()
{
    return derived().coeff(1);
}

template <typename Derived, typename T>
T& VectorBase<Derived, T>::z()
{
    return derived().coeff(2);
}

template <typename Derived, typename T>
T& VectorBase<Derived, T>::w()
{
    return derived().coeff(3);
}

template <typename Derived, typename T>
const T& VectorBase<Derived, T>::x() const
{
    return derived().coeff(0);
}

template <typename Derived, typename T>
const T& VectorBase<Derived, T>::y() const
{
    return derived().coeff(1);
}

template <typename Derived, typename T>
const T& VectorBase<Derived, T>::z() const
{
    return derived().coeff(2);
}

template <typename Derived, typename T>
const T& VectorBase<Derived, T>::w() const
{
    return derived().coeff(3);
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::operator+(const Derived& other) const
{
    Derived result = derived();
    result += other;
    return result;
}

template <typename Derived, typename T>
Derived& VectorBase<Derived, T>::operator+=(const Derived& other)
{
    IL_CHECK(size() == other.size(), "Vector sizes must match for addition.");

    Parallel::isolatedParallelFor(0, size(), [&](const Parallel::Index& i) { derived().coeff(i) += other.coeff(i); });
    return derived();
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::operator-(const Derived& other) const
{
    Derived result = derived();
    result -= other;
    return result;
}

template <typename Derived, typename T>
Derived& VectorBase<Derived, T>::operator-=(const Derived& other)
{
    IL_CHECK(size() == other.size(), "Vector sizes must match for subtraction.");

    Parallel::isolatedParallelFor(0, size(), [&](const Parallel::Index& i) { derived().coeff(i) -= other.coeff(i); });
    return derived();
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::operator*(const T& scalar) const
{
    Derived result = derived();
    result *= scalar;
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::operator/(const T& scalar) const
{
    Derived result = derived();
    result /= scalar;
    return result;
}

template <typename Derived, typename T>
Derived& VectorBase<Derived, T>::operator*=(const T& scalar)
{
    Parallel::isolatedParallelFor(0, size(), [&](const Parallel::Index& i) { derived().coeff(i) *= scalar; });
    return derived();
}

template <typename Derived, typename T>
Derived& VectorBase<Derived, T>::operator/=(const T& scalar)
{
    IL_CHECK(scalar != T{}, "Division by zero.");

    Parallel::isolatedParallelFor(0, size(), [&](const Parallel::Index& i) { derived().coeff(i) /= scalar; });
    return derived();
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::operator-() const
{
    Derived result = derived();
    result *= static_cast<T>(-1);
    return result;
}

template <typename Derived, typename T>
T VectorBase<Derived, T>::dot(const Derived& other) const
{
    IL_CHECK(size() == other.size(), "Vector sizes must match for dot product.");

    return Parallel::parallelSum<T>(0, size(),
                                    [&](const Parallel::Index& i) { return derived().coeff(i) * other.coeff(i); });
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::cross(const Derived& other) const
{
    IL_CHECK(size() == 3 && other.size() == 3, "Cross product is only defined for 3D vectors.");

    return Derived{derived().coeff(1) * other.coeff(2) - derived().coeff(2) * other.coeff(1),
                   derived().coeff(2) * other.coeff(0) - derived().coeff(0) * other.coeff(2),
                   derived().coeff(0) * other.coeff(1) - derived().coeff(1) * other.coeff(0)};
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::projectOnto(const Derived& other) const
{
    IL_CHECK(other.squaredNorm() != T{}, "Cannot project onto a zero vector.");

    return other * (dot(other) / other.squaredNorm());
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::projectOntoPlane(const Derived& planeNormal) const
{
    return derived() - projectOnto(planeNormal);
}

template <typename Derived, typename T>
double VectorBase<Derived, T>::distanceTo(const Derived& other) const
{
    return (derived() - other).norm();
}

template <typename Derived, typename T>
T VectorBase<Derived, T>::squaredDistanceTo(const Derived& other) const
{
    return (derived() - other).squaredNorm();
}

template <typename Derived, typename T>
std::size_t VectorBase<Derived, T>::size() const
{
    return derived().size();
}

template <typename Derived, typename T>
bool VectorBase<Derived, T>::empty() const
{
    return size() == 0;
}

template <typename Derived, typename T>
double VectorBase<Derived, T>::norm() const
{
    return std::sqrt(static_cast<double>(squaredNorm()));
}

template <typename Derived, typename T>
T VectorBase<Derived, T>::squaredNorm() const
{
    return Parallel::parallelSum<T>(0, size(),
                                    [&](const Parallel::Index& i)
                                    {
                                        const T value = derived().coeff(i);
                                        return value * value;
                                    });
}

template <typename Derived, typename T>
double VectorBase<Derived, T>::length() const
{
    return norm();
}

template <typename Derived, typename T>
double VectorBase<Derived, T>::lengthSquared() const
{
    return static_cast<double>(squaredNorm());
}

template <typename Derived, typename T>
T VectorBase<Derived, T>::max() const
{
    IL_CHECK(!empty(), "Cannot compute the maximum of an empty vector.");

    return Parallel::parallelMax<T>(0, size(), [&](const Parallel::Index& i) { return derived().coeff(i); });
}

template <typename Derived, typename T>
T VectorBase<Derived, T>::min() const
{
    IL_CHECK(!empty(), "Cannot compute the minimum of an empty vector.");

    return Parallel::parallelMin<T>(0, size(), [&](const Parallel::Index& i) { return derived().coeff(i); });
}

template <typename Derived, typename T>
void VectorBase<Derived, T>::normalize()
{
    static_assert(std::is_floating_point_v<T>, "normalize() is only supported for floating-point vector types.");
    const double n = norm();
    IL_CHECK(n != 0.0, "Cannot normalize a zero vector.");

    derived() /= static_cast<T>(n);
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::normalized() const
{
    Derived result = derived();
    result.normalize();
    return result;
}

template <typename Derived, typename T>
T VectorBase<Derived, T>::sum() const
{
    return Parallel::parallelSum<T>(0, size(), [&](const Parallel::Index& i) { return derived().coeff(i); });
}

template <typename Derived, typename T>
T VectorBase<Derived, T>::product() const
{
    return Parallel::parallelProduct<T>(0, size(), [&](const Parallel::Index& i) { return derived().coeff(i); });
}

template <typename Derived, typename T>
T VectorBase<Derived, T>::mean() const
{
    IL_CHECK(!empty(), "Cannot compute the mean of an empty vector.");

    return sum() / static_cast<T>(size());
}

template <typename Derived, typename T>
void VectorBase<Derived, T>::fill(const T& value)
{
    Parallel::isolatedParallelFor(0, size(), [&](const Parallel::Index& i) { derived().coeff(i) = value; });
}

template <typename Derived, typename T>
void VectorBase<Derived, T>::setToZero()
{
    fill(T{});
}

template <typename Derived, typename T>
typename VectorBase<Derived, T>::ElementWiseView VectorBase<Derived, T>::elementWise() const
{
    return ElementWiseView(derived());
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::product(const Derived& other) const
{
    IL_CHECK(m_v.size() == other.size(), "Vector sizes must match for elementwise product.");

    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i) { result.coeff(i) = m_v.coeff(i) * other.coeff(i); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::quotient(const Derived& other) const
{
    IL_CHECK(m_v.size() == other.size(), "Vector sizes must match for elementwise quotient.");

    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i)
                                  {
                                      IL_CHECK(other.coeff(i) != T{}, "Division by 0 in elementwise quotient.");
                                      result.coeff(i) = m_v.coeff(i) / other.coeff(i);
                                  });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::min(const Derived& other) const
{
    IL_CHECK(m_v.size() == other.size(), "Vector sizes must match for elementwise minimum.");

    Derived result = m_v;
    Parallel::isolatedParallelFor(
        0, m_v.size(), [&](const Parallel::Index& i) { result.coeff(i) = std::min(m_v.coeff(i), other.coeff(i)); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::max(const Derived& other) const
{
    IL_CHECK(m_v.size() == other.size(), "Vector sizes must match for elementwise maximum.");

    Derived result = m_v;
    Parallel::isolatedParallelFor(
        0, m_v.size(), [&](const Parallel::Index& i) { result.coeff(i) = std::max(m_v.coeff(i), other.coeff(i)); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::pow(const Derived& other) const
{
    IL_CHECK(m_v.size() == other.size(), "Vector sizes must match for elementwise power.");

    Derived result = m_v;
    Parallel::isolatedParallelFor(
        0, m_v.size(), [&](const Parallel::Index& i) { result.coeff(i) = std::pow(m_v.coeff(i), other.coeff(i)); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::abs() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i) { result.coeff(i) = std::abs(m_v.coeff(i)); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::sqrt() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i)
                                  {
                                      IL_CHECK(m_v.coeff(i) >= 0,
                                               "Attempted to take the square root of a negative number.");
                                      result.coeff(i) = std::sqrt(m_v.coeff(i));
                                  });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::square() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i) { result.coeff(i) = m_v.coeff(i) * m_v.coeff(i); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::cube() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i)
                                  {
                                      const T value = m_v.coeff(i);
                                      result.coeff(i) = value * value * value;
                                  });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::inverse() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i)
                                  {
                                      IL_CHECK(m_v.coeff(i) != T{}, "Division by zero in element-wise inverse.");
                                      result.coeff(i) = T{1} / m_v.coeff(i);
                                  });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::exp() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i) { result.coeff(i) = std::exp(m_v.coeff(i)); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::log() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i)
                                  {
                                      IL_CHECK(m_v.coeff(i) > 0, "Attempted to take the log of a number <= 0.");
                                      result.coeff(i) = std::log(m_v.coeff(i));
                                  });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::log10() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i)
                                  {
                                      IL_CHECK(m_v.coeff(i) > 0, "Attempted to take the log of a number <= 0.");
                                      result.coeff(i) = std::log10(m_v.coeff(i));
                                  });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::sin() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i) { result.coeff(i) = std::sin(m_v.coeff(i)); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::cos() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i) { result.coeff(i) = std::cos(m_v.coeff(i)); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::tan() const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i) { result.coeff(i) = std::tan(m_v.coeff(i)); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::pow(T exponent) const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(
        0, m_v.size(), [&](const Parallel::Index& i) { result.coeff(i) = std::pow(m_v.coeff(i), exponent); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::clamp(T minValue, T maxValue) const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(), [&](const Parallel::Index& i)
                                  { result.coeff(i) = std::clamp(m_v.coeff(i), minValue, maxValue); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::min(T value) const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i) { result.coeff(i) = std::min(m_v.coeff(i), value); });
    return result;
}

template <typename Derived, typename T>
Derived VectorBase<Derived, T>::ElementWiseView::max(T value) const
{
    Derived result = m_v;
    Parallel::isolatedParallelFor(0, m_v.size(),
                                  [&](const Parallel::Index& i) { result.coeff(i) = std::max(m_v.coeff(i), value); });
    return result;
}

template <typename Derived, typename T>
bool VectorBase<Derived, T>::isApprox(const Derived& other, double tolerance) const
{
    if (size() != other.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < size(); ++i)
    {
        if (std::abs(static_cast<double>(derived().coeff(i) - other.coeff(i))) > tolerance)
        {
            return false;
        }
    }

    return true;
}

template <typename Derived, typename T>
bool VectorBase<Derived, T>::allFinite() const
{
    for (std::size_t i = 0; i < size(); ++i)
    {
        if (!std::isfinite(static_cast<double>(derived().coeff(i))))
        {
            return false;
        }
    }

    return true;
}

template <typename Derived, typename T>
bool VectorBase<Derived, T>::hasNaN() const
{
    for (std::size_t i = 0; i < size(); ++i)
    {
        if (std::isnan(static_cast<double>(derived().coeff(i))))
        {
            return true;
        }
    }

    return false;
}
} // namespace InfinityLearn
