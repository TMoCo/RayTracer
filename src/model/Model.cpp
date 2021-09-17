//
// Model class definition
//

#include <model/Model.h>

Mesh::Mesh() : positions{}, normals{}, UVs{}, faces{}, meshName{}, material{} {}

void Mesh::DrawMesh() const {
    glBegin(GL_TRIANGLES);
    for (uint32_t f = 0; f < faces.size(); f+=3) {
        glNormal3fv(&(normals[faces[f + 2]][0]));
        glTexCoord2fv(&(UVs[faces[f + 1]][0]));
        glVertex3fv(&(positions[faces[f]][0]));
    }
    glEnd();
}

Model::Model() : meshes{}, materials{} {}

void Model::Render() const {
    // draw all objects in the scene
    for (uint32_t m = 0; m < meshes.size(); ++m) {
        UseMaterial(meshes[m].material);
        meshes[m].DrawMesh();
    }
}


void Model::UseMaterial(const char* name) const {
    const Material& material = materials.at(name);
    glMaterialfv(GL_FRONT, GL_AMBIENT, &material.ambient[0]);
    glMaterialfv(GL_FRONT, GL_EMISSION, &material.emissive[0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, &material.diffuse[0]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, &material.specular[0]);
    float shininess[4] = {material.specularExp, material.specularExp, material.specularExp, material.specularExp};
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}