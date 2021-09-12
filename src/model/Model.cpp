//
// Model class definition
//

#include <model/Model.h>

Vertex::Vertex() : position{}, normal{}, texture{} {}

Material::Material() : ambient{}, diffuse{}, specular{}, emissive{}, dissolve(1.0f),
    specularExp (1.0f), ior(1.0f) {}

Object::Object() : vertices{}, normals{}, uvs{}, faces{}, triangles{}, quads{}, 
    primitives{} {}

void Object::DrawTriangles() const {
    if (triangles.empty())
        return;

    glBegin(GL_TRIANGLES);

    for (uint32_t t = 0; t < triangles.size(); ++t)
        for (uint32_t v = 0; v < 3; ++v) {
            glNormal3fv(   &((normals[faces[triangles[t].offset + v * 3 + 2]])[0]));
            glTexCoord2fv( &((uvs[faces[triangles[t].offset + v * 3 + 1]])[0]));
            glVertex3fv(   &((vertices[faces[triangles[t].offset + v * 3]])[0]));
        }
    glEnd();
}

void Object::DrawQuads() const {
    if (quads.empty())
        return;

    glBegin(GL_TRIANGLES);

    for (uint32_t t = 0; t < quads.size(); ++t)
        for (uint32_t v = 0; v < 4; ++v) {
            glNormal3fv(   &((normals[faces[triangles[t].offset + v * 3 + 2]])[0]));
            glTexCoord2fv( &((uvs[faces[triangles[t].offset + v * 3 + 1]])[0]));
            glVertex3fv(   &((vertices[faces[triangles[t].offset + v * 3]])[0]));
        }
    glEnd();
}

Model::Model() : objects{} {}

void Model::Render() const {
    // draw all objects in the scene
    for (uint32_t o = 0; o < objects.size(); ++o) {
        UseMaterial(objects[o].material);
        objects[o].DrawQuads();
        objects[o].DrawTriangles();
    }
}

void Model::UseMaterial(const char* name) const {
    const Material& material = materialMap.at(name);
    glMaterialfv(GL_FRONT, GL_AMBIENT, &material.ambient[0]);
}