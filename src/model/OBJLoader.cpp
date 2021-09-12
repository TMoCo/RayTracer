//
// OBJLoader class definition
//

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
    std::strcpy(dir, path); // get a modifiable copy of the directory

    char* slash = std::strchr(dir,'/');
    uint32_t pos = 0;
    while (slash!=NULL) { // find last occurence of '/' 
        pos = slash-dir+1;
        slash = std::strchr(slash+1, '/');
    }
    dir[pos] = '\0'; // terminate string 

    // input
    char buffer[MAX_LINE];
    char* token;

    // model data
    Object* obj;

    // vertex data (initialised with empty)
    std::vector<Vector3> vertices = {};
    std::vector<Vector3> normals = {};
    std::vector<Vector2> uvs = {};

    // while there are lines available
    while (fgets(buffer, MAX_LINE, pFile) != NULL) {
        switch (buffer[0]) {
        // vertex data*
        case 'v':
            switch(buffer[1]) {
            case ' ':
                vertices.push_back({
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)});
                break;
            case 'n':
                normals.push_back({
                    std::strtof(&buffer[3], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)});
                break;
            case 't':
                uvs.push_back({
                    std::strtof(&buffer[3], &token), 
                    std::strtof(token, NULL)});
                break;
            }
            break;
        // face data
        case 'f': {
            // count occurences of whitespace
            uint32_t size = 0;
            char* face[4]; // max 4 accepted for quad

            token = std::strtok(&buffer[2], " ");
            // tokenise the faces string
            while ((token != NULL) && (size < 4)) {
                face[size] = token;
                ++size;
                token = std::strtok(NULL, " \n");
            }

            switch ((kPrimitives)size)
            {
            case kPrimitives::TRIANGLE: {
                obj->triangles.push_back(Tri{(uint32_t)obj->faces.size()});

                uint32_t primitiveData = obj->faces.size();
                obj->faces.resize(primitiveData + Tri::size);
                
                for (uint32_t f = 0; f < size; ++f) {
                    token = std::strtok(face[size], "/");
                    while (token != NULL) {
                        obj->faces[primitiveData] = 
                            std::strlen(token) == 0 
                                ? 0 
                                : (uint32_t)std::strtol(token, NULL, 10);
                        ++primitiveData;
                        token = std::strtok(NULL, "/");
                    }
                }
                // append primitive
                obj->primitives.push_back(&(*(obj->triangles.rbegin())));
                break;
            }
            case kPrimitives::QUAD: {
                // push a new quad, using current indices size as offset
                obj->quads.push_back(Quad((uint32_t)obj->faces.size()));
                uint32_t primitiveData = obj->faces.size();
                obj->faces.resize(primitiveData + Quad::size);
                // tokenise the face strings into int
                for (uint32_t f = 0; f < size; ++f) {
                    token = std::strtok(face[f], "/");
                    while (token != NULL) {
                        obj->faces[primitiveData] = std::strlen(token) == 0 
                            ? 0 
                            : (uint32_t)std::strtol(token, NULL, 10);
                        ++primitiveData;
                        token = strtok(NULL, "/");
                    }
                }
                // append primitive
                obj->primitives.push_back(&(*(obj->quads.rbegin())));
                break;
            }
            default:
                break;
            }
        }
            break;
        // object
        case 'o':
            // create a new object in the model
            model.objects.push_back({});
            obj = &(*model.objects.rbegin());
            std::strcpy(obj->name, &buffer[2]);
            break;
        // comments
        case '#':
            break;
        case '\n':
            break;
        // other string type
        default:
            token = std::strtok(buffer, " ");
            if (std::strcmp(token, "mtllib") == 0) {
                token = std::strtok(NULL, " \n");
                // concatenate 
                char mtlPath[std::strlen(token) + std::strlen(dir)];
                std::strcpy(mtlPath, dir);
                std::strcat(mtlPath, token);
                LoadMtl(mtlPath, model);
                break;
            }

            if (std::strcmp(token, "usemtl") == 0) {
                token = std::strtok(NULL, " \n");
                // set the object's material
                std::strncpy(obj->material, token, MAX_NAME_LENGTH);
                break;
            }
            break;
        }
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

    Material* material;

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
                model.materialMap[token] = Material{};
                material = &model.materialMap[token];                 
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
