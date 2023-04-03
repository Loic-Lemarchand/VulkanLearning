#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_EXT_quaternion_common

namespace FWPhysics
{

class RigidBody
{
protected:
    float invMass;
    glm::mat3 invInertiaTensor;
    
    float linearDamping;
    float angularDamping;
    
    glm::vec3 position;
    glm::vec3 rotation;
    glm::quat orientation;

    glm::vec3 velocity;

    glm::mat3 inverseInertiaTensorWorld;

    float motion;
    bool isAwake;
    bool canSleep;

    glm::mat4 transformMat;

    glm::vec3 forceAccum;
    glm::vec3 torqueAccum;
    glm::vec3 acceleration;
    glm::vec3 lastFrameAcceleration;
public:

    void Integrate(float duration);

private:
    void CalculateDerivedData();

    //Mass
    void setMass(const float mass);
    float getMass() const;

    void setInvMass(const float invMass);
    float getInvMass() const;

    bool hasFiniteMass() const;

    //Inertia Tensor
    void setInertiaTensor(const glm::mat3 &inertiaTensor);
    void getInertiaTensor(glm::mat3 *inertiaTensor) const;
    glm::mat3 getInertiaTensor() const;
    void getInertiaTensorWorld(glm::mat3 *inertiaTensor) const;
    glm::mat3 getInertiaTensorWorld() const;

    //Inverse Inertia Tensor
    void setInvInertiaTensor(const glm::mat3 &inverseInertiaTensor);
    void getInverseInertiaTensor(glm::mat3 *inverseInertiaTensor);
    glm::mat3 getInverseInertiaTensor() const;
    void getInverseInertiaTensorWorld(glm::mat3 *inverseInertiaTensor);
    glm::mat3 getInverseInertiaTensorWorld() const;

    //Damping
    void setDamping(const float linearDamping, const float angularDamping);
    void setLinearDamping(const float linearDamping);
    float getLinearDamping() const;
    void setAngularDamping(const float angularDamping);
    float getAngularDamping() const;

    //Position
    void setPosition(const glm::vec3 &position);
    void getPosition(glm::vec3 *position) const;
    glm::vec3 getPosition() const;

    //Orientation
    void setOrientation(const glm::quat &orientation);


private:
    void getOrientation(glm::quat *orientation);
    glm::quat getOrientation() const;
    void getOrientation(glm::mat3 *matrix) const;

    //Transform
    void getTransform(glm::mat4 *transform) const;
    glm::mat4 getTransform() const;
    //
    glm::vec3 getPointInLocalSpace(const glm::vec3 &point) const;
    glm::vec3 getPointInWorldSpace(const glm::vec3 &point) const;
    //
    glm::vec3 getDirectionInLocalSpace(const glm::vec3 &direction) const;
    glm::vec3 getDirectionInWorldSpace(const glm::vec3 &direction) const;

    //Velocity
    void setVelocity(const glm::vec3 &velocity);
    void getVelocity(glm::vec3 *velocity) const;
    glm::vec3 getVelocity() const;
    void addVelocity(const glm::vec3 &deltaVelocity);

    //rotation
    void setRotation(const glm::vec3 &rotation);
    void getRotation(glm::vec3 *rotation) const;
    glm::vec3 getRotation() const;
    void addRotation(glm::vec3 &deltaRotation);

    //Awake
    bool getAwake() const
    {
        return isAwake;
    }

    void setAwake(const bool awake=true);

    //Can Sleep
    bool getCanSleep()
    {
        return canSleep;
    }

    void setCanSleep(const bool canSleep=true);
    void getLastFrameAcceleration(glm::vec3 *linearAcceleration) const;
    glm::vec3 getLastFrameAcceleration() const;

    void clearAccumulators();
    void addForce(const glm::vec3 &force);
    void addForceAtPoint(const glm::vec3 &force, const glm::vec3 &point);
    void addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point);

    void addTorque(const glm::vec3 &torque);
    void setAcceleration(const glm::vec3 &acceleration);

    void getAcceleration(glm::vec3 *acceleration) const;
    glm::vec3 getAcceleration() const;
};

        
}


