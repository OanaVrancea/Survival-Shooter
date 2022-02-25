#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill)
{

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    vertices.push_back(VertexFormat(center, color));

    for (float angle = 0.0f; angle < 360; angle++) {
        float rad = M_PI * angle / 180;
        int x = radius * cos(rad);
        int y = radius * sin(rad);
        vertices.push_back(VertexFormat(glm::vec3(center.x + x, center.y + y, 0), color));
    }

    for (unsigned int i = 1; i < vertices.size() - 1; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(vertices.size() - 1);
        indices.push_back(1);
    }

    circle->InitFromData(vertices, indices);

    return circle;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 corner,
    float length,
    float width,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2 };

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

