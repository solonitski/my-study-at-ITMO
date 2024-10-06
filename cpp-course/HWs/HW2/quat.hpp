#pragma once

#include <cmath>

template< typename T >
struct matrix_t
{
    T data[16];
};

template< typename T >
struct vector3_t
{
    T x, y, z;
};

template< typename T >
class Quat
{
public:
    // Constructors
    Quat() : m_value{ 0, 0, 0, 0 } {}

    Quat(T a, T b, T c, T d) : m_value{ b, c, d, a } {}

    Quat(T angle, bool isRadians, const vector3_t< T > &axis)
    {
        angle *= isRadians ? 1 : static_cast< T >(M_PI) / static_cast< T >(180);

        T norm = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
        T sinusDivNorm = std::sin(angle / 2) / (norm != 0 ? norm : 1);

        m_value[0] = axis.x * sinusDivNorm;
        m_value[1] = axis.y * sinusDivNorm;
        m_value[2] = axis.z * sinusDivNorm;
        m_value[3] = std::cos(angle / 2);
    }

    // Operators
    Quat operator+(const Quat &rhs) const
    {
        // Questions about your .clang-format, everything was fine without it.
        return Quat(m_value[3] + rhs.m_value[3], m_value[0] + rhs.m_value[0], m_value[1] + rhs.m_value[1], m_value[2] + rhs.m_value[2]);
    }

    Quat &operator+=(const Quat &rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    Quat operator-() const { return { -m_value[3], -m_value[0], -m_value[1], -m_value[2] }; }

    Quat operator-(const Quat &rhs) const { return *this + -rhs; }

    Quat &operator-=(const Quat &rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    Quat operator*(const Quat &rhs) const
    {
        T a1 = m_value[3], b1 = m_value[0], c1 = m_value[1], d1 = m_value[2];
        T a2 = rhs.m_value[3], b2 = rhs.m_value[0], c2 = rhs.m_value[1], d2 = rhs.m_value[2];

        return Quat(
            a1 * a2 - b1 * b2 - c1 * c2 - d1 * d2,
            a1 * b2 + b1 * a2 + c1 * d2 - d1 * c2,
            a1 * c2 - b1 * d2 + c1 * a2 + d1 * b2,
            a1 * d2 + b1 * c2 - c1 * b2 + d1 * a2);
    }

    Quat operator*(const T scalar) const
    {
        return Quat(m_value[3] * scalar, m_value[0] * scalar, m_value[1] * scalar, m_value[2] * scalar);
    }

    Quat operator*(const vector3_t< T > &vec) const
    {
        Quat< T > vec_quat(0, vec.x, vec.y, vec.z);
        return *this * vec_quat;
    }

    Quat operator~() const { return Quat(m_value[3], -m_value[0], -m_value[1], -m_value[2]); }

    bool operator==(const Quat &rhs) const
    {
        // Questions about your .clang-format, everything was fine without it.
        return m_value[0] == rhs.m_value[0] && m_value[1] == rhs.m_value[1] && m_value[2] == rhs.m_value[2] &&
        m_value[3] == rhs.m_value[3];
    }

    bool operator!=(const Quat &rhs) const { return !(*this == rhs); }

    explicit operator T() const
    {
        // Questions about your .clang-format, everything was fine without it.
        return std::sqrt(m_value[0] * m_value[0] + m_value[1] * m_value[1] + m_value[2] * m_value[2] + m_value[3] * m_value[3]);
    }

    // Methods
    void normalize()
    {
        T norm = static_cast< T >(*this);
        if (norm != 0)
        {
            m_value[0] /= norm;
            m_value[1] /= norm;
            m_value[2] /= norm;
            m_value[3] /= norm;
        }
    }

    const T *data() const { return m_value; }

    matrix_t< T > rotation_matrix() const
    {
        T modulus = static_cast< T >(*this);
        T s = 1 / (modulus * modulus);

        T ii = m_value[0] * m_value[0];
        T jj = m_value[1] * m_value[1];
        T kk = m_value[2] * m_value[2];
        T ij = m_value[0] * m_value[1];
        T kr = m_value[2] * m_value[3];
        T ik = m_value[0] * m_value[2];
        T jr = m_value[1] * m_value[3];
        T jk = m_value[1] * m_value[2];
        T ir = m_value[0] * m_value[3];

        // Questions about your .clang-format, everything was fine without it.
        return {
            { 1 - 2 * s * (jj + kk),
                2 * s * (ij + kr),
                2 * s * (ik - jr),
                0,
                2 * s * (ij - kr),
                1 - 2 * s * (ii + kk),
                2 * s * (jk + ir),
                0,
                2 * s * (ik + jr),
                2 * s * (jk - ir),
                1 - 2 * s * (ii + jj),
                0,
                0,
                0,
                0,
                1 }
        };
    }

    matrix_t< T > matrix() const
    {
        // Questions about your .clang-format, everything was fine without it.
        return {
            { m_value[3],
                -m_value[0],
                -m_value[1],
                -m_value[2],
                m_value[0],
                m_value[3],
                -m_value[2],
                m_value[1],
                m_value[1],
                m_value[2],
                m_value[3],
                -m_value[0],
                m_value[2],
                -m_value[1],
                m_value[0],
                m_value[3] }
        };
    }

    T angle(bool isRadians = true) const
    {
        T angle = 2 * std::acos(m_value[3]);
        angle *= isRadians ? 1 : static_cast< T >(180) / static_cast< T >(M_PI);

        return angle;
    }

    vector3_t< T > apply(const vector3_t< T > &vec) const
    {
        Quat< T > normalized_quat = *this;
        normalized_quat.normalize();
        Quat< T > result_quat = normalized_quat * vec * ~normalized_quat;

        return { result_quat.m_value[0], result_quat.m_value[1], result_quat.m_value[2] };
    }

private:
    T m_value[4];
};
