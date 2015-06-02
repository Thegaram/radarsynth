#ifndef MATRIXVALUESTORE_H_INCLUDED
#define MATRIXVALUESTORE_H_INCLUDED

#include <vector>    // vector
#include <cassert>   // assert
#include <algorithm> // swap, min


/**
* A simple matrix class for storing elements in a rectangular grid.
* Stores elements of type T in a resizable matrix.
* (Note: might not work with bool type.)
**/
template <typename T>
class MatrixValueStore
{
public:
	using size_type = typename std::vector<T>::size_type;

public:
	/**
	*   \brief Constructs a MatrixValueStore with default initialized elements.
	*
	*   \param width Width of matrix.
	*   \param height Height of matrix.
	**/
	MatrixValueStore(size_type width = 0, size_type height = 0)
		: m_width(width), m_height(height), m_values(std::vector<T>(width * height))
	{
		// check size
		assert(m_values.size() == (m_width * m_height));
	}

	/**
	*   \brief Returns the element at the given coordinates.
	*
	*   \param x x coordinate of element.
	*   \param y y coordinate of element.
	*   \return Reference to the element at (x, y).
	**/
	const T& operator()(size_type x, size_type y) const
	{
		assert(x < m_width); assert(y < m_height);

		size_type ID = getLinearID(x, y, m_width);
		return m_values[ID];
	}

	/**
	*   \brief Returns the element at the given coordinates.
	*
	*   \param x x coordinate of element.
	*   \param y y coordinate of element.
	*   \return Reference to the element at (x, y).
	**/
	T& operator()(size_type x, size_type y)
	{
		// reuse const version
		return const_cast<T&>(static_cast<const MatrixValueStore<T>&>(*this)(x, y));
	}

	/**
	*   \brief Returns the width of the matrix.
	*
	*   \return The width of the matrix.
	**/
	size_type width() const { return m_width; }

	/**
	*   \brief Returns the height of the matrix.
	*
	*   \return The height of the matrix.
	**/
	size_type height() const { return m_height; }

	/**
	*   \brief Resized the matrix.
	*
	*   Changes the size of the matrix to the given dimensions, copying the elements from the "intersection of
	*   the old and new matrix and default initializing the rest. (Note: can be slow with big matrices due to copying.)
	*
	*   \param newWidth New width of matrix.
	*   \param newHeight New height of matrix.
	**/
	void resize(size_type newWidth, size_type newHeight)
	{
		// initialize new array
		std::vector<T> newValues(newWidth * newHeight);

		// copy old elements
		size_type copyWidth = std::min(m_width, newWidth);
		size_type copyHeight = std::min(m_height, newHeight);

		for (size_type y = 0; y < copyHeight; ++y)
			for (size_type x = 0; x < copyWidth; ++x)
			{
				size_type old_ID = getLinearID(x, y, m_width);
				size_type new_ID = getLinearID(x, y, newWidth);
				newValues[new_ID] = m_values[old_ID];
			}

		swap(m_values, newValues);
		m_width = newWidth; m_height = newHeight;

		assert(m_values.size() == (m_width * m_height));
	}

public:
	/*void print() const
	{
		for (size_type y = 0; y < m_height; ++y)
		{
			for (size_type x = 0; x < m_width; ++x)
			{
				size_type ID = getLinearID(x, y, m_width);

				cout << (m_values[ID] > T() ? '|' : '-');
			}
			cout << endl;
		}
	}*/

private:
	MatrixValueStore(const MatrixValueStore&) = delete;
	MatrixValueStore& operator=(const MatrixValueStore&) = delete;

	/**
	*   \brief Converts the coordinates (x, y) to linear coordinates using the given width value.
	**/
	static size_type getLinearID(size_type x, size_type y, size_type width)
	{
		return y * width + x;
	}

private:
	size_type m_width, m_height; /*!< Matrix dimensions. */
	std::vector<T> m_values;     /*!< Vector storing the actual values. */
};

#endif  // MATRIXVALUESTORE_H_INCLUDED
