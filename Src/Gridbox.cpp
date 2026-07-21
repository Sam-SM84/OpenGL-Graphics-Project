#include "InventorySlot.h"
#include <stb_image.h>
unsigned int InventorySlot::VAO = 0;
unsigned int InventorySlot::VBO = 0;
unsigned int InventorySlot::EBO = 0;
unsigned int InventorySlot::slotTexture = 0;

unsigned int InventorySlot::indeces[]
{
        0,1,3,
        2,3,1,
};

float InventorySlot::vertices[] =
{
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};


InventorySlot::InventorySlot()
{
    item = nullptr;
    count = 0;
    minPos = glm::vec2(0,0);
    maxPos = glm::vec2(0,0);
}

void InventorySlot::initialize(const char* path)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO <- Array Buffer (data)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &slotTexture);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format;
        if (nrChannels == 1) format = GL_RED;
        if (nrChannels == 3) format = GL_RGB;
        if (nrChannels == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, slotTexture);
        //glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
        std::cout << "Slot texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}

void InventorySlot::setup(Item* _item,unsigned int _count,glm::vec2 min, glm::vec2 max)
{
    item = _item;
    count = _count;
    minPos = min;
    maxPos = max;
}

void InventorySlot::setItem(Item* _item, unsigned int _count)
{
    item = _item;
    count = _count;
}


void InventorySlot::draw(Shader& shader,float mouseX,float mouseY)
{
    float scale = hovers(mouseX, mouseY) ? 3 : 1;

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(glm::vec2((maxPos + minPos) / 2.0f), 0.0f));  
    model = glm::scale(model, glm::vec3((maxPos.x - minPos.x) * scale, (maxPos.y - minPos.y) * scale, 1));
    model = glm::translate(model, glm::vec3(-0.5, -0.5, -0.5));

    shader.setMat4("model", model);
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, slotTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, slotTexture);
    if (item != nullptr && item->ID != 0)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, item->itemTexture);
    }

    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, slotTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

bool InventorySlot::hovers(float mouseX, float mouseY)
{
    return
        (
            mouseX > minPos.x && mouseX < maxPos.x&&
            mouseY > minPos.y && mouseY < maxPos.y
            );
}
