//
// Model class definition
//

#include <model/Model.h>

Mesh::Mesh() : positions{}, normals{}, UVs{}, faces{}, meshName{}, material{} {}

void Mesh::DeepCopy(const Mesh& original, Mesh& copy) {
    UI32 size;
    // copy vertex data
    size = original.positions.size();
    copy.positions.resize(size);
    std::memcpy(copy.positions.data(), original.positions.data(), size * sizeof(Vector3));
    
    size = original.normals.size();
    copy.normals.resize(size);
    std::memcpy(copy.normals.data(), original.normals.data(), size * sizeof(Vector3));
    
    size = original.UVs.size();
    copy.UVs.resize(size);
    std::memcpy(copy.UVs.data(), original.UVs.data(), size * sizeof(Vector2));

    // copy face data
    size = original.faces.size();
    copy.faces.resize(size);
    std::memcpy(copy.faces.data(), original.faces.data(), size * sizeof(UI32));

    // mesh name and material
    std::strcpy(copy.meshName, original.meshName);
    std::strcpy(copy.material, original.material);
}

void Mesh::DrawMesh() const {
    glBegin(GL_TRIANGLES);
    // PRINT("mesh faces %zi\n", faces.size());
    // PRINT("positions %zi\n normals %zi \nuvs %zi", positions.size(), normals.size(), UVs.size());
    for (uint32_t f = 0; f < faces.size(); f+=3) {
        glNormal3fv(&(normals[faces[f + 2]][0]));
        glTexCoord2fv(&(UVs[faces[f + 1]][0]));
        glVertex3fv(&(positions[faces[f]][0]));
    }
    glEnd();
}

Model::Model() : meshes{}, materials{} {}

void Model::DeepCopy(const Model& original, Model& copy) {
    // copy mesh data
    copy.meshes.resize(original.meshes.size());
    for (UI32 m = 0; m < copy.meshes.size(); ++m)
        Mesh::DeepCopy(original.meshes[m], copy.meshes[m]);
    // copy materials
    copy.materials = original.materials;
}

void Model::Render() const {
    // draw all objects in the scene
    for (uint32_t m = 0; m < meshes.size(); ++m) {
        UseMaterial(meshes[m].material);
        meshes[m].DrawMesh();
    }
}

void Model::UseMaterial(const char* name) const {
    if (std::strlen(name)) {
        const Material& material = materials.at(name);
        glMaterialfv(GL_FRONT, GL_AMBIENT, &material.ambient[0]);
        glMaterialfv(GL_FRONT, GL_EMISSION, &material.emissive[0]);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, &material.diffuse[0]);
        glMaterialfv(GL_FRONT, GL_SPECULAR, &material.specular[0]);
        float shininess[4] = {material.specularExp, material.specularExp, material.specularExp, material.specularExp};
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
}