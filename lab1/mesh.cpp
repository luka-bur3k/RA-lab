#include "mesh.hh"
#include "bspline.hh"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
    : vertices(vertices), indices(indices)
{
}

void Mesh::draw()
{
    glBegin(GL_TRIANGLES);

    for (Vertex &vert : vertices)
    {
        glVertex3f(vert.position.x, vert.position.y, vert.position.z);
        glNormal3f(vert.normal.x, vert.normal.y, vert.normal.z);
    }

    glEnd();
}

void Mesh::drawAll(float fake_time)
{

    glColor3f(0.2f, 0.7f, 0.8f);
    glPushMatrix();
    glTranslatef(0.5f, 0.0f, 0.0f);
    if(Bspline::splines.size() > 0)
    {
        VertexOnRoids translationVec = Bspline::splines[0]->getPositionOnCurve(fake_time);
        glTranslatef(translationVec.position.x,translationVec.position.y,translationVec.position.z);
        float cosAngle = glm::dot(glm::vec3(1,0,0), translationVec.tangent) / glm::length(translationVec.tangent);
        float angle = glm::acos(cosAngle) / 3.141352f * 180.f;
        glm::vec3 crossProd = glm::cross(glm::vec3(1,0,0), translationVec.tangent);
        glRotatef(angle, crossProd.x, crossProd.y, crossProd.z);
    } 
    glScalef(1.0f, 1.0f, 1.0f);
    for (auto &it : Mesh::meshes)
    {
        it->draw();
    }
    glPopMatrix();
}

std::vector<std::unique_ptr<Mesh>> Mesh::meshes;