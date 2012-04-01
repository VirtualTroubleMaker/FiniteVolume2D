/*
 * Name  : IGeometricEntity
 * Path  : 
 * Use   : Base class for vertices, faces, cells
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include "DeclSpec.h"

#include <boost/cstdint.hpp>


class DECL_SYMBOLS IGeometricEntity {
public:
    typedef boost::uint64_t Id_t;

public:
    ~IGeometricEntity() {}

    // Unique id of this entity
    virtual Id_t id() const = 0;
    virtual Id_t meshId() const = 0;
};
