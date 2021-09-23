//
// OBJLoader class definition
//

#include <model/Material.h>
#include <model/OBJLoader.h>

#include <cstdio>
#include <cstring>
#include <algorithm>

bool OBJLoader::LoadObj(const std::string& path, Model& model) {
    return LoadObj(path.c_str(), model);
}

bool OBJLoader::LoadObj(const char* path, Model& model) {
    if (IsObj(path) != 0) {
        std::cerr << "Not a .obj file!\n";
        return false;
    }

    std::FILE* pFile = std::fopen(path, "r");
    if (pFile == NULL)
        std::perror("Error opening file!");

    char dir[std::strlen(path)];
    std::strcpy(dir, path); // get a modifiable copy of given .obj path

    char* slash = std::strchr(dir,'/');
    UI32 pos = 0;
    while (slash!=NULL) { // find last occurence of '/' 
        pos = slash-dir+1;
        slash = std::strchr(slash+1, '/');
    }
    dir[pos] = '\0'; // terminate string at '/'

    // input
    char buffer[MAX_LINE];
    char* token;

    // model data
    std::vector<Mesh>::iterator mesh;

    // while there are lines available
    while (std::fgets(buffer, MAX_LINE, pFile) != NULL) {
        switch (buffer[0]) {
        // vertex data*
        case 'v':
            switch(buffer[1]) {
            case ' ':
                mesh->positions.push_back({
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token),
                    std::strtof(token, NULL)});
                break;
            case 'n':
                mesh->normals.push_back({
                    std::strtof(&buffer[3], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)});
                break;
            case 't':
                mesh->UVs.push_back({
                    std::strtof(&buffer[3], &token), 
                    std::strtof(token, NULL)});
                break;
            }
            break;
        case 'f': { // face data
            // get vertices in face (max 4 for quad)
            char* vertices[4];
            UI32 vertCount = 0;

            // tokenise vertices by ' '
            char* token = std::strtok(&buffer[2], " \n");
            while (token != NULL)
            {
                if (vertCount == 4) {
                    std::cerr << "Can't load 5+ sided polygons!" << std::endl; 
                    break;
                    // handle error here (skip line for now)
                }
                vertices[vertCount++] = token;
                token = strtok(NULL, " \n");
            }

            if (vertCount > 4)
                break;

            // get offset into faces
            UI32 offset = mesh->faces.size();
            mesh->faces.resize(offset + (vertCount == 4 ? 18 : 9));

            // process vertex data
            UI32 i;
            for (UI32 v = 0; v < vertCount; ++v) {
                token = strtok(vertices[v], "/");
                for (i = 0; token != NULL; i++) {
                    mesh->faces[offset + (v * 3) + i] = 
                            std::strlen(token) == 0 
                                ? 0 
                                : (UI32)std::strtol(token, NULL, 10) - 1;
                    token = std::strtok(NULL, "/");
                }
                if (i % 3) {
                    std::cerr << "Error processing face" << std::endl; 
                    mesh->faces.resize(offset);
                    // handle error (resize to original faces size and skip face)
                    break;
                }
            }

            if (i % 3) 
                break;

            /*
            auto it = mesh->faces.begin() + offset;
            while (it != mesh->faces.end()) {
                std::cout << *it << ' ';
                it++;
            }
            std::cout << std::endl;
            */

            if (vertCount == 4) { // append 2 more vertex indices to complete the quad
                // v4 = v0
                mesh->faces[offset + 12] = mesh->faces[offset];
                mesh->faces[offset + 13] = mesh->faces[offset + 1];
                mesh->faces[offset + 14] = mesh->faces[offset + 2];
                // v5 = v2
                mesh->faces[offset + 15] = mesh->faces[offset + 6];
                mesh->faces[offset + 16] = mesh->faces[offset + 7];
                mesh->faces[offset + 17] = mesh->faces[offset + 8];
            }

            /*
            it = mesh->faces.begin() + offset;
            while (it != mesh->faces.end()) {
                std::cout << *it << ' ';
                it++;
            }
            */
            break;
        }
        case 'o': // object
            // create a new mesh object in the model
            model.meshes.push_back({});
            mesh = model.meshes.end() - 1;
            std::strcpy(mesh->meshName, &buffer[2]);
            break;
        case '#': // comments
            break;
        case '\n':
            break; // other string type
        default:
            token = std::strtok(buffer, " ");

            if (std::strcmp(token, "usemtl") == 0) {
                token = std::strtok(NULL, " \n");
                // set the object's material
                std::strncpy(mesh->material, token, MAX_NAME_LENGTH);
                break;
            }

            if (std::strcmp(token, "mtllib") == 0) {
                token = std::strtok(NULL, " \n");
                // concatenate mtl path to current dir
                char mtlPath[std::strlen(token) + std::strlen(dir)];
                std::strcpy(mtlPath, dir);
                std::strcat(mtlPath, token);
                LoadMtl(mtlPath, model);
                break;
            }
            break;
        }
    }

    /*
    std::cout << "loaded\n";
    std::cout << model.meshes.size() << "\n";
    std::cout << model.meshes.begin()->faces.size() << "\n";

    for (auto& pos : model.meshes[0].positions) {
        std::cout << pos << "\n";
    }

    for (auto& face : model.meshes[0].faces) {
        std::cout << face << " ";
    }
    std::cout << std::endl;
    */
    
    // update face indices for separate objects
    if (model.meshes.size() > 1) {
        mesh = model.meshes.begin();
        UI32 pOffset = mesh->positions.size();
        UI32 tOffset = mesh->UVs.size();
        UI32 nOffset = mesh->normals.size();
        do {
            mesh++;
            for (UI32 f = 0; f < mesh->faces.size(); f+= 3) {
                mesh->faces[f    ] -= pOffset;
                mesh->faces[f + 1] -= tOffset;
                mesh->faces[f + 2] -= nOffset;
            }
            pOffset += mesh->positions.size();
            tOffset += mesh->UVs.size();
            nOffset += mesh->normals.size();
        }
        while ((mesh + 1) != model.meshes.end());
    }

    // set pointers to emissive objects
    for (mesh = model.meshes.begin(); mesh < model.meshes.end(); ++mesh) {
        // sum of terms greater than 1 means a channel other than alpha is emitting light
        if (model.materials.at(mesh->material).emissive.Sum() > 1.0f) 
            model.lights.push_back(&(*mesh)); 
        else
            model.objects.push_back(&(*mesh)); 
    }
    
    std::fclose(pFile);
    return true;
}

bool OBJLoader::LoadMtl(const char* path, Model& model) {
    if (IsMtl(path) != 0) {
        std::cerr << "Not a .mtl file!\n";
        return false;
    }

    FILE* pFile = std::fopen(path, "r");
    if (pFile == NULL)
        std::perror("Error opening file");

    // input
    char buffer[MAX_LINE];
    char* token;

    Material* material = nullptr;

    while (std::fgets(buffer, MAX_LINE, pFile) != NULL) {
        switch (buffer[0]) {
        case 'N':
            switch (buffer[1]) {
            case 's':
                token = std::strtok(&buffer[3], " \n");
                material->specularExp = std::strtof(token, NULL);
                break;
            case 'i':
                token = std::strtok(&buffer[3], " \n");
                material->ior = std::strtof(token, NULL);
                break;
            }
            break;
        case 'K':
            switch (buffer[1]) {
            case 'a':
                material->ambient = {
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            case 'd':
                material->diffuse = {
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            case 's':
                material->specular = {
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            case 'e':
                material->emissive = {
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            }
        case 'd':
            material->dissolve = std::strtof(&buffer[2], NULL);
            break;
        default:
            token = std::strtok(buffer, " ");
            if (std::strcmp(token, "newmtl") == 0) {
                token = std::strtok(NULL, " \n");
                // insert a new material into the map
                material = &(model.materials[token]);
            }

            if (std::strcmp(token, "illum") == 0) {
                // illumination mode
                break;
            }
            break;
        }
    }

    std::fclose(pFile);
    return true;
}

bool OBJLoader::IsFileType(const char* path, const char* extension) {
    int len = std::strlen(path);
    if (len > 4)
        return std::strcmp(&path[len - 4], extension);
    return false;
}

bool OBJLoader::IsObj(const char* path) {
    return IsFileType(path, ".obj");
}

bool OBJLoader::IsMtl(const char* path) {
    return IsFileType(path, ".mtl");
}
