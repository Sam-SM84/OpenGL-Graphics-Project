#pragma once

struct Plane
{
    glm::vec3 direction;        // which way is the plane facing -> n
    float distance;             // distance from players POV

    void normalize()            // normalize (for calcualting)
    {
        float length = glm::length(direction);
        direction /= length;
        distance /= length;
    }
    // linear algebra formula
    float planeDistance(const glm::vec3& p)
    {
        return glm::dot(direction, p) + distance;
    }
};

struct Frustum
{
    Plane planes[6];

    enum
    {
        LEFT = 0, RIGHT, BOTTOM, TOP, NEAR, FAR
    };
    // another linear algebra formula
    void extract(const glm::mat4& vp)
    {
        planes[LEFT].direction.x = vp[0][3] + vp[0][0];
        planes[LEFT].direction.y = vp[1][3] + vp[1][0];
        planes[LEFT].direction.z = vp[2][3] + vp[2][0];
        planes[LEFT].distance = vp[3][3] + vp[3][0];

        planes[RIGHT].direction.x = vp[0][3] - vp[0][0];
        planes[RIGHT].direction.y = vp[1][3] - vp[1][0];
        planes[RIGHT].direction.z = vp[2][3] - vp[2][0];
        planes[RIGHT].distance = vp[3][3] - vp[3][0];

        planes[BOTTOM].direction.x = vp[0][3] + vp[0][1];
        planes[BOTTOM].direction.y = vp[1][3] + vp[1][1];
        planes[BOTTOM].direction.z = vp[2][3] + vp[2][1];
        planes[BOTTOM].distance = vp[3][3] + vp[3][1];

        planes[TOP].direction.x = vp[0][3] - vp[0][1];
        planes[TOP].direction.y = vp[1][3] - vp[1][1];
        planes[TOP].direction.z = vp[2][3] - vp[2][1];
        planes[TOP].distance = vp[3][3] - vp[3][1];

        planes[NEAR].direction.x = vp[0][3] + vp[0][2];
        planes[NEAR].direction.y = vp[1][3] + vp[1][2];
        planes[NEAR].direction.z = vp[2][3] + vp[2][2];
        planes[NEAR].distance = vp[3][3] + vp[3][2];

        planes[FAR].direction.x = vp[0][3] - vp[0][2];
        planes[FAR].direction.y = vp[1][3] - vp[1][2];
        planes[FAR].direction.z = vp[2][3] - vp[2][2];
        planes[FAR].distance = vp[3][3] - vp[3][2];

        for (int i = 0; i < 6; i++)
            planes[i].normalize();
    }

    bool isSphereVisible(const glm::vec3& center, float radius)
    {
        for (int i = 0; i < 6; i++)
        {
            float dist = planes[i].planeDistance(center);
            if (dist < -radius)             // |dist| < radius
                return false;
        }
        return true;
    }
};

struct LODmodel
{
    Model* far;
    Model* near;
};

void backFaceCulling()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

