#include "particle.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstdlib>

#define BILLsBOARD

Particle::Particle(glm::vec3 pos, GLfloat inGameTime) : position(pos), spawnedTime(inGameTime)
{
    //
}

void populateVec(std::vector<GLfloat> &vec, glm::vec3 center, float timeToLive, glm::vec3 ociste)
{
 /*    printf("(%f,%f,%f)->", center.x, center.y, center.z);
    glm::vec3 temp2{center.x, center.y, center.z};
    temp2 = glm::rotate(rotation, temp2);
    printf("(%f,%f,%f)\n", temp2.x, temp2.y, temp2.z);
 */
    glm::quat rotation = glm::rotation(glm::vec3{0,0,1}, glm::normalize(ociste));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.f);
    glm::mat4 view = glm::lookAt(
        ociste,
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );

    glm::vec4 temp{center.x - 0.05, center.y - 0.05, center.z, 1.0f};
#ifdef BILLBOARD
    temp = glm::rotate(rotation, temp);
#endif    
    //printf("(%f,%f,%f)->", temp.x, temp.y, temp.z);
    temp = view * temp;
    //printf("(%f,%f,%f)->", temp.x, temp.y, temp.z);
    temp = projection * temp;
    printf("(%f,%f,%f)\n", temp.x, temp.y, temp.z);
    vec.emplace_back(temp.x);
    vec.emplace_back(temp.y);
    vec.emplace_back(temp.z);


    vec.emplace_back(timeToLive/5.0);
    vec.emplace_back(timeToLive/5.0);
    vec.emplace_back(timeToLive/5.0);

    temp = glm::vec4{center.x - 0.05, center.y + 0.05, center.z, 1.0f};
#ifdef BILLBOARD
    temp = glm::rotate(rotation, temp);
#endif    
    temp = projection * view * temp;

    vec.emplace_back(temp.x);
    vec.emplace_back(temp.y);
    vec.emplace_back(temp.z);
    
    vec.emplace_back(timeToLive/5.0);
    vec.emplace_back(timeToLive/5.0);
    vec.emplace_back(timeToLive/5.0);

    temp = glm::vec4{center.x + 0.05, center.y - 0.05, center.z, 1.0f};
#ifdef BILLBOARD
    temp = glm::rotate(rotation, temp);
#endif    
    temp = projection * view * temp;

    vec.emplace_back(temp.x);
    vec.emplace_back(temp.y);
    vec.emplace_back(temp.z);

    vec.emplace_back(timeToLive/5.0);
    vec.emplace_back(timeToLive/5.0);
    vec.emplace_back(timeToLive/5.0);
}

std::tuple<GLfloat, GLfloat, GLfloat> getNewParticleParams(GLfloat inGameTime)
{
    GLfloat y = (rand() % 1000 - 500.f) / 2000.f;
    GLfloat x = (rand() % 1000 - 500.f) / 2000.f;
    GLfloat spawn = rand() % 500 / 100.f;
    //printf("Kreirao cesticu na %f %f sa spawnonom u %f\n", x, y, spawn);
    return std::make_tuple(x, y, spawn);
}

ParticleSystem::ParticleSystem(int numberOfParticles)
{

    {
        for (int i = 0; i < numberOfParticles; ++i)
        {
            std::tuple<GLfloat, GLfloat, GLfloat> params = getNewParticleParams(0);
            particles.emplace_back(glm::vec3{std::get<0>(params), std::get<1>(params), 0.0f}, std::get<2>(params));
        }
    }
    update(0, glm::vec3{0,0,1});
}

std::vector<GLfloat> ParticleSystem::getBuffer()
{
    //std::cerr << "[.] Retrieving buffer\n"; 
    return buffer;
}


void ParticleSystem::update(GLfloat inGameTime, glm::vec3 ociste)
{
    static GLfloat lastTime = inGameTime;

    buffer.clear();
    for (int i = 0; i < particles.size(); ++i)
    {
        particles[i].spawnedTime -= inGameTime - lastTime; 
        if (particles[i].spawnedTime <= 0)
        {
            
            std::tuple<GLfloat, GLfloat, GLfloat> params = getNewParticleParams(inGameTime);
            particles[i] = Particle{glm::vec3{std::get<0>(params), std::get<1>(params), 0.0f}, std::get<2>(params)};
        }
        else
        {
            //printf("O(%f,%f,%f)=>R(%f,%f,%f,%f)\n",
            //    ociste.x, ociste.y, ociste.z,
            //    rotation.x, rotation.y, rotation.z, rotation.w);
            // particles[i].position.z = particles[i].spawnedTime / 5.0;
            populateVec(buffer, particles[i].position, particles[i].spawnedTime, ociste);
        } 
    }
    lastTime = inGameTime;
}
