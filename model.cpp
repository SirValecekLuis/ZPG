#include "model.h"

Model::Model(const float *data, const GLsizei size, const GLsizei stride)
    : data(data), size(size) {
    vbo = new VBO(data, size);
    vao = new VAO(vbo, nullptr, stride);

    is_set = true;
}

Model::~Model() {
    delete vbo;
    delete vao;
    delete ibo;

    if (has_texture) {
        glDeleteTextures(1, &textureID);
    }

    delete[] vertices;
    delete[] indices;
}

void Model::bind_vao() const {
    vao->bind_vao();
}

void Model::draw() const {
    if (is_set == false) {
        throw std::runtime_error("Model was not set!");
    }

    if (indices != nullptr && vertices != nullptr) {
        glDrawElements(GL_TRIANGLES, indicies_count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, size); // mode, first, count
    }

    glBindVertexArray(0);
}

void Model::set_material(const float ra, const float rd, const float rs) {
    material.set_coefficients(ra, rd, rs);
}

void Model::randomize_material() {
    material.randomize();
}

Material &Model::get_material() {
    return material;
}

void Model::create_2d_texture(const float *data, const GLsizei data_size, const char *path, const float uv_size) {
    this->data = data;
    this->size = data_size;

    const float plain_tex_coords[] = {
        0.0f, 0.0f,
        1.0f * uv_size, 0.0f,
        0.0f, 1.0f * uv_size,

        1.0f * uv_size, 0.0f,
        1.0f * uv_size, 1.0f * uv_size,
        0.0f, 1.0f * uv_size
    };

    constexpr int plain_size = sizeof(plain_tex_coords);

    vbo = new VBO(data, data_size);
    vao = new VAO(vbo, nullptr);

    const auto *vbo_coord = new VBO(plain_tex_coords, plain_size);
    vao->add_texture_coords(*vbo_coord);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    unsigned char *image = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_RGBA);

    if (image == nullptr) {
        throw std::runtime_error("Failed to load texture: " + std::string(SOIL_last_result()));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);

    has_texture = true;
    is_set = true;
}

void Model::create_3d_texture(const char *file_3d, const char *file_2d) {

    if (file_2d != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        textureID = SOIL_load_OGL_texture(file_2d, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
        if (textureID == 0) {
            std::cout << "An error occurred while loading texture." << std::endl;
            exit(EXIT_FAILURE);
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        has_texture = true;
    }

    Assimp::Importer importer;
    unsigned int importOptions = aiProcess_Triangulate //Converts polygons to triangles
                                 | aiProcess_OptimizeMeshes // Reduces the number of submeshes
                                 | aiProcess_JoinIdenticalVertices // Removes duplicate vertices
                                 | aiProcess_CalcTangentSpace; // Computes tangents and bitangents

    //aiProcess_GenNormals/ai_Process_GenSmoothNormals - Generates flat/Smooth normals

    const aiScene *scene = importer.ReadFile(file_3d, importOptions);

    if (scene) {
        //pokud bylo nacteni uspesne
        printf("scene->mNumMeshes = %d\n", scene->mNumMeshes);
        printf("scene->mNumMaterials = %d\n", scene->mNumMaterials);
        //Materials
        // for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        //     const aiMaterial *mat = scene->mMaterials[i];
        //     aiString name;
        //     mat->Get(AI_MATKEY_NAME, name);
        //     printf("Material [%d] name %s\n", i, name.C_Str());
        //     aiColor4D d;
        //     glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        //     if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &d))
        //         diffuse = glm::vec4(d.r, d.g, d.b, d.a);
        // }

        aiMesh *mesh = scene->mMeshes[0];
        vertices = new Vertex[mesh->mNumVertices];
        std::memset(vertices, 0, sizeof(Vertex) * mesh->mNumVertices);
        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            if (mesh->HasPositions()) {
                vertices[j].Position[0] = mesh->mVertices[j].x;
                vertices[j].Position[1] = mesh->mVertices[j].y;
                vertices[j].Position[2] = mesh->mVertices[j].z;
            }

            if (mesh->HasNormals()) {
                vertices[j].Normal[0] = mesh->mNormals[j].x;
                vertices[j].Normal[1] = mesh->mNormals[j].y;
                vertices[j].Normal[2] = mesh->mNormals[j].z;
            }

            if (mesh->HasTextureCoords(0)) {
                vertices[j].Texture[0] = mesh->mTextureCoords[0][j].x;
                vertices[j].Texture[1] = mesh->mTextureCoords[0][j].y;
            }

            if (mesh->HasTangentsAndBitangents()) {
                vertices[j].Tangent[0] = mesh->mTangents[j].x;
                vertices[j].Tangent[1] = mesh->mTangents[j].y;
                vertices[j].Tangent[2] = mesh->mTangents[j].z;
            }
        }

        if (mesh->HasFaces()) {
            indices = new unsigned int[mesh->mNumFaces * 3];
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                indices[j * 3] = mesh->mFaces[j].mIndices[0];
                indices[j * 3 + 1] = mesh->mFaces[j].mIndices[1];
                indices[j * 3 + 2] = mesh->mFaces[j].mIndices[2];
            }
        }

        vbo = new VBO(vertices, mesh->mNumVertices * sizeof(Vertex));
        ibo = new IBO(indices, mesh->mNumFaces * 3 * sizeof(GLuint));
        vao = new VAO(vbo, ibo, sizeof(Vertex));

        indicies_count = mesh->mNumFaces * 3;
    } else {
        printf("Error during parsing mesh from %s : %s \n", file_3d, importer.GetErrorString());
    }

    is_set = true;
}

void Model::bind_texture() const {
    if (has_texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
