#include "../Include/RigidBody.h"



namespace FWPhysics
{
        static inline void _checkInverseInertiaTensor(const glm::mat3 &iitWorld)
    {
        // TODO: Perform a validity check in an assert.
    }

    /**
     * Internal function to do an intertia tensor transform by a quaternion.
     * Note that the implementation of this function was created by an
     * automated code-generator and optimizer.
     */
    static inline void _transformInertiaTensor(glm::mat3 &iitWorld,
                                               const glm::quat &q,
                                               const glm::mat3 &iitBody,
                                               const glm::mat4 &rotmat)
    {
        float t4 = rotmat[0][0]*iitBody[0][0]+
            rotmat[0][1]*iitBody[0][3]+
            rotmat[0][2]*iitBody[1][2];
        float t9 = rotmat[0][0]*iitBody[0][1]+
            rotmat[0][1]*iitBody[1][0]+
            rotmat[0][2]*iitBody[1][3];
        float t14 = rotmat[0][0]*iitBody[0][2]+
            rotmat[0][1]*iitBody[1][1]+
            rotmat[0][2]*iitBody[2][0];
        float t28 = rotmat[1][0]*iitBody[0][0]+
            rotmat[1][1]*iitBody[0][3]+
            rotmat[1][2]*iitBody[1][2];
        float t33 = rotmat[1][0]*iitBody[0][1]+
            rotmat[1][1]*iitBody[1][0]+
            rotmat[1][2]*iitBody[1][3];
        float t38 = rotmat[1][0]*iitBody[0][2]+
            rotmat[1][1]*iitBody[1][1]+
            rotmat[1][2]*iitBody[2][0];
        float t52 = rotmat[2][0]*iitBody[0][0]+
            rotmat[2][1]*iitBody[0][3]+
            rotmat[2][2]*iitBody[1][2];
        float t57 = rotmat[2][0]*iitBody[0][1]+
            rotmat[2][1]*iitBody[1][0]+
            rotmat[2][2]*iitBody[1][3];
        float t62 = rotmat[2][0]*iitBody[0][2]+
            rotmat[2][1]*iitBody[1][1]+
            rotmat[2][2]*iitBody[2][0];

        iitWorld[0][0] = t4*rotmat[0][0]+
            t9*rotmat[0][1]+
            t14*rotmat[0][2];
        iitWorld[0][1] = t4*rotmat[1][0]+
            t9*rotmat[1][1]+
            t14*rotmat[1][2];
        iitWorld[0][2] = t4*rotmat[2][0]+
            t9*rotmat[2][1]+
            t14*rotmat[2][2];
        iitWorld[0][3] = t28*rotmat[0][0]+
            t33*rotmat[0][1]+
            t38*rotmat[0][2];
        iitWorld[1][0] = t28*rotmat[1][0]+
            t33*rotmat[1][1]+
            t38*rotmat[1][2];
        iitWorld[1][1] = t28*rotmat[2][0]+
            t33*rotmat[2][1]+
            t38*rotmat[2][2];
        iitWorld[1][2] = t52*rotmat[0][0]+
            t57*rotmat[0][1]+
            t62*rotmat[0][2];
        iitWorld[1][3] = t52*rotmat[1][0]+
            t57*rotmat[1][1]+
            t62*rotmat[1][2];
        iitWorld[2][0] = t52*rotmat[2][0]+
            t57*rotmat[2][1]+
            t62*rotmat[2][2];
    }

    /**
     * Inline function that creates a transform matrix from a
     * position and orientation.
     */
    static inline void _calculateTransformMatrix(glm::mat4 &transformMatrix,
                                             const glm::vec3 &position,
                                             const glm::quat &orientation)
        {
            transformMatrix[0][0] = 1-2*orientation.y*orientation.y-
                2*orientation.z*orientation.z;
            transformMatrix[1][0] = 2*orientation.x*orientation.y -
                2*orientation.w*orientation.z;
            transformMatrix[2][0] = 2*orientation.x*orientation.z +
                2*orientation.w*orientation.y;
            transformMatrix[3][0] = position.x;

            transformMatrix[0][1] = 2*orientation.x*orientation.y +
                2*orientation.w*orientation.z;
            transformMatrix[1][1] = 1-2*orientation.x*orientation.x-
                2*orientation.z*orientation.z;
            transformMatrix[2][1] = 2*orientation.y*orientation.z -
                2*orientation.w*orientation.x;
            transformMatrix[3][1] = position.y;

            transformMatrix[0][2] = 2*orientation.x*orientation.z -
                2*orientation.w*orientation.y;
            transformMatrix[1][2] = 2*orientation.y*orientation.z +
                2*orientation.w*orientation.x;
            transformMatrix[2][2] = 1-2*orientation.x*orientation.x-
                2*orientation.y*orientation.y;
            transformMatrix[3][2] = position.z;
        }
        
    void RigidBody::Integrate(float duration)
    {
        if(!isAwake) return;

            lastFrameAcceleration = acceleration;
            //lastFrameAcceleration.add
            //glm::vec3 angularAcceleration = inverseInertiaTensorWorld.
    }

    void RigidBody::CalculateDerivedData()
    {
        //glm::normalize(orientation);

        _calculateTransformMatrix(
            transformMat,
            position,
            orientation);

        _transformInertiaTensor(
            inverseInertiaTensorWorld,
            orientation,
            invInertiaTensor,
            transformMat);
    }

    void RigidBody::setMass(const float mass)
    {
        assert(mass != 0 && "Mass can't be null !");
            RigidBody::invMass = ((float)1.0)/mass;

    }

    float RigidBody::getMass() const
    {
            if(invMass == 0)
            {
                return 10000;// define a max
            } else
            {
                return ((float)1.0)/invMass;
            }
    }

    void RigidBody::setInvMass(const float invMass)
    {
        RigidBody::invMass = invMass;
    }

    float RigidBody::getInvMass() const
    {
        return invMass;
    }

    bool RigidBody::hasFiniteMass() const
    {
            return invMass >= 0.0f;
    }

    void RigidBody::setInertiaTensor(const glm::mat3& inertiaTensor)
    {
            invInertiaTensor = inverse(inertiaTensor);
            _checkInverseInertiaTensor(invInertiaTensor);
    }

    void RigidBody::getInertiaTensor(glm::mat3* inertiaTensor) const
    {
            *inertiaTensor = inverse(invInertiaTensor);//////////////////pif
    }

    glm::mat3 RigidBody::getInertiaTensor() const
    {
            glm::mat3 it;
            getInertiaTensor(&it);
            return it;
    }

    void RigidBody::getInertiaTensorWorld(glm::mat3* inertiaTensor) const
    {
            *inertiaTensor = inverse(inverseInertiaTensorWorld);
    }

    glm::mat3 RigidBody::getInertiaTensorWorld() const
    {
            return inverse(inverseInertiaTensorWorld);
    }

    void RigidBody::setInvInertiaTensor(const glm::mat3& inverseInertiaTensor)
    {
            invInertiaTensor = inverseInertiaTensor;
    }

    void RigidBody::getInverseInertiaTensor(glm::mat3* inverseInertiaTensor)
    {
            *inverseInertiaTensor = inverse(invInertiaTensor);
    }

    glm::mat3 RigidBody::getInverseInertiaTensor() const
    {
            return invInertiaTensor;
    }

    void RigidBody::getInverseInertiaTensorWorld(glm::mat3* inverseInertiaTensor)
    {
            *inverseInertiaTensor = inverseInertiaTensorWorld;
    }

    glm::mat3 RigidBody::getInverseInertiaTensorWorld() const
    {
            return inverseInertiaTensorWorld;
    }

    void RigidBody::setDamping(const float linearDamping, const float angularDamping)
    {
        RigidBody::linearDamping = linearDamping;
        RigidBody::angularDamping = angularDamping;
    }

    void RigidBody::setLinearDamping(const float linearDamping)
    {
        RigidBody::linearDamping = linearDamping;
    }

    float RigidBody::getLinearDamping() const
    {
            return linearDamping;
    }

    void RigidBody::setAngularDamping(const float angularDamping)
    {
        RigidBody::angularDamping = angularDamping;
    }

    float RigidBody::getAngularDamping() const
    {
            return angularDamping;
    }

    void RigidBody::setPosition(const glm::vec3& position)
    {
            RigidBody::position = position;
    }

    void RigidBody::getPosition(glm::vec3* position) const
    {
            *position = RigidBody::position;
    }

    glm::vec3 RigidBody::getPosition() const
    {
            return position;
    }

    void RigidBody::setOrientation(const glm::quat& orientation)
    {
        RigidBody::orientation = orientation;
       // RigidBody::orientation.normalise();
    }

    void RigidBody::getOrientation(glm::quat* orientation)
    {
            *orientation = RigidBody::orientation;
    }

    glm::quat RigidBody::getOrientation() const
    {
            return orientation;
    }

    void RigidBody::getOrientation(glm::mat3* matrix) const
    {
            *matrix = orientation;
    }

    void RigidBody::getTransform(glm::mat4* transform) const
    {
            *transform = transformMat;
    }

    glm::mat4 RigidBody::getTransform() const
    {
            return transformMat;
    }

    glm::vec3 RigidBody::getPointInLocalSpace(const glm::vec3& point) const
    {
            //return transformMat.inv;
    }

    glm::vec3 RigidBody::getPointInWorldSpace(const glm::vec3& point) const
    {
    }

    glm::vec3 RigidBody::getDirectionInLocalSpace(const glm::vec3& direction) const
    {
    }

    glm::vec3 RigidBody::getDirectionInWorldSpace(const glm::vec3& direction) const
    {
    }

    void RigidBody::setVelocity(const glm::vec3& velocity)
    {
            RigidBody::velocity = velocity;
    }

    void RigidBody::getVelocity(glm::vec3* velocity) const
    {
            *velocity = RigidBody::velocity;
    }

    glm::vec3 RigidBody::getVelocity() const
    {
            return velocity;
    }

    void RigidBody::addVelocity(const glm::vec3& deltaVelocity)
    {
            velocity += deltaVelocity;
    }

    void RigidBody::setRotation(const glm::vec3& rotation)
    {
            RigidBody::rotation = rotation;
    }

    void RigidBody::getRotation(glm::vec3* rotation) const
    {
            *rotation = RigidBody::rotation;
    }

    glm::vec3 RigidBody::getRotation() const
    {
            return rotation;
    }

    void RigidBody::addRotation(glm::vec3& deltaRotation)
    {
            rotation += deltaRotation;
    }

    void RigidBody::setAwake(const bool awake)
    {
            if (awake) {
                isAwake= true;

                // Add a bit of motion to avoid it falling asleep immediately.
                //motion = sleepEpsilon*2.0f;
            } else {
                isAwake = false;
                /*velocity.clear();
                rotation.clear();*/
            }
    }

    void RigidBody::setCanSleep(const bool canSleep)
    {
            RigidBody::canSleep = canSleep;

            if (!canSleep && !isAwake) setAwake();
    }

    void RigidBody::getLastFrameAcceleration(glm::vec3 *acceleration) const
    {
            *acceleration = lastFrameAcceleration;
    }

    glm::vec3 RigidBody::getLastFrameAcceleration() const
    {
            return lastFrameAcceleration;
    }

    void RigidBody::clearAccumulators()
    {
            /*forceAccum.clear();
            torqueAccum.clear();*/
    }

    void RigidBody::addForce(const glm::vec3& force)
    {
            forceAccum += force;
            isAwake = true;
    }

    void RigidBody::addForceAtPoint(const glm::vec3& force, const glm::vec3 &point)
    {
            glm::vec3 pt = point;
            pt -= position;

            forceAccum += force;
            torqueAccum += glm::cross(pt, force);

            isAwake = true;    
    }

    void RigidBody::addForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point)
    {
            glm::vec3 pt = getPointInWorldSpace(point);
            addForceAtPoint(force, pt);
    }

    void RigidBody::addTorque(const glm::vec3& torque)
    {
            torqueAccum += torque;
            isAwake = true;
    }

    void RigidBody::setAcceleration(const glm::vec3& acceleration)
    {
            RigidBody::acceleration = acceleration;
    }

    void RigidBody::getAcceleration(glm::vec3* acceleration) const
    {
            *acceleration = RigidBody::acceleration;
    }

    glm::vec3 RigidBody::getAcceleration() const
    {
            return acceleration;
    }
}
