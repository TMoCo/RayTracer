//
// Model class definition
//

#include <model/Model.h>

Object::Object() : triangles{}, quads{}, primitives{} {}

Model::Model() : objects{} {}

void Model::Render() const {
    // draw all objects in the scene
    for (uint32_t o = 0; o < objects.size(); ++o) {
        UseMaterial(objects[o].material);
        // quads
        DrawQuads(objects[o]);
        // tris
        DrawTris(objects[o]);
    }
}


void Model::DrawTris(const Object& obj) const {
    glBegin(GL_TRIANGLES);
    std::vector<Tri>::const_iterator t = obj.triangles.begin();
    while (t != obj.triangles.end()) { // automatically exits when tri is empty
        for (uint32_t v = 0; v < 3; ++v) {
            glNormal3fv(
                &(normals  [faces[t->_nor + v]][0]));
            glTexCoord2fv(
                &(texCoords[faces[t->_tex + v]][0]));
            glVertex3fv(
                &(vertices [faces[t->_ver + v]][0]));
        }
        t++;
    }
    glEnd();
}

void Model::DrawQuads(const Object& obj) const {
    glBegin(GL_QUADS);
    std::vector<Quad>::const_iterator q = obj.quads.begin();
    while (q != obj.quads.end()) { // automatically exits when tri is empty
        for (uint32_t v = 0; v < 4; ++v) {
            glNormal3fv(
                &(normals  [faces[q->_nor + v]][0]));
            glTexCoord2fv(
                &(texCoords[faces[q->_tex + v]][0]));
            glVertex3fv(
                &(vertices [faces[q->_ver + v]][0]));
        }
        q++;
    }
    glEnd();
}


void Model::UseMaterial(const char* name) const {
    const Material& material = materials.at(name);
    glMaterialfv(GL_FRONT, GL_AMBIENT, &material.ambient[0]);
}