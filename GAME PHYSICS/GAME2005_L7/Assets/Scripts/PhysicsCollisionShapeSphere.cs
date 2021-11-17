using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PhysicsCollisionShapeSphere : PhysicsCollisionShape
{
    public float radius = 1.0f;

    public override PhysicsColliderShape GetColliderShape()
    {
        return PhysicsColliderShape.Sphere;
    }
}
