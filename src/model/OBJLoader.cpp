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
    std::vector<Object>::iterator obj;

    // while there are lines available
    while (fgets(buffer, MAX_LINE, pFile) != NULL) {
        switch (buffer[0]) {
        // vertex data*
        case 'v':
            switch(buffer[1]) {
            case ' ':
                model.vertices.push_back({
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token),
                    std::strtof(token, NULL)});
                break;
            case 'n':
                model.normals.push_back({
                    std::strtof(&buffer[3], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)});
                break;
            case 't':
                model.texCoords.push_back({
                    std::strtof(&buffer[3], &token), 
                    std::strtof(token, NULL)});
                break;
            }
            break;
        // face data
        case 'f': {
            // count occurences of whitespace
            uint32_t size = 0;
            char* faceData[4]; // max 4 accepted for quad

            token = std::strtok(&buffer[2], " ");
            // tokenise the faces string
            while ((token != NULL) && (size < 4)) {
                faceData[size] = token;
                ++size;
                token = std::strtok(NULL, " \n");
            }

            switch ((kPrimitives)size)
            {
            case kPrimitives::TRIANGLE: {
                uint32_t offset = (uint32_t)model.faces.size(); // offset into indices vector

                obj->triangles.push_back(Tri{offset, offset + 3, offset + 6});

                uint32_t primitiveData = model.faces.size();
                model.faces.resize(primitiveData + TRI_SIZE); // resize indices for new tri
                
                for (uint32_t f = 0; f < size; ++f) {
                    token = std::strtok(faceData[size], "/");
                    while (token != NULL) {
                        model.faces[primitiveData] = 
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
                uint32_t offset, j;
                offset = j = (uint32_t)model.faces.size(); // offset into indices vector

                obj->quads.push_back( Quad{offset, offset + 4, offset + 8});

                model.faces.resize(offset + QUAD_SIZE); // accomodate new primitive

                // tokenise the face strings into int
                std::cout << "\ntokenise face\n";
                for (uint32_t k = 0; k < 4; ++k) {
                    std::cout << "\nnew face vertex:\n\n";
                    token = std::strtok(faceData[offset & 3], "/"); // modulus offset by 4 for quad
                    for (uint32_t i = 0; token != NULL; i+=4) {
                        std::cout << "token: " << token << '\n';
                        model.faces[offset+i] = std::strlen(token) == 0 
                            ? 0 
                            : (uint32_t)std::strtol(token, NULL, 10) - 1; // obj are 1 indexed
                        std::cout << "offset: " << offset + i << '\n';
                        std::cout << "face index: " << model.faces[offset+i] << '\n';
                        token = strtok(NULL, "/");
                    }
                    ++offset;
                }
                std::cout << "face indices\n\n";
                for (uint32_t i = 0; i < QUAD_SIZE; i++) {
                    std::cout << model.faces[j+i] << ' ';
                }
                std::cout << "\nfrom quad:\n";
                for (uint32_t i = 0; i < 4; i++) {
                    std::cout << model.faces[(obj->quads.end() - 1)->_ver + i] + 1 << '/';
                    std::cout << model.faces[(obj->quads.end() - 1)->_tex + i] + 1 << '/';
                    std::cout << model.faces[(obj->quads.end() - 1)->_nor + i] + 1 << ' ';
                }
                std::cout << std::endl;
                // pad zeros if indices missing?

                // append primitive pointer
                obj->primitives.push_back( &(*(obj->quads.rbegin())) );
                break;
            }
            default: // unrecognised face
                break;
            }
        }
            break;
        // object
        case 'o':
            // create a new object in the model
            model.objects.push_back({});
            obj = model.objects.end() - 1;
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

            if (std::strcmp(token, "usemtl") == 0) {
                token = std::strtok(NULL, " \n");
                // set the object's material
                std::strncpy(obj->material, token, MAX_NAME_LENGTH);
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

    std::cout << "print face indices\n" << model.objects.size() << '\n';
    for (auto& f :  model.faces) {
        std::cout << f << ' ';
    }

    auto q = model.objects[0].quads.begin();
    std::cout << "\n\nprint obj\n";
    do {
        for (uint32_t i = 0; i < 4; i++) {
            std::cout << model.faces[q->_ver + i] + 1 << '/';
            std::cout << model.faces[q->_tex + i] + 1 << '/';
            std::cout << model.faces[q->_nor + i] + 1 << ' ';
        }
    }
    while (++q != model.objects[0].quads.end());

    for (uint32_t v = 0; v < model.vertices.size(); ++v) 
        std::cout << model.vertices[v] << '\n';
    for (uint32_t f = 0; f < model.faces.size(); ++f) 
        std::cout << model.faces[f] << '\n';

    // using quads
    for (Object& object : model.objects) {
        q = object.quads.begin();
        std::cout << "\n\nprint object " << object.name << " quads\n";
        std::cout << model.vertices.size() << "\n\n";

        do {
            for (uint32_t i = 0; i < 4; i++) {
                std::cout << model.vertices[model.faces[q->_ver + i]] << ' ';
                std::cout << model.texCoords[model.faces[q->_tex + i]].x << ' ';
                std::cout << model.texCoords[model.faces[q->_tex + i]].y << ' ';
                std::cout << model.normals[model.faces[q->_nor + i]] << '\n';
            }
            std::cout << "\n\n";
        }
        while (++q != object.quads.end());
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
                material = &model.materials[token];             
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
