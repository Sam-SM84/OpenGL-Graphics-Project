#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "commonLibraries.h"
#include "stbDefinition.h"

#include "Figure.h"
#include "Model.h"
#include <random>
#include "Object.h"
#include "Skybox.h"
#include "InventorySlot.h"
#include "Item.h"

void reshapeScreen(GLFWwindow* window, int width, int height);
void mouseMotion(GLFWwindow* window, double xpos, double ypos);
void movement_and_IO(GLFWwindow* window, float dt);
float getRealTime(float& t1, float& t2);
float getRandom1f(float min, float max);
void displayTitle(GLFWwindow* window, float dt);
void mouseFunc(GLFWwindow* window, int button, int action, int mods);
bool wait(float time, float dt);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

Figure player = Figure({ 0.0f, 0.0f, 3.0f });

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastMouseX = 800.0f / 2.0;
float lastMouseY = 600.0 / 2.0;
const float FOV = 45.0f;

float t1, t2 = 0;
float duration = 0;

float groundLevel = 0.0f;

bool pressed = false;

float renderDistance = 50.0f;

const float nearDistance = 30.0f;

bool inventory = false;

vector<Object> objects;
vector<Object> visibleObjects;

int main()
{
                                            // Initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Loading...", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
                                           // Functions
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, reshapeScreen);
    glfwSetCursorPosCallback(window, mouseMotion);
    glfwSetMouseButtonCallback(window, mouseFunc);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(false);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                                            // Shaders
    Shader modelShader("model_loading.vs", "model_loading.fs");
    Shader skyboxShader("skybox.vs", "skybox.fs");
    Shader gridShader("gridShader.vs", "gridShader.fs");

                                            // Skybox Initialization
    vector<string> faces
    {
        "skybox/px.png",
        "skybox/nx.png",
        "skybox/py.png",
        "skybox/ny.png",
        "skybox/pz.png",
        "skybox/nz.png"
    };

    Skybox skybox(faces);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

                                            // Initializing Models
    Model grassPath_near("Models/grass/10450_Rectangular_Grass_Patch_v1_iterations-2.obj");
    Model grassPath_far("Models/grass/ratio_0.1.obj");
    LODmodel grassPath{ &grassPath_far,&grassPath_near };

    Model tree_near("Models/Tree/Tree.obj");
    Model tree_far("Models/Tree/ratio_0.3.obj");
    LODmodel tree{ &tree_far,&tree_near };

    Model crocus_near("Models/crocus/12974_crocus_flower_v1_l3.obj");
    Model crocus_far("Models/crocus/ratio_0.01.obj");
    LODmodel crocus{ &crocus_far, &crocus_near };

    Model tulip_near("Models/tulip/ratio_0.1.obj");
    Model tulip_far("Models/tulip/ratio_0.005.obj");
    LODmodel tulip{ &tulip_far,&tulip_near };

    Model grass_near("Models/grass02/High Grass.obj");
    Model grass_far("Models/grass02/ratio_0.3.obj");
    LODmodel grass{ &grass_far,&grass_near };

    Model rock_("Models/rock/Rock1.obj");
    LODmodel rock{ &rock_,&rock_ };

                                            // Adding Objects into the world
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
        {
            objects.push_back(Object(&grassPath, { 0.0f + (float)i * 5,-1.0f,0.0f + (float)j * 5 }, { 0.05f,0.01f,0.05f }, { -90.0f,0.0f,0.0f}, false, false));
        }

    for (int i = 0; i < 25; i++)
        objects.push_back(Object(&tree, { getRandom1f(0,50),-0.9f,getRandom1f(0,50) }, { 1.0f,getRandom1f(0.8f,1.4f),1.0f }, { 0.0f,getRandom1f(0.0f,180.0f),0.0f }, false, true));

    for (int i = 0; i < 25; i++)
        objects.push_back(Object(&crocus, { getRandom1f(0,50),-0.9f,getRandom1f(0,50) }, { 0.015f,0.015f,0.01f }, { -90.0f,0.0f,0.0f }, true, true));

    for (int i = 0; i < 120; i++)
        objects.push_back(Object(&tulip, { getRandom1f(0,50),-0.9f,getRandom1f(0,50) }, { 0.025f,0.025f,0.025f }, { -90.0f,0.0f,0.0f }, false, true));

    for (int j = 0; j < 50; j++)
        objects.push_back(Object(&grass, { getRandom1f(0,50),-0.9f,getRandom1f(0,50) }, { 3.0f,3.0 * getRandom1f(0.8,1.3),3.0f }, { 0.0f,0.0f,0.0f }, false, true));

    for (int j = 0; j < 50; j++)
        objects.push_back(Object(&rock, { getRandom1f(0,50),-0.9f,getRandom1f(0,50) }, { 0.1f,0.1f,0.1f }, { 0.0f,0.0f,0.0f }, false, true));


                                            // Inventory setup
    InventorySlot::initialize("Images/slot1.png");

    Item sth(1, "Images/redx.png");
    Item null(0, "Images/redx.png");
    for (int i = 0; i < 5; i++)
    {
        player.bag[i].setup(&null, 1, glm::vec2(200 + 75 * i, 200), glm::vec2(250 + 75 * i, 250));
    }

    player.bag[2].setItem(&sth, 1);

                                            // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float dt = getRealTime(t1, t2);
        movement_and_IO(window, dt);
        player.applyGravity(groundLevel, 0.98, dt);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection, view, model;

        if (!inventory)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            modelShader.use();
            projection = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, renderDistance);
            view = glm::lookAt(player.Position,
                { player.Position.x + player.direction.x * cos(glm::radians(pitch)),
                player.Position.y + player.direction.y,
                player.Position.z + player.direction.z * cos(glm::radians(pitch)) }
            , { 0.0f,1.0f,0.0f });
            modelShader.setMat4("projection", projection);
            modelShader.setMat4("view", view);

            float limitAngle = glm::cos(glm::radians(FOV + 10.0f)) - 0.05f;
            visibleObjects.clear();

            // Frustum culling Optimization
            for (auto obj = objects.begin(); obj != objects.end(); obj++)
            {
                glm::vec3 objectVector = glm::normalize(obj->modelPosition - player.Position);
                if (glm::dot(glm::normalize(player.direction), objectVector) > limitAngle || glm::distance(player.Position, obj->modelPosition) < obj->radius)
                    visibleObjects.push_back(*obj);
            }

            for (auto obj = visibleObjects.begin(); obj != visibleObjects.end(); obj++)
            {
                // Level Of Detail Optimization
                obj->drawObject(modelShader, glm::distance(player.Position, obj->modelPosition) < nearDistance);
            }
        }

        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
            glfwSetCursor(window, cursor);

            gridShader.use();

            projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
            gridShader.setMat4("projection", projection);

            for (int i = 0; i < 5; i++)
            {
                player.bag[i].draw(gridShader, lastMouseX, SCR_HEIGHT - lastMouseY);
            }
        }

        if (wait(0.5f, dt)) displayTitle(window, dt);

        skybox.drawSkybox(skyboxShader, glm::mat4(glm::mat3(view)), projection);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void movement_and_IO(GLFWwindow* window, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !pressed) {
        inventory = !inventory; pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE && pressed) pressed = false;

    if (!inventory)
    {
        glm::vec3 futurePosition = { 0,0,0 };
        float cameraSpeed = (5 * dt);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cameraSpeed *= 2;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            futurePosition += glm::vec3(cameraSpeed * player.direction.x, 0, cameraSpeed * player.direction.z);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            futurePosition += glm::vec3(cameraSpeed * player.direction.x * -1, 0, cameraSpeed * player.direction.z * -1);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            futurePosition += glm::vec3(player.direction.z * cameraSpeed, 0, player.direction.x * cameraSpeed * -1);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            futurePosition += glm::vec3(player.direction.z * cameraSpeed * -1, 0, player.direction.x * cameraSpeed);

        


        player.moveFigure({ futurePosition.x,0,futurePosition.z });
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !player.onAir) player.jump();
    }
}

void reshapeScreen(GLFWwindow* window, int width, int height)
{
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
    glViewport(0, 0, width, height);
}

void mouseMotion(GLFWwindow* window, double xposIn, double yposIn)
{
    float currentMouseX = static_cast<float>(xposIn);
    float currentMouseY = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastMouseX = currentMouseX;
        lastMouseY = currentMouseY;
        firstMouse = false;
    }

    float dx = currentMouseX - lastMouseX;
    float dy = lastMouseY - currentMouseY; // reversed since y-coordinates go from bottom to top

    lastMouseX = currentMouseX;
    lastMouseY = currentMouseY;

    if (!inventory)
    {
        float sensitivity = 0.1f;

        yaw += dx * sensitivity;
        pitch += dy * sensitivity;

        if (pitch > 89.0) pitch = 89.0f;
        if (pitch < -89.0) pitch = -89.0f;

        if (yaw > 360) yaw -= 360;
        if (yaw < 0) yaw += 360;

        glm::vec3 reverse_direction;
        reverse_direction.x = cos(glm::radians(yaw));
        reverse_direction.y = sin(glm::radians(pitch));
        reverse_direction.z = sin(glm::radians(yaw));

        player.direction = glm::normalize(reverse_direction);
    }
}

float getRealTime(float& t1, float& t2)
{
    t2 = glfwGetTime();
    float d = t2 - t1;
    t1 = t2;
    return d;
}

float getRandom1f(float min, float max)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(min, max);
    return dist(gen);
}

void displayTitle(GLFWwindow* window, float dt)
{
    std::string fpsText = "Fps : " + std::to_string((int)(1.0f / dt));
    glfwSetWindowTitle(window, fpsText.c_str());
}

void mouseFunc(GLFWwindow* window, int button, int action, int mods)
{

}

bool wait(float timeDuration, float dt)
{
    if (duration < timeDuration)
    {
        duration += dt;
        return false;
    }

    else
    {
        duration = 0.0f;
        return true;
    }
}
