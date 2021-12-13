using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Physics_System : MonoBehaviour
{


    public Vector3 gravity = new Vector3(0, -9.81f, 0);
    public float globalGravityScale = 1;
    public List<Physics_Object> physicsObjects = new List<Physics_Object>();
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void FixedUpdate()
    {
       
        //Velocity
        foreach(Physics_Object obj in physicsObjects)
        {
            if (!obj.lockPosition)
            {
                //If the object isn't colliding, accelerate it by gravity (Free-Fall)
                if (!obj.gravityLock)
                {
                    obj.velocity += (gravity * (obj.gravityScale * globalGravityScale * Time.deltaTime)) ;
                }
                
                    
            }
        }
        //Position
        foreach(Physics_Object obj in physicsObjects)
        {
            //If position isn't locked, transform position by the objects velocity.
            if (!obj.lockPosition)
            {
                
                obj.transform.position = obj.transform.position +  obj.velocity * Time.fixedDeltaTime;
            }
        }

       CollisionUpdate();
    }


     void getLocked(Physics_Object a, Physics_Object b, out float movementScalarA, out float movementScalarB)
    {
        
        movementScalarA = 0.5f;
        movementScalarB = 0.5f;

        //Cases for different lock positions.
        if (a.lockPosition && b.lockPosition)
        {
            movementScalarB = 0;
            movementScalarA = 0;
        }
        if (a.lockPosition && !b.lockPosition)
        {
            movementScalarB = 1.0f;
            movementScalarA = 0;
        }
        if (!a.lockPosition && b.lockPosition)
        {
            movementScalarB = 0;
            movementScalarA = 1.0f;
        }
        if (!a.lockPosition && !b.lockPosition)
        {
            movementScalarB = 0.5f;
            movementScalarA = 0.5f;
        }

    }

    void SphereSphereCollision(Physics_ColliderSphere a, Physics_ColliderSphere b,
        Physics_Object objectA, Physics_Object objectB)
    {
        //Distance between the sphere centers.
        Vector3 displacement = b.transform.position - a.transform.position;

        //Get the magnitude of the displacement between the spheres
        float distance = displacement.magnitude;

        //The total radius of both spheres.
        float sumRadii = a.radius + b.radius;

        //Get the difference between the radius of the two spheres and the distance between the origins.
        float penetrationDepth = sumRadii - distance;

        // If distance is not greater than the sum of the two spheres, we are colliding.
        bool isOverlapping = penetrationDepth > 0;

        //Collision Normal
        Vector3 collisionNormalAtoB;

        //Minimum distance that is required to trigger a collision response.
        float minDistance = 0.0001f;
        
        //If the magnitude of the distance is greater than 0.0001 set the collision normal to the y axis with the penetration depth.
        if (distance < minDistance)
        {
            collisionNormalAtoB = new Vector3(0, penetrationDepth, 0);
        }
        else
        {
            //Reset the collision normal
            collisionNormalAtoB = displacement / distance;
        }

        //Are the objects locked? Affects the movement scalars.
        getLocked(a.kinematicsObject, b.kinematicsObject, out float movementScalarA, out float movementScalarB);
    
        //Gets the collision normal and multiply by the depth of the penetration to get the direction the sphere will transform.
       Vector3 minimumTranslationVectorAtoB = penetrationDepth * collisionNormalAtoB;

       //Transform in the direction calculated by the movementScalars.
        Vector3 TranslationVectorA = -minimumTranslationVectorAtoB * movementScalarA;
        Vector3 TranslationVectorB = minimumTranslationVectorAtoB * movementScalarB;

        //transform objects
        a.transform.position += TranslationVectorA;
        b.transform.position += TranslationVectorB;

        // --- Collision response. ---

        // Bounce depenencies

        // Related velocity between to spheres
        Vector3 relativeVelocity = objectB.velocity - objectA.velocity;

        // Getting normal velocity 
        float relativeNormalVelocity = Vector3.Dot(relativeVelocity, collisionNormalAtoB);

        //Represents squeeze coefficient
        float restitution = (objectA.bounciness + objectB.bounciness ) * 0.5f;
        float changeVelocity = restitution * -relativeNormalVelocity;

        if ( relativeNormalVelocity >= 0.0f)
        {
            return;
        }


        if (objectA.lockPosition && !objectB.lockPosition)
        {
            float impulse = changeVelocity * objectB.mass;
            objectB.transform.position -= collisionNormalAtoB * (impulse / objectB.mass);
        }
        if (objectB.lockPosition && !objectA.lockPosition)
        {
            float impulse = changeVelocity * objectA.mass;
            objectA.transform.position -= collisionNormalAtoB * (impulse / objectA.mass);
        }
        if (!objectB.lockPosition && !objectA.lockPosition)
        {
            float impulse = changeVelocity * (objectA.mass + objectB.mass);

            objectA.velocity += collisionNormalAtoB * -(impulse / objectA.mass / objectB.mass);
            objectB.velocity += collisionNormalAtoB * (impulse / objectB.mass / objectA.mass);
        }

        if (isOverlapping)
        {
            Debug.Log(a.name + ("Collided with : ") + b.name);

            Color colorA = a.GetComponent<Renderer>().material.color;
            Color colorB = b.GetComponent<Renderer>().material.color;

            //a.GetComponent<Renderer>().material.color = Color.Lerp(colorA, colorB, 0.05f);
            //b.GetComponent<Renderer>().material.color = Color.Lerp(colorB, colorA, 0.05f);

            //a.transform.position += bounceVector * objectA.mass;
            //b.transform.position += -bounceVector * objectB.mass;
        }
        else
        {
            return;
        }
    }

    

    void SpherePlaneCollision(Physics_ColliderSphere a, Physics_ColliderPlane b)
    {
        Vector3 PointOnPlane = b.transform.position;

        Vector3 SphereCentre = a.transform.position;

        //Vector from point on the plane to the center of the sphere
        Vector3 displacement = SphereCentre - PointOnPlane;
        
        //Dot product of the plane-sphere vector and the normal of the plane.
        //If this dot is negative, the sphere is behind the plane. If it is positive, the sphere center is in front of the plane.
        float dot = Vector3.Dot(displacement, b.GetNormal());

        //Represents distance from plane to center of the sphere.
        float distance = Mathf.Abs(dot);

        //Get the depth of the penetration depending on the dot product of the normal(direction of the plane) and the displacement of the shapes inside each other.
        float penetrationDepth = distance - a.radius + 0.001f;

        //Get the penetration direction vector based upon the normal.
        Vector3 Penetration = b.GetNormal() * penetrationDepth;

        //If the distance from the plane to the center of the sphere is less than the radius of the sphere, they are overlapping.
        bool isOverlapping = distance <= a.radius;

       //If shapes are overlapping currently, do something. ( Collision response )
        if (isOverlapping)  
        {
            Debug.Log(a.name + ("Collided with : ") + b.name);

            Color colorA = a.GetComponent<Renderer>().material.color;
            Color colorB = b.GetComponent<Renderer>().material.color;

            //Collision Indication - changes colors
            //a.GetComponent<Renderer>().material.color = Color.Lerp(colorA, colorB, 0.05f);
            //b.GetComponent<Renderer>().material.color = Color.Lerp(colorB, colorA, 0.05f);

            a.kinematicsObject.velocity *= -0.5f;
            //a.kinematicsObject.gravityLock = false;
        }
        else
        {
            //Turn the gravity acceleration back on when the sphere is not colliding.
            a.kinematicsObject.gravityLock = false;
            return;
        }


        //---------------IF IS STILL OVERLAPPING--------------------------

        //get whether the objects are locked or not.
        getLocked(a.kinematicsObject, b.kinematicsObject, out float movementScalarA, out float movementScalarB);
    
        //The minimum translation neccessary to push the object back.
        Vector3 minimumTranslationVectorAtoB = Penetration;


        //Calculation of the translation vectors based on the movement scalars according to whether an object is locked or not.
        //If both objects are not locked, they both move a half space, if one is lock and the other is not, the unlocked object will move a full space to compensate.

        Vector3 TranslationVectorA = -minimumTranslationVectorAtoB * movementScalarA;
        Vector3 TranslationVectorB = minimumTranslationVectorAtoB * movementScalarB;


        //Transform the position of the objects based on the translation vectors.
        a.transform.position += TranslationVectorA;
        b.transform.position += TranslationVectorB;

    }

    void AABBCollision(Physics_ColliderAABB a, Physics_ColliderAABB b)
    {
        // GetHalf sizes along each axis (x, y, and z)
        // Get distance detween the boxes on each axis (x, y, and z)

        Vector3 halfSizeA = (((Physics_ColliderAABB)a).GetSize() / 2);
        Vector3 halfSizeB = (((Physics_ColliderAABB)b).GetSize() / 2);

        Vector3 displacementAtoB = b.transform.position - a.transform.position;

        float distX = Mathf.Abs(displacementAtoB.x);
        float distY = Mathf.Abs(displacementAtoB.y);
        float distZ = Mathf.Abs(displacementAtoB.z);

        float penetrationX = halfSizeA.x + halfSizeB.x - distX;
        float penetrationY = halfSizeA.y + halfSizeB.y - distY;
        float penetrationZ = halfSizeA.z + halfSizeB.z - distZ;

        if (penetrationX < 0 || penetrationY < 0 || penetrationZ < 0)
        {
            return;
        }

        Vector3 minimumTranslationVector;
        Vector3 collisionNormalAtoB;
        Vector3 contact;

        if (penetrationX < penetrationY && penetrationX < penetrationZ) // is penX the shortest?
        {
            collisionNormalAtoB = new Vector3(Mathf.Sign(displacementAtoB.x), 0, 0);    // Sign returns -1 or 1 based on sign
            minimumTranslationVector = collisionNormalAtoB * penetrationX;
        }
        else if (penetrationY < penetrationX && penetrationY < penetrationZ) // is penY the shortest?
        {
            collisionNormalAtoB = new Vector3(0, Mathf.Sign(displacementAtoB.y), 0);    // Sign returns -1 or 1 based on sign
            minimumTranslationVector = collisionNormalAtoB * penetrationY;
        }
        else //if (penetrationZ < penetrationY && penetrationZ < penetrationX) // is penZ the shortest?   // could just be else
        {
            collisionNormalAtoB = new Vector3(0, 0, Mathf.Sign(displacementAtoB.z));    // Sign returns -1 or 1 based on sign
            minimumTranslationVector = collisionNormalAtoB * penetrationZ;
        }

        contact = a.transform.position + minimumTranslationVector;

        ApplyMinimumTraslationVector(a.kinematicsObject, b.kinematicsObject, minimumTranslationVector, collisionNormalAtoB, contact);
    }

    static void ApplyMinimumTraslationVector(Physics_Object a, Physics_Object b, Vector3 minimumTranslationVectorAtoB, Vector3 collisionNormalAtoB, Vector3 contact)
    {
        ComputeMovementScalars(a, b, out float moveScalarA, out float moveScalarB);

        // calculate Translations
        Vector3 TranslationVectorA = -minimumTranslationVectorAtoB * moveScalarA;
        Vector3 TranslationVectorB = minimumTranslationVectorAtoB * moveScalarB;

        a.transform.position += TranslationVectorA;
        b.transform.position += TranslationVectorB;

        ApplyVelocityResponse(a, b, collisionNormalAtoB);

    }

    static void ComputeMovementScalars(Physics_Object a, Physics_Object b, out float mtvScalarA, out float mtvScalarB)
    {
        // Check to see if either object is Locked
        if (a.lockPosition && !b.lockPosition)
        {
            mtvScalarA = 0.0f;
            mtvScalarB = 1.0f;
            return;
        }
        if (!a.lockPosition && b.lockPosition)
        {
            mtvScalarA = 1.0f;
            mtvScalarB = 0.0f;
            return;
        }
        if (!a.lockPosition && !b.lockPosition)
        {
            mtvScalarA = 0.5f;
            mtvScalarB = 0.5f;
            return;
        }
        mtvScalarA = 0.0f;
        mtvScalarB = 0.0f;
    }

    static void ApplyVelocityResponse(Physics_Object objA, Physics_Object objB, Vector3 collisionNormal)
    {
        Vector3 normal = collisionNormal;

        // Velocity of B relative to A
        Vector3 relativeVelocityAB = objB.velocity - objA.velocity;
        // Find relative velocity
        float relativeNormalVelocityAB = Vector3.Dot(relativeVelocityAB, normal);

        // Early exit if they are not going towards each other (no bounce)
        if (relativeNormalVelocityAB >= 0.0f)
        {
            return;
        }

        // Choose a coefficient of restitution
        float restitution = (objA.bounciness + objB.bounciness) * 0.5f;

        float deltaV;

        float minimumRelativeVelocityForBounce = 3.0f;

        // If we only need the objects to slide and not bounce, then...
        if (relativeNormalVelocityAB < -minimumRelativeVelocityForBounce)
        {
            // Determine change in velocity 
            deltaV = (relativeNormalVelocityAB * (1.0f + restitution));
        }
        else
        {
            // no bounce
            deltaV = (relativeNormalVelocityAB);
        }

        float impulse;

        // respond differently based on locked states
        if (objA.lockPosition && !objB.lockPosition)
        {
            impulse = -deltaV * objB.mass;
            objB.velocity += normal * (impulse / (objB.mass));
        }
        else if (!objA.lockPosition && objB.lockPosition)
        {
            impulse = -deltaV * objA.mass;
            objA.velocity -= normal * (impulse / (objA.mass));
        }
        else if (!objA.lockPosition && !objB.lockPosition)
        {
            // Both
            impulse = deltaV / ((1.0f / objA.mass) + (1.0f / objB.mass));
            objA.velocity += normal * (impulse / objA.mass);
            objB.velocity -= normal * (impulse / objB.mass);
        }
        else if (!objA.lockPosition && !objB.lockPosition)
        {
        }
        else
            return;
        

        Vector3 relativeSurfaceVelocity = relativeVelocityAB - (relativeNormalVelocityAB * normal);

        ApplyFriction(objA, objB, relativeSurfaceVelocity, normal);
    }

    static void ApplyFriction(Physics_Object a, Physics_Object b, Vector3 relativeSurfaceVelocityAtoB, Vector3 normalAtoB)
    {

        float minFrictionSpeed = 0.0001f;
        float relativeSpeed = relativeSurfaceVelocityAtoB.magnitude;

        // Only apply friction if the relative velocity is significant
        if (relativeSurfaceVelocityAtoB.sqrMagnitude < minFrictionSpeed)
        {
            return;
        }

        // Choose a coefficient of friction
        float kFrictionCoefficient = (a.friction + b.friction) * 0.5f;

        // Find Normal
        Vector3 directionToApplyFriction = relativeSurfaceVelocityAtoB / relativeSpeed; // normalizing

        // Find force of gravity
        Vector3 gravity1 = new Vector3(0.0f, -9.81f, 0.0f);

        // Calculate Normal Force from gravity
        float gravityAccelerationAlongNormal = Vector3.Dot(gravity1, normalAtoB);    // * by mass to find force

        Vector3 frictionAcceleration = directionToApplyFriction * gravityAccelerationAlongNormal * kFrictionCoefficient;

        if (!a.lockPosition)
        {
            a.velocity -= frictionAcceleration * Time.fixedDeltaTime;
        }
        if (!b.lockPosition)
        {
            b.velocity += frictionAcceleration * Time.fixedDeltaTime;
        }
    }

    void CollisionUpdate()
    {
        //Go through all the physicsObjects
        for(int i = 0; i < physicsObjects.Count; i++)
        {
            //Go through all the physicsObjects, adjacent to the previous loop.
            for(int j = i + 1; j < physicsObjects.Count; j++)
            {
                Physics_Object objectA = physicsObjects[i];
                Physics_Object objectB = physicsObjects[j];


                
                //If null, skip over loop.
                if(objectA.shape == null && objectB.shape == null)
                {
                    continue;
                }


                //If both are spheres ... do sphere-sphere collision

                if(objectA.shape.GetCollisionShape() == CollisionShape.Sphere &&
                   objectB.shape.GetCollisionShape() == CollisionShape.Sphere)
                {
                    //if the distance between spheres is less than the sum of their radii, then they are overlapping.

                    SphereSphereCollision((Physics_ColliderSphere)objectA.shape, (Physics_ColliderSphere)objectB.shape,
                        objectA, objectB);
                }

                //If sphere and plane.. Do sphere-plane collision
                if (objectA.shape.GetCollisionShape() == CollisionShape.Sphere &&
                    objectB.shape.GetCollisionShape() == CollisionShape.Plane)
                {
                    SpherePlaneCollision((Physics_ColliderSphere)objectA.shape, (Physics_ColliderPlane)objectB.shape);
                }

                if (objectA.shape.GetCollisionShape() == CollisionShape.Plane &&
                    objectB.shape.GetCollisionShape() == CollisionShape.Sphere)
                {
                    SpherePlaneCollision((Physics_ColliderSphere)objectB.shape, (Physics_ColliderPlane)objectA.shape);
                }

                if (objectA.shape.GetCollisionShape() == CollisionShape.AABB &&
                   objectB.shape.GetCollisionShape() == CollisionShape.AABB)
                {
                    //if the distance between spheres is less than the sum of their radii, then they are overlapping.

                    AABBCollision((Physics_ColliderAABB)objectA.shape, (Physics_ColliderAABB)objectB.shape);
                }

            }
        }
    }
}


