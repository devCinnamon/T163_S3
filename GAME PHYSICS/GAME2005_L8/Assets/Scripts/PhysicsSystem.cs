using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PhysicsSystem : MonoBehaviour
{
    public Vector3 gravity = new Vector3(0, -9.80665f, 0);

    public List<PhysicsObject> physicsObjects = new List<PhysicsObject>();
    public List<PhysicsCollisionShape> physicsColliders = new List<PhysicsCollisionShape>();

    void Start()
    {
    }

    void FixedUpdate()
    {
        foreach (PhysicsObject obj in physicsObjects)
        {
            obj.transform.position = obj.transform.position + obj.velocity * Time.fixedDeltaTime;
        }
        // Gravity
        foreach (PhysicsObject obj in physicsObjects)
        {
            obj.velocity += obj.gravityScale * obj.mass * Time.fixedDeltaTime * gravity;
        }
        CollisionUpdate();
    }

    void CollisionUpdate()
    {
        for (int i = 0; i < physicsColliders.Count; i++)
        {
            for (int j = i + 1; j < physicsColliders.Count; j++)
            {
                if (i == j) continue;

                PhysicsCollisionShape objectA = physicsColliders[i];
                PhysicsCollisionShape objectB = physicsColliders[j];

                PhysicsColliderShape shapeA = objectA.GetColliderShape();
                PhysicsColliderShape shapeB = objectB.GetColliderShape();

                if (shapeA == PhysicsColliderShape.Sphere &&
                    shapeB == PhysicsColliderShape.Sphere)
                {
                    SSCollision((PhysicsCollisionShapeSphere)objectA, (PhysicsCollisionShapeSphere)objectB);
                }

                if (shapeA == PhysicsColliderShape.Plane &&
                    shapeB == PhysicsColliderShape.Sphere)
                {
                    PlaneSphereCollision((PhysicsCollisionShapePlane)objectA, (PhysicsCollisionShapeSphere)objectB);
                }

            }
        }
    }

    void SSCollision(PhysicsCollisionShapeSphere objA, PhysicsCollisionShapeSphere objB)
    {

        Vector3 displacementBetweeenSpheresAB = objA.transform.position - objB.transform.position;

        float distanceBetween = displacementBetweeenSpheresAB.magnitude;
        float sumOfRadius = objA.radius + objB.radius;
        float penetration = sumOfRadius - distanceBetween;

        bool isOverlapping = penetration > 0.0f;

        if ( isOverlapping )
        {
            Color colorA = objA.GetComponent<Renderer>().material.color;
            Color colorB = objB.GetComponent<Renderer>().material.color;

            objA.GetComponent<Renderer>().material.color = colorB;
            objB.GetComponent<Renderer>().material.color = colorA;
        }
        else { return; }

        Vector3 collisionNormalAB = displacementBetweeenSpheresAB / distanceBetween;

        Vector3 minABTranslationVector = collisionNormalAB * penetration;

        float moveScalar = 0.5f;

        Vector3 translationA = minABTranslationVector * moveScalar;
        Vector3 translationB = -minABTranslationVector * moveScalar;

        if (distanceBetween < 0.001f)
        {
            distanceBetween = 0.001f;
            collisionNormalAB = new Vector3(0, penetration, 0);
        }
        
        objA.transform.position += translationA;
        objB.transform.position += translationB;

        CollisionInfo collisionInfo;
        collisionInfo.colliderA = objA;
        collisionInfo.colliderB = objB;
        collisionInfo.collisionNormalAB = collisionNormalAB;
        collisionInfo.contactPoint = objA.transform.position + collisionNormalAB * objA.radius;

        VelResponse(collisionInfo);
    }

    void VelResponse(CollisionInfo collisionInfo)
    {
        throw new NotImplementedException();
    }

    void PlaneSphereCollision(PhysicsCollisionShapePlane plane, PhysicsCollisionShapeSphere sphere)
    {
        Vector3 fromPlaneToSphereCenter = sphere.transform.position - plane.transform.position;

        float dot = Vector3.Dot(fromPlaneToSphereCenter, plane.getNormal());
        float radius = sphere.radius;
        float distanceFromPlaneToSphereCenter = Mathf.Abs(dot);

        Vector3 penetration = new Vector3(0, distanceFromPlaneToSphereCenter - radius, 0);

        bool isOverlapping = distanceFromPlaneToSphereCenter <= sphere.radius;

        if ( isOverlapping )
        {
            sphere.transform.position -= penetration;
        }

    }
}
