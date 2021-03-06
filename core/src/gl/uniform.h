#pragma once

#include "util/variant.h"

#include "glm/glm.hpp"
#include "platform.h"
#include <string>
#include <vector>

namespace Tangram {

class ShaderProgram;

struct UniformTextureArray {
    std::vector<std::string> names;
    std::vector<int> slots;

    inline bool operator==(const UniformTextureArray& uta) {
        return uta.slots.size() == slots.size() && uta.slots == slots;
    };
};

using UniformArray1f = std::vector<float>;
using UniformArray2f = std::vector<glm::vec2>;
using UniformArray3f = std::vector<glm::vec3>;

/* Style Block Uniform types */
using UniformValue = variant<none_type, bool, std::string, float, int, glm::vec2, glm::vec3, glm::vec4,
    glm::mat2, glm::mat3, glm::mat4, UniformArray1f, UniformArray2f, UniformArray3f, UniformTextureArray>;


class UniformLocation {

public:
    UniformLocation(const std::string& _name) : name(_name) {}

private:
    const std::string name;

    mutable int location = -1;
    mutable int generation = -1;

    friend class ShaderProgram;
};

}
