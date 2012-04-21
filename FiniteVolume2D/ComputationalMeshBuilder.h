/*
 * Name  : ComputationalMeshBuilder
 * Path  : 
 * Use   : Builds a computational mesh from a geometrical mesh.
 *         Nodes -> ComputationalNodes
 *         Faces -> ComputationalFaces
 *         Cells -> ComputationalCells
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "DeclSpec.h"

#include "FiniteVolume2DLib/Mesh.h"
#include "FiniteVolume2DLib/BoundaryConditionCollection.h"

#include "ComputationalMesh.h"


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D ComputationalMeshBuilder {
public:
    explicit ComputationalMeshBuilder(Mesh::Ptr const & mesh, BoundaryConditionCollection const & bc);

    ComputationalMesh::Ptr build() const;

private:
    // geometric mesh to convert to a computational one
    Mesh::Ptr geometrical_mesh_;

    // all face boundary conditions
    BoundaryConditionCollection const & bc_;
};

#pragma warning(default:4251)
