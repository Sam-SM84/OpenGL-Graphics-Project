#pragma once

struct HUD
{
	float x, y;
	unsigned int texture;
    unsigned int VAO;
    float size;
    glm::vec3 color;

	void load(const char* path,float w,float h,float _size,glm::vec3 _color)
	{
        size = _size;
        x = w;
        y = h;
        color = _color;

        float vertices[] =
        {
            0.0f, 0.0f, 0.0f, 
            1.0f, 0.0f, 0.0f, 
            1.0f, 1.0f, 0.0f, 
            0.0f, 1.0f, 0.0f, 
        };

        unsigned int indeces[]
        {
                0,1,3,
                2,3,1,
        };

        unsigned int VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glGenTextures(1, &texture);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }
	}

    void draw()
    {
        glDisable(GL_DEPTH_TEST);

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }

    void setShader(Shader& shader,glm::mat4 projection)
    {
        shader.setMat4("projection", projection);
        shader.setInt("texture", 0);        // 0 -> GL_TEXTURE0
        shader.setFloat("size", size);
        shader.setVec2("position", x, y);
        shader.setVec3("color", color);
    }
};
