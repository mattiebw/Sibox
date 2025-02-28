#pragma once

template<typename T>
struct Matrix2x2
{
    // Create an identity matrix.
    Matrix2x2()
        : m_Data({{1, 0}, {0, 1}})
    { }
    
    void Set(int row, int column, T value)
    {
        m_Data[row][column] = value;
    }
    
private:
    T m_Data[2][2];
};
