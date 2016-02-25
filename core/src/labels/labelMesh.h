#pragma once

#include "gl/mesh.h"
#include "labels/label.h"

#include <memory>
#include <vector>

namespace Tangram {

static constexpr size_t maxLabelMeshVertices = 16384;

class ShaderProgram;

class LabelMesh : public Mesh<Label::Vertex> {
public:
    LabelMesh(std::shared_ptr<VertexLayout> _vertexLayout, GLenum _drawMode);

    virtual ~LabelMesh() override;

    virtual void draw(ShaderProgram& _shader) override;

    void compile(std::vector<Label::Vertex>& _vertices);

    bool compiled() { return m_isCompiled; }

    static void loadQuadIndices();

};

}
