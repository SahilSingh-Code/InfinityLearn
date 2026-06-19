//-----------------------------------------------------------------------------------------------------------------------
// Matrix.h
//
// Core dense matrix utilities for InfinityLearn.
//-----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "Core/Parallelization.h" // Parallel::Range, Parallel::parallelFor, etc
#include <cstddef>                // std::size_t

namespace InfinityLearn
{

/// <summary>
/// Base class for all matrix types. Do not construct this matrix type directly, it does not have storage and only
/// contains common functionality for all matrix types. Use StaticMatrix or DynamicMatrix instead. This class uses CRTP
/// to allow for static and dynamic matrices to share common functionality while still allowing for the performance
/// benefits of static matrices when the size is known at compile time. The Derived template parameter is the type of
/// the derived class that inherits from this base class, and the T template parameter is the type of the elements in
/// the matrix (e.g. float, double, int, etc.).
/// </summary>
/// <typeparam name="Derived">What type of class is this?</typeparam>
/// <typeparam name="T">The type of the elements in the matrix.</typeparam>
template <typename Derived, typename T>
class MatrixBase
{
   public:
   protected:
    //------------------------------
    // Constructors And Destructors
    //------------------------------

    /// <summary>
    /// Default Constructor
    /// </summary>
    MatrixBase() = default;

    /// <summary>
    /// A default destructor that will clean up the matrix when it goes out of scope.
    /// </summary>
    ~MatrixBase() = default;

    /// <summary>
    /// Default copy constructor that will copy the values from another matrix into this matrix. The original matrix is
    /// not modified by this operation, and the returned matrix is a new matrix containing the copied values.
    /// </summary>
    /// <param name="other">The matrix to copy from</param>
    MatrixBase(const MatrixBase&) = default;

    /// <summary>
    /// Default move constructor that will move the values from another matrix into this matrix. The other matrix will
    /// be left in a valid but unspecified state.
    /// </summary>
    /// <param name="other">The matrix to move from</param>
    MatrixBase(MatrixBase&&) noexcept = default;


    /// <summary>
    /// Default copy assignment operator that will copy the values from another matrix into this matrix. The original
    /// matrix is not modified by this operation, and the returned matrix is a new matrix containing the copied values.
    /// </summary>
    /// <param name="other">The matrix to copy from</param>
    /// <returns>A reference to this matrix</returns>
    MatrixBase& operator=(const MatrixBase&) = default;


    /// <summary>
    /// Default move assignment operator that will move the values from another matrix into this matrix. The other
    /// matrix will be left in a valid but unspecified state.
    /// </summary>
    /// <param name="other">The matrix to move from</param>
    /// <returns>A reference to this matrix</returns>
    MatrixBase& operator=(MatrixBase&&) noexcept = default;


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
/// Multiply a matrix by a scalar value. This enables multiplication of a matrix by a scalar from the left side, e.g.
/// scalar * matrix. The original matrix is not modified, and the returned matrix is a new matrix containing the
/// multiplied values.
/// </summary>
/// <param name="scalar">The scalar value to multiply by</param>
/// <param name="v">The matrix to multiply</param>
/// <returns>The resulting matrix after multiplication</returns>
template <typename Derived, typename T>
Derived operator*(const T& scalar, const MatrixBase<Derived, T>& v)
{
    return static_cast<const Derived&>(v) * scalar;
}


/// <summary>
/// A dense matrix with a fixed size known at compile time.
/// </summary>
/// <typeparam name="T">What data type are you storing.</typeparam>
/// <typeparam name="NRows">The number of rows in the matrix.</typeparam>
/// <typeparam name="NCols">The number of columns in the matrix.</typeparam>
template <typename T, std::size_t NRows, std::size_t NCols>
class StaticMatrix : public MatrixBase<StaticMatrix<T, NRows, NCols>, T>
{
   public:
   private:
    T m_data[NRows][NCols] = {0};
};

/// <summary>
/// A dense matrix with a size determined at runtime. The number of rows and columns can be set at construction time,
/// and can be changed later if needed.
/// </summary>
/// <typeparam name="T">The type of the elements in the matrix.</typeparam>
template <typename T>
class DynamicMatrix : public MatrixBase<DynamicMatrix<T>, T>
{
   public:
    /// <summary>
    /// Defualt constructor will create an empty matrix of size 0x0.
    /// </summary>
    DynamicMatrix() = default;

    /// <summary>
    /// Constructor that creates a matrix with the given number of rows and columns. The elements are value-initialized.
    /// </summary>
    /// <param name="rows">The number of rows in the matrix.</param>
    /// <param name="cols">The number of columns in the matrix.</param>
    DynamicMatrix(std::size_t rows, std::size_t cols)
        : m_rows(rows), m_cols(cols), m_data(checkedElementCount(rows, cols))
    {
    }

    /// <summary>
    /// Constructor that creates a matrix with the given number of rows and columns, and initializes all
    /// elements to the given value.
    /// </summary>
    /// <param name="rows">The number of rows in the matrix.</param>
    /// <param name="cols">The number of columns in the matrix.</param>
    /// <param name="value">The value to initialize all elements with.</param>
    DynamicMatrix(std::size_t rows, std::size_t cols, const T& value)
        : m_rows(rows), m_cols(cols), m_data(checkedElementCount(rows, cols))
    {
        // The isolated parallel for is protected by the check that the number of elements is not too large, so we can
        // safely use it here without worrying about slowing the loop down by running parallel in a small number of
        // elements
        Parallel::isolatedParallelFor(0, m_data.size(),
                                      [&](const Parallel ::Index& i)
                                      {
                                          IL_ASSERT(i < m_data.size(),
                                                    "Indexed out of range when initializing the dynamic matrix.");
                                          m_data[i] = value;
                                      });
    }

    std::size_t rows() const noexcept
    {
        return m_rows;
    }

    std::size_t cols() const noexcept
    {
        return m_cols;
    }

    std::size_t size() const noexcept
    {
        return m_data.size();
    }

    bool empty() const noexcept
    {
        return m_data.empty();
    }

    T* data() noexcept
    {
        return m_data.data();
    }

    const T* data() const noexcept
    {
        return m_data.data();
    }

    T& operator()(std::size_t row, std::size_t col)
    {
        IL_ASSERT(row < m_rows, "Index out of bounds when retrieving entry in matrix.");
        IL_ASSERT(col < m_cols, "Index out of bounds when retrieving entry in matrix.");
        return m_data[row * m_cols + col];
    }

    const T& operator()(std::size_t row, std::size_t col) const
    {
        IL_ASSERT(row < m_rows, "Index out of bounds when retrieving entry in matrix.");
        IL_ASSERT(col < m_cols, "Index out of bounds when retrieving entry in matrix.");
        return m_data[row * m_cols + col];
    }

    void resize(std::size_t rows, std::size_t cols)
    {
        m_rows = rows;
        m_cols = cols;
        m_data.resize(checkedElementCount(rows, cols));
    }

    void resize(std::size_t rows, std::size_t cols, const T& value)
    {
        m_rows = rows;
        m_cols = cols;
        m_data.assign(checkedElementCount(rows, cols), value);
    }

   private:
    static std::size_t checkedElementCount(std::size_t rows, std::size_t cols)
    {
        if (cols != 0 && rows > std::numeric_limits<std::size_t>::max() / cols)
        {
            throw std::length_error("DynamicMatrix size overflow.");
        }

        return rows * cols;
    }

    std::size_t m_rows = 0;
    std::size_t m_cols = 0;
    std::vector<T> m_data;
};

} // namespace InfinityLearn
