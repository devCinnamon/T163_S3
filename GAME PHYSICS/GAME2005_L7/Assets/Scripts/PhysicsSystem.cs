using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PhysicsSystem : MonoBehaviour
{
    public Vector3 gravity = new Vector3(0, -9.80665f, 0);

    public List<PhysicsObject> physicsObjects = new List<PhysicsObject>();
    public List<PhysicsCollisionShape> physicsColliders = new List<PhysicsCollisionShape>();

    void Update()
    {
        // Gravity
        foreach (PhysicsObject obj in physicsObjects)
        {
            obj.velocity += obj.gravityScale * obj.mass * Time.deltaTime * gravity;
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
            }
        }
    }

    void SSCollision(PhysicsCollisionShapeSphere objA, PhysicsCollisionShapeSphere objB)
    {
        Vector3 displacementBetweeenSpheres = objA.transform.position - objB.transform.position;
        float distanceBetween = displacementBetweeenSpheres.magnitude;
        float sumOfRadius = objA.radius + objB.radius;
        bool isOverlapping = distanceBetween < sumOfRadius;

        if (isOverlapping != false)
        {
            Debug.Log(objA.name + " collided with " + objB.name);
            objA.GetComponent<Renderer>().material.color = Color.red;
            objB.GetComponent<Renderer>().material.color = Color.green;
        }
    }
}
