/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2024, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */
#pragma once

#include <mrpt/core/bits_math.h>  // hypot_fast()
#include <mrpt/math/TPoint2D.h>
#include <mrpt/math/TPoseOrPoint.h>
#include <mrpt/math/wrap2pi.h>

#include <vector>

namespace mrpt::math
{
/**
 * Lightweight 2D pose. Allows coordinate access using [] operator.
 * \sa mrpt::poses::CPose2D
 * \ingroup geometry_grp
 */
struct TPose2D : public TPoseOrPoint, public internal::ProvideStaticResize<TPose2D>
{
  enum
  {
    static_size = 3
  };
  /** X,Y coordinates */
  double x{.0}, y{.0};
  /** Orientation (rads) */
  double phi{.0};

  /** Returns the identity transformation */
  static constexpr TPose2D Identity() { return TPose2D(); }

  /** Explicit constructor from TPoint2D. Zeroes the phi coordinate.
   * \sa TPoint2D
   */
  explicit TPose2D(const TPoint2D& p);

  /**
   * Constructor from TPoint3D, losing information. Zeroes the phi
   * coordinate.
   * \sa TPoint3D
   */
  explicit TPose2D(const TPoint3D& p);
  /**
   * Constructor from TPose3D, losing information. The phi corresponds to the
   * original pose's yaw.
   * \sa TPose3D
   */
  explicit TPose2D(const TPose3D& p);
  /**
   * Constructor from coordinates.
   */
  constexpr TPose2D(double xx, double yy, double Phi) : x(xx), y(yy), phi(Phi) {}
  /**
   * Default fast constructor. Initializes to zeros.
   */
  constexpr TPose2D() = default;

  /** Builds from the first 3 elements of a vector-like object: [x y phi]
   *
   * \tparam Vector It can be std::vector<double>, Eigen::VectorXd, etc.
   */
  template <typename Vector>
  static TPose2D FromVector(const Vector& v)
  {
    TPose2D o;
    for (int i = 0; i < 3; i++) o[i] = v[i];
    return o;
  }
  /** Coordinate access using operator[]. Order: x,y,phi */
  double& operator[](size_t i)
  {
    switch (i)
    {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return phi;
      default:
        throw std::out_of_range("index out of range");
    }
  }
  /** Coordinate access using operator[]. Order: x,y,phi */
  constexpr double operator[](size_t i) const
  {
    switch (i)
    {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return phi;
      default:
        throw std::out_of_range("index out of range");
    }
  }
  /** Gets the pose as a vector of doubles.
   * \tparam Vector It can be std::vector<double>, Eigen::VectorXd, etc.
   */
  template <typename Vector>
  void asVector(Vector& v) const
  {
    v.resize(3);
    v[0] = x;
    v[1] = y;
    v[2] = phi;
  }
  /// \overload
  template <typename Vector>
  Vector asVector() const
  {
    Vector v;
    asVector(v);
    return v;
  }

  /** Returns a human-readable textual representation of the object (eg: "[x y
   * yaw]", yaw in degrees)
   * \sa fromString
   */
  void asString(std::string& s) const;
  std::string asString() const
  {
    std::string s;
    asString(s);
    return s;
  }

  /** Operator "oplus" pose composition: "ret=this \oplus b"  \sa CPose2D */
  mrpt::math::TPose2D operator+(const mrpt::math::TPose2D& b) const;
  /** Operator "ominus" pose composition: "ret=this \ominus b"  \sa CPose2D */
  mrpt::math::TPose2D operator-(const mrpt::math::TPose2D& b) const;

  mrpt::math::TPoint2D composePoint(const TPoint2D l) const;

  mrpt::math::TPoint2D operator+(const mrpt::math::TPoint2D& b) const;

  mrpt::math::TPoint2D inverseComposePoint(const TPoint2D g) const;

  /** Returns the (x,y) translational part of the SE(2) transformation. */
  mrpt::math::TPoint2D translation() const { return {x, y}; }

  /** Returns the norm of the (x,y) vector (phi is not used) */
  double norm() const { return mrpt::hypot_fast(x, y); }
  /** Forces "phi" to be in the range [-pi,pi] */
  void normalizePhi() { phi = mrpt::math::wrapToPi(phi); }
  /** Set the current object value from a string generated by 'asString' (eg:
   * "[0.02 1.04 -45.0]" )
   * \sa asString
   * \exception std::exception On invalid format
   */
  void fromString(const std::string& s);
  static TPose2D FromString(const std::string& s)
  {
    TPose2D o;
    o.fromString(s);
    return o;
  }
};

/** Exact comparison between 2D poses, taking possible cycles into account */
inline bool operator==(const TPose2D& p1, const TPose2D& p2)
{
  return (p1.x == p2.x) && (p1.y == p2.y) &&
         (mrpt::math::wrapTo2Pi(p1.phi) == mrpt::math::wrapTo2Pi(p2.phi));  //-V550
}
/** Exact comparison between 2D poses, taking possible cycles into account */
inline bool operator!=(const TPose2D& p1, const TPose2D& p2)
{
  return (p1.x != p2.x) || (p1.y != p2.y) ||
         (mrpt::math::wrapTo2Pi(p1.phi) != mrpt::math::wrapTo2Pi(p2.phi));  //-V550
}

}  // namespace mrpt::math

namespace mrpt::typemeta
{
// Specialization must occur in the same namespace
MRPT_DECLARE_TTYPENAME_NO_NAMESPACE(TPose2D, mrpt::math)
}  // namespace mrpt::typemeta
