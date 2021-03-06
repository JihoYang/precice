#pragma once

#include "FindClosestVertex.hpp"
#include "FindClosestEdge.hpp"
#include "FindClosestTriangle.hpp"
#include "FindClosestQuad.hpp"

namespace precice {
   namespace mesh {
      class Mesh;
   }
}

// ----------------------------------------------------------- CLASS DEFINITION

namespace precice {
namespace query {

/**
 * @brief Weighting and reference to target element for a value to interpolate
 */
struct InterpolationElement
{
  mesh::Vertex* element;
  double weight;

  InterpolationElement ()
  : element(NULL), weight(0.0) {}
};

/**
 * @brief Closest element to all objects with given mesh ID
 */
struct ClosestElement
{
  std::vector<int> meshIDs;
  double distance;
  Eigen::VectorXd vectorToElement;
  std::vector<InterpolationElement> interpolationElements;

  ClosestElement (int dim)
  : meshIDs(),
    distance(0.0),
    vectorToElement(Eigen::VectorXd::Zero(dim)),
    interpolationElements() {}
};


/**
 * @brief Determines closest Triangle, Edge, or Vertex object to a given point.
 *
 * Computes a distance vector to every Triangle and Vertex object found and
 * stores the object with shortest distance.
 *
 * The distance to a Vertex object is measured directly from the search point
 * to the Vertex's coordinates. The interpolation weight to a vertex is one.
 *
 * The distance to a Triangle is measured from the search point, to a point
 * orthogonally projected onto the triangles plane (or line in 2D). The
 * distance is valid only then, when the projected point lies within the
 * triangle, i.e. when the barycentric coordinates of the projected point are
 * all smaller or equal to one. The interpolation weigths for the points of the
 * triangle are equal to the barycentric coordinates.
 */
class FindClosest
{
public:

  /**
   * @brief Constructor, searchpoint can be specified only there
   *
   * @param[in] searchpoint Point from where distances to objects are measured
   */
  template<typename VECTOR_T>
  FindClosest ( const VECTOR_T& searchpoint );

  /// Finds closest distance to all mesh elements in the given container.
  template<typename CONTAINER_T>
  bool operator() ( CONTAINER_T& container );

  /// Returns true, if a closest element was found.
  bool hasFound() const;

  /// Returns ClosestElement found, error when no visitable has been found
  const ClosestElement& getClosest();

  /// Returns the euclidian distance to the closest element.
  double getEuclidianDistance();

  /// Returns search point
  const Eigen::VectorXd& getSearchPoint() const;

  /// Resets the found visitables, not done automatically
  void reset();

private:

  logging::Logger _log{"query::FindClosest"};

  /// Finds closest distance to Vertex objects.
  FindClosestVertex _findClosestVertex;

  /// Finds closest distance to Edge objects.
  FindClosestEdge _findClosestEdge;

  /// Find closest distance to Triangle objects.
  FindClosestTriangle _findClosestTriangle;

  /// Find closest distance to Quad objects.
  FindClosestQuad _findClosestQuad;

  /// Closest mesh element.
  ClosestElement _closest;

  /// Search point, from where distances to objects are measured
  const Eigen::VectorXd _searchpoint;

  /**
   * @brief Determines the closest element from all FindXY member objects.
   *
   * @return True, if a closest object has been found.
   */
  bool determineClosest();
};

// --------------------------------------------------------- HEADER DEFINITIONS

template<typename VECTOR_T>
FindClosest:: FindClosest
(
  const VECTOR_T& searchpoint )
:
  _findClosestVertex(searchpoint),
  _findClosestEdge(searchpoint),
  _findClosestTriangle(searchpoint),
  _findClosestQuad(searchpoint),
  _closest(searchpoint.size()),
  _searchpoint(searchpoint)
{}

template<typename CONTAINER_T>
bool FindClosest:: operator()
(
  CONTAINER_T& container )
{
  // It is not valid here, to stop the search for vertices (e.g.) if a closest
  // edge has been found already, since the edge might be oriented wrongly.
  _findClosestTriangle(container);
  _findClosestEdge(container);
  _findClosestVertex(container);
  _findClosestQuad(container);
  return determineClosest();
}

}} // namespace precice, query
