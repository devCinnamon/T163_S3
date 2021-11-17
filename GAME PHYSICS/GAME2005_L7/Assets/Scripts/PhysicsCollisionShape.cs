using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum PhysicsColliderShape
{
    Sphere = 0,
    AABB,
    Plane
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