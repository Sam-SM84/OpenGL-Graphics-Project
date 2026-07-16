#include "Gridbox.h"

Gridbox::Gridbox() 
{ 
    itemID = 0;
    count = 0; 
    VBO = 0;

}

void Gridbox::setup()
{
    float vertices[] =
    {
        400.0f,300.0f,0.0f,
        500.0f,300.0f,0.0f,
        500.0f,200.0f,0.0f,
        400.0f,200.0f,0.0f
    };

    unsigned int indeces[]
    {
        0,1,3,
        2,3,1,
    };

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

void Gridbox::setItem(unsigned int _itemID, int _count)
{
	itemID = _itemID;
	count = _count;
}

void Gridbox::draw(Shader& shader)
{

    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
}