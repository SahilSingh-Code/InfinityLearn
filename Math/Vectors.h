//-----------------------------------------------------------------------------------------------------------------------
// Vectors.h
//
// Core vector utilities for InfinityLearn.
//-----------------------------------------------------------------------------------------------------------------------

#include <cstddef>          // std::size_t
#include <initializer_list> // std::initializer_list
#include <vector>           // std::vector

namespace InfinityLearn
{

// A basic vector class that is templated on the type of the elements and the number of elements. This class owns an
// array of N elements, and provides all the utilities needed for a vector
template <typename T, std::size_t N>
class Vector
{
   public:
    //------------------------------
    // Constructors And Destructors
    //------------------------------

    /// <summary>
    /// Blank constructor, will construct a zero-initialized vector of requested size
    /// </summary>
    Vector();

    /// <summary>
    /// Constructor that takes an initializer list of values to initialize the vector with. The number of values in the
    /// initializer list must match the size of the vector.
    /// </summary>
    /// <param name="values">The values to initialize the vector with.</param>
    Vector(std::initializer_list<T> values);

    /// <summary>
    /// Copy constructor that creates a new vector by copying the values from another vector.
    /// </summary>
    /// <param name="other"></param>
    Vector(const Vector& other);

    /// <summary>
    /// Move constructor that creates a new vector by moving the values from another vector. The other vector will be
    /// left in a valid but unspecified state
    /// </summary>
    /// <param name="other">The other vector to be moved</param>
    Vector(Vector&& other) noexcept;

    /// <summary>
    /// A default destructor that will clean up the vector when it goes out of scope. Since this class only contains a
    /// fixed-size array, the default destructor is sufficient for cleaning up the vector.
    /// </summary>
    ~Vector() = default;


    //------------------
    // Basic Operators
    //------------------

    /// <summary>
    /// Copy assignment operator that copies the values from another vector into this vector.
    /// </summary>
    /// <param name="other">The other vector to be copied from</param>
    /// <returns>The copied vector</returns>
    Vector& operator=(const Vector& other);

    /// <summary>
    /// Move assignment operator that moves the values from another vector into this vector. The other vector will be
    /// left in a valid but unspecified state.
    /// </summary>
    /// <param name="other">The other vector to be moved from</param>
    /// <returns>The moved vector</returns>
    Vector& operator=(Vector&& other) noexcept;

    /// <summary>
    /// Equality operator that checks if two vectors are equal.
    /// </summary>
    /// <param name="other">The other vector to compare against</param>
    /// <returns>True if the vectors are equal, false otherwise</returns>
    bool operator==(const Vector& other) const;


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
    Vector operator+(const Vector& other) const;

    /// <summary>
    /// Add two vectors together element wise and store the result in this vector. The other vector is not modified.
    /// </summary>
    /// <param name="other">The other vector to be accumulated into this one.</param>
    /// <returns>A reference to this vector after the operation</returns>
    Vector& operator+=(const Vector& other);

    /// <summary>
    /// Subtract two vectors element wise and return the result as a new vector. The original vectors are not modified.
    /// </summary>
    /// <param name="other">The other vector to be subtracted.</param>
    /// <returns>The resulting vector after subtraction</returns>
    Vector operator-(const Vector& other) const;

    /// <summary>
    /// Subtract two vectors element wise and store the result in this vector. The other vector is not modified.
    /// </summary>
    /// <param name="other">The other vector to be subtracted.</param>
    /// <returns>A reference to this vector after the operation</returns>
    Vector& operator-=(const Vector& other);

    /// <summary>
    /// Multiply a vector by a scalar value and return the result as a new vector. The original vector is not modified.
    /// </summary>
    /// <param name="scalar">The scalar value to multiply by.</param>
    /// <returns>The resulting vector after multiplication</returns>
    Vector operator*(const T& scalar) const;

    /// <summary>
    /// Divide a vector by a scalar value and return the result as a new vector. The original vector is not modified.
    /// </summary>
    /// <param name="scalar">The scalar value to divide by.</param>
    /// <returns>The resulting vector after division</returns>
    Vector operator/(const T& scalar) const;

    /// <summary>
    /// Scale a vector element wise by a scalar value.
    /// </summary>
    /// <param name="scalar">The scalar value to be multiplied by</param>
    /// <returns>A reference to this vector after the operation</returns>
    Vector& operator*=(const T& scalar);

    /// <summary>
    /// Scale a vector element wise by the reciprocal of a scalar value.
    /// </summary>
    /// <param name="scalar">The scalar value to be divided by</param>
    /// <returns>A reference to this vector after the operation</returns>
    Vector& operator/=(const T& scalar);

    /// <summary>
    /// Negate a vector element wise and return the result as a new vector. The original vector is not modified.
    /// </summary>
    /// <returns></returns>
    Vector operator-() const;

    /// <summary>
    /// Multiply a vector by a scalar value.
    /// </summary>
    /// <param name="scalar">The scalar value to multiply by</param>
    /// <param name="v">The vector to multiply</param>
    /// <returns>The resulting vector after multiplication</returns>
    friend Vector operator*(const T& scalar, const Vector& v);


    //-------------------------
    // Complex Math Operations
    //-------------------------

    /// <summary>
    /// Compute the dot product of this vector with another vector and return the result as a scalar value. The original
    /// vectors are not modified.
    /// </summary>
    /// <param name="other">The other vector to compute the dot product with</param>
    /// <returns>The dot product of the two vectors</returns>
    T dot(const Vector& other) const;

    /// <summary>
    /// Compute the cross product of this vector with another vector and return the result as a new vector. The original
    /// vectors are not modified. Is only valid for 3D vectors, and will fail to compile otherwise.
    /// </summary>
    /// <param name="other">The other vector to compute the cross product with</param>
    /// <returns></returns>
    Vector cross(const Vector& other) const;

    /// <summary>
    /// Project this vector onto another vector and return the result as a new vector. The original vectors are not
    /// modified.
    /// </summary>
    /// <param name="other">The other vector to project onto</param>
    /// <returns>The projection of this vector onto the other vector</returns>
    Vector projectOnto(const Vector& other) const;

    /// <summary>
    /// Project this vector onto a plane defined by a normal vector and return the result as a new vector. The original
    /// vectors are not modified.
    /// </summary>
    /// <param name="planeNormal">The normal vector of the plane to project onto</param>
    /// <returns>The projection of this vector onto the plane</returns>
    Vector projectOntoPlane(const Vector& planeNormal) const;

    /// <summary>
    /// Find the distance between this vector and another vector and return it as a scalar value. The original vectors
    /// are not modified by this operation.
    /// </summary>
    /// <param name="other">The vector to which the distance is calculated</param>
    /// <returns>The distance between the two vectors</returns>
    double distanceTo(const Vector& other) const;

    /// <summary>
    /// Find the squared distance between this vector and another vector and return it as a scalar value. The original
    /// vectors are not modified by this operation.
    /// </summary>
    /// <param name="other">The vector to which the squared distance is calculated</param>
    /// <returns>The squared distance between the two vectors</returns>
    T squaredDistanceTo(const Vector& other) const;


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
    Vector normalized() const;

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

    /// <summary>
    /// Return a vector of the same size as this one, but with all entries set to zero. The original vector is not
    /// modified, and the returned vector is a new vector with all entries set to zero.
    /// </summary>
    /// <returns>A vector of all zeroes that is the same size as this one</returns>
    static Vector zero();

    /// <summary>
    /// Return a vector of the same size as this one, but with all entries set to one. The original vector is not
    /// modified, and the returned vector is a new vector with all entries set to one.
    /// </summary>
    /// <returns>A vector of all ones that is the same size as this one</returns>
    static Vector ones();


    //----------------------
    // Slicing
    //----------------------

    /// <summary>
    /// Create a slice of the vector from the specified start index to the specified end index. The original vector is
    /// not modified, and the returned vector is a new vector containing the specified slice of the original vector. The
    /// start index is inclusive, and the end index is exclusive. The size of the returned vector will be equal to end -
    /// start.
    /// </summary>
    /// <typeparam name="Start">The starting index</typeparam>
    /// <typeparam name="End">The ending index</typeparam>
    /// <returns>A new vector containing the specified slice of the original vector</returns>
    template <std::size_t Start, std::size_t End>
    Vector<T, End - Start> slice() const;


    //------------------------
    // Elementwise Operations
    //------------------------

    /// <summary>
    /// A view into the elements of a vector for elementwise operations.
    /// </summary>
    class ElementWiseView
    {
       public:
        explicit ElementWiseView(const Vector& v) : m_v(v) {};

        /// <summary>
        /// Compute the elementwise product of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise product of the two vectors</returns>
        Vector product(const Vector& other) const;

        /// <summary>
        /// Compute the elementwise quotient of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise quotient of the two vectors</returns>
        Vector quotient(const Vector& other) const;

        /// <summary>
        /// Compute the elementwise minimum of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise minimum of the two vectors</returns>
        Vector min(const Vector& other) const;

        /// <summary>
        /// Compute the elementwise maximum of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise maximum of the two vectors</returns>
        Vector max(const Vector& other) const;

        /// <summary>
        /// Compute the elementwise power of this vector with another vector.
        /// </summary>
        /// <param name="other">The other vector</param>
        /// <returns>The elementwise power of the two vectors</returns>
        Vector pow(const Vector& other) const;

        /// <summary>
        /// Compute the elementwise absolute value of this vector.
        /// </summary>
        /// <returns>The elementwise absolute value of the vector</returns>
        Vector abs() const;

        /// <summary>
        /// Compute the elementwise square root of this vector.
        /// </summary>
        /// <returns>The elementwise square root of the vector</returns>
        Vector sqrt() const;

        /// <summary>
        /// Compute the elementwise square of this vector.
        /// </summary>
        /// <returns>The elementwise square of the vector</returns>
        Vector square() const;

        /// <summary>
        /// Compute the elementwise cube of this vector.
        /// </summary>
        /// <returns>The elementwise cube of the vector</returns>
        Vector cube() const;

        /// <summary>
        /// Compute the elementwise reciprocal of this vector.
        /// </summary>
        /// <returns>The elementwise reciprocal of the vector</returns>
        Vector inverse() const;

        /// <summary>
        /// Compute the elementwise exponential of this vector.
        /// </summary>
        /// <returns>The elementwise exponential of the vector</returns>
        Vector exp() const;

        /// <summary>
        /// Compute the elementwise natural logarithm of this vector.
        /// </summary>
        /// <returns>The elementwise natural logarithm of the vector</returns>
        Vector log() const;

        /// <summary>
        /// Compute the elementwise base-10 logarithm of this vector.
        /// </summary>
        /// <returns>The elementwise base-10 logarithm of the vector</returns>
        Vector log10() const;

        /// <summary>
        /// Compute the elementwise sine of this vector. The sine function is applied to each element of the vector
        /// independently, and the result is a new vector containing the sine of each element. The original vector is
        /// not modified by this operation.
        /// </summary>
        /// <returns>The elementwise sine of the vector</returns>
        Vector sin() const;

        /// <summary>
        /// Compute the elementwise cosine of this vector. The cosine function is applied to each element of the vector
        /// independently, and the result is a new vector containing the cosine of each element. The original vector is
        /// not modified by this operation.
        /// </summary>
        /// <returns>The elementwise cosine of the vector</returns>
        Vector cos() const;

        /// <summary>
        /// Compute the elementwise tangent of this vector. The tangent function is applied to each element of the
        /// vector independently, and the result is a new vector containing the tangent of each element. The original
        /// vector is not modified by this operation.
        /// </summary>
        /// <returns>The elementwise tangent of the vector</returns>
        Vector tan() const;

        /// <summary>
        /// Compute the elementwise power of this vector with a scalar exponent.
        /// </summary>
        /// <param name="exponent">The exponent to raise each element to</param>
        /// <returns>The elementwise power of the vector</returns>
        Vector pow(T exponent) const;

        /// <summary>
        /// Clamp each element of this vector to be within the specified minimum and maximum values. The original vector
        /// is not modified by this operation, and the returned vector is a new vector containing the clamped values.
        /// </summary>
        /// <param name="minValue">The minimum value to clamp to</param>
        /// <param name="maxValue">The maximum value to clamp to</param>
        /// <returns>The clamped vector</returns>
        Vector clamp(T minValue, T maxValue) const;

        /// <summary>
        /// Compute the elementwise minimum of this vector with a scalar value, returning a new vector where each
        /// element is the minimum of the corresponding element in this vector and the scalar value. The original vector
        /// is not modified by this operation.
        /// </summary>
        /// <param name="value">The scalar value to compare against</param>
        /// <returns>The elementwise minimum of the vector</returns>
        Vector min(T value) const;

        /// <summary>
        /// Compute the elementwise maximum of this vector with a scalar value, returning a new vector where each
        /// element is the maximum of the corresponding element in this vector and the scalar value. The original vector
        /// is not modified by this operation.
        /// </summary>
        /// <param name="value">The scalar value to compare against</param>
        /// <returns>The elementwise maximum of the vector</returns>
        Vector max(T value) const;

       private:
        const Vector& m_v;
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
    bool isApprox(const Vector& other, double tolerance = 1e-9) const;

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


   private:
    T m_data[N];
};

//---------------------------------
// Aliases for common vector types
//---------------------------------

using Vector2f = Vector<float, 2>;
using Vector3f = Vector<float, 3>;
using Vector4f = Vector<float, 4>;
using Vector2d = Vector<double, 2>;
using Vector3d = Vector<double, 3>;
using Vector4d = Vector<double, 4>;
using Vector2i = Vector<int, 2>;
using Vector3i = Vector<int, 3>;
using Vector4i = Vector<int, 4>;
} // namespace InfinityLearn
