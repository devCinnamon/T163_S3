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

            }
        }
    }
}


