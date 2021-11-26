using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum PhysicsColliderShape
{
    Sphere = 0,
    AABB,
    Plane
}

struct CollisionInfo
{
    public PhysicsCollisionShape colliderA;
    public PhysicsCollisionShape colliderB;
    public Vector3 collisionNormalAB;
    public Vector3 contactPoint;
}

public abstract class PhysicsCollisionShape : MonoBehaviour
{
    public PhysicsColliderShape shape;
    public void Start()
    {
        FindObjectOfType<PhysicsSystem>().physicsColliders.Add(this);
    }

    public abstract PhysicsColliderShape GetColliderShape();
}