//
// Model class definition
//

#include <scene/Model.h>
  /*

Mesh::Mesh() : positions{}, normals{}, UVs{}, faces{}, meshName{}, material{} {}

void Mesh::DeepCopy(const Mesh& original, Mesh& copy) {
    UI32 size;
    // copy vertex data
    size = static_cast<UI32>(original.positions.size());
    copy.positions.resize(size);
    std::memcpy(copy.positions.data(), original.positions.data(), size * sizeof(Vector3));
    
    size = static_cast<UI32>(original.normals.size());
    copy.normals.resize(size);
    std::memcpy(copy.normals.data(), original.normals.data(), size * sizeof(Vector3));
    
    size = static_cast<UI32>(original.UVs.size());
    copy.UVs.resize(size);
    std::memcpy(copy.UVs.data(), original.UVs.data(), size * sizeof(Vector2));

    // copy face data
    size = static_cast<UI32>(original.faces.size());
    copy.faces.resize(size);
    std::memcpy(copy.faces.data(), original.faces.data(), size * sizeof(UI32));

    // mesh name and material
    std::strncpy(copy.meshName, original.meshName, MAX_NAME_LENGTH);
    std::strncpy(copy.material, original.material, MAX_NAME_LENGTH);
}

void Mesh::DrawMesh() const {
    glBegin(GL_TRIANGLES);
    // PRINT("mesh faces %zi\n", faces.size());
    // PRINT("positions %zi\n normals %zi \nuvs %zi", positions.size(), normals.size(), UVs.size());
    for (uint32_t f = 0; f < faces.size(); f+=3) {
        glNormal3fv(normals[faces[f + 2]]._v);
        glTexCoord2fv(UVs[faces[f + 1]]._v);
        glVertex3fv(positions[faces[f]]._v);
    }
    glEnd();
}

Model::Model() : meshes{}, materials{} {}

void Model::DeepCopy(const Model& original, Model& copy) {
    // copy mesh data
    copy.meshes.resize(original.meshes.size());

    std::vector<Mesh>::iterator mesh = copy.meshes.begin();

    // copy the objects in the scene
    for (UI32 o = 0; o < original.objects.size(); ++o) {
        Mesh::DeepCopy(*original.objects[o], *mesh);
        copy.objects.push_back(&(*mesh));
        ++mesh;
    }

    // copy the lights
    for (UI32 l = 0; l < original.lights.size(); ++l) {
        Mesh::DeepCopy(*original.lights[l], *mesh);
        copy.lights.push_back(&(*mesh));
        ++mesh;
    }
    
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
  /*
    if (std::strlen(name)) {
        const Material& material = materials.at(name);
        // glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient._v);
        glMaterialfv(GL_FRONT, GL_EMISSION, material.emissive._v);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse._v);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular._v);
        float shininess[4] = {material.specularExp, material.specularExp, material.specularExp, material.specularExp};
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
 
}
*/