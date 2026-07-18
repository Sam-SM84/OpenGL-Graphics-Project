#include "Gridbox.h"

unsigned int Gridbox::VAO = 0;
unsigned int Gridbox::VBO = 0;
unsigned int Gridbox::EBO = 0;

unsigned int Gridbox::indeces[]
{
        0,1,3,
        2,3,1,
};

float Gridbox::vertices[]
{
        0.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f
};


Gridbox::Gridbox()
{
    itemID = 0;
    count = 0;
    minPos = glm::vec2(0,0);
    maxPos = glm::vec2(0,0);
    color =  glm::vec3(0,0,0);
}

void Gridbox::initialize()
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void Gridbox::setup(glm::vec2 min, glm::vec2 max, glm::vec3 _color)
{
    itemID = 0;
    count = 0;
    minPos = min;
    maxPos = max;
    color = _color;
}

void Gridbox::setItem(unsigned int _itemID, int _count)
{
	itemID = _itemID;
	count = _count;
}

void Gridbox::draw(Shader& shader)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(minPos.x,minPos.y, 0));
    model = glm::scale(model, glm::vec3(maxPos.x - minPos.x, maxPos.y - minPos.y, 1));
    shader.setMat4("model", model);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
}

bool Gridbox::hovers(float mouseX, float mouseY)
{
    return
        (
            mouseX > minPos.x && mouseX < maxPos.x &&
            mouseY > minPos.y && mouseY < maxPos.y
        );
}
