#pragma once
#include "Partition.hpp"
#include "logging/Logger.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/Vertex.hpp"



namespace PartitionTests {
namespace ProvidedBoundingBoxTests {
struct TestProvidedBoundingBox;
}
}

namespace PartitionTests {
namespace ReceivedBoundingBoxTests {
struct TestReceivedBoundingBox2D;
struct TestReceivedBoundingBox3D;
}
}




namespace precice {
namespace partition {


/**
 * @brief this class is supposed to:
 * 1- creat bounding boxes around each ranks mesh partition
 * 2- gather these bounding boxes in the master
 * 3- send them to the other master
 */
class ProvidedBoundingBox :  public Partition 
{
public:

   /// Constructor
  ProvidedBoundingBox(mesh::PtrMesh mesh, bool hasToSend, double safetyFactor, mesh::Mesh::BoundingBoxMap globalBB);

  virtual ~ProvidedBoundingBox() {}

   /// The boundingbox is gathered and sent to another participant (if required)
  virtual void communicate();
  virtual void compute();  
  friend struct PartitionTests::ProvidedBoundingBoxTests::TestProvidedBoundingBox;
  friend struct PartitionTests::ReceivedBoundingBoxTests::TestReceivedBoundingBox2D;
  friend struct PartitionTests::ReceivedBoundingBoxTests::TestReceivedBoundingBox3D;
  

private:

  virtual void createOwnerInformation();
  static logging::Logger _log;
  bool _hasToSend;
  mesh::Mesh::BoundingBox _bb;
  mesh::Mesh::FeedbackMap  received_feedbackMap;
  mesh::Mesh::BoundingBoxMap _globalBB;
  int _dimensions;
  std::vector<int> connected_ranks;
  double _safetyFactor;
  
};

}} // namespace precice, partition
