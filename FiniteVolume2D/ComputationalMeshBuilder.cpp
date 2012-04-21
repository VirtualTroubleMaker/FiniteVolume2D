#include "ComputationalMeshBuilder.h"

#include "FiniteVolume2DLib/Thread.hpp"
#include "FiniteVolume2DLib/Node.h"


ComputationalMeshBuilder::ComputationalMeshBuilder(Mesh::Ptr const & geometrical_mesh, BoundaryConditionCollection const & bc) : geometrical_mesh_(geometrical_mesh), bc_(bc) {}

ComputationalMesh::Ptr
ComputationalMeshBuilder::build() const {
    ComputationalMesh::Ptr cmesh(new ComputationalMesh(geometrical_mesh_->getMeshConnectivity()));


    // build computational nodes
    Thread<Node> const & interior_node_thread = geometrical_mesh_->getNodeThread(IGeometricEntity::INTERIOR);
    for (Thread<Node>::size_type i = 0; i < interior_node_thread.size(); ++i)
        cmesh->addNode(interior_node_thread.getEntityAt(i), ComputationalNode::Ptr(new ComputationalNode(interior_node_thread.getEntityAt(i))));

    Thread<Node> const & boundary_node_thread = geometrical_mesh_->getNodeThread(IGeometricEntity::BOUNDARY);
    for (Thread<Node>::size_type i = 0; i < boundary_node_thread.size(); ++i)
        cmesh->addNode(boundary_node_thread.getEntityAt(i), ComputationalNode::Ptr(new ComputationalNode(boundary_node_thread.getEntityAt(i))));


    // build computational faces
    Thread<Face> const & interior_face_thread = geometrical_mesh_->getFaceThread(IGeometricEntity::INTERIOR);
    for (Thread<Face>::size_type i = 0; i < interior_face_thread.size(); ++i)
        cmesh->addFace(interior_face_thread.getEntityAt(i), ComputationalFace::Ptr(new ComputationalFace(interior_face_thread.getEntityAt(i), NULL)));

    Thread<Face> const & boundary_face_thread = geometrical_mesh_->getFaceThread(IGeometricEntity::BOUNDARY);
    for (Thread<Face>::size_type i = 0; i < boundary_face_thread.size(); ++i)
    {
        BoundaryCondition::Ptr face_bc = NULL;

        Face::Ptr const & face = boundary_face_thread.getEntityAt(i);

        boost::optional<BoundaryConditionCollection::Pair> face_bc_opt = bc_.find(face->id());
        if (face_bc_opt)
            face_bc = BoundaryCondition::Ptr(new BoundaryCondition(*face_bc_opt));

        cmesh->addFace(boundary_face_thread.getEntityAt(i), ComputationalFace::Ptr(new ComputationalFace(boundary_face_thread.getEntityAt(i), face_bc)));
    }

    // build computational cells
    Thread<Cell> const & cell_thread = geometrical_mesh_->getCellThread();
    for (Thread<Cell>::size_type i = 0; i < cell_thread.size(); ++i)
        cmesh->addCell(cell_thread.getEntityAt(i), ComputationalCell::Ptr(new ComputationalCell(cell_thread.getEntityAt(i))));
    
    return cmesh;
}