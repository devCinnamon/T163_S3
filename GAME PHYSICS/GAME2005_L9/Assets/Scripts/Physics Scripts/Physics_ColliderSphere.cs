using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Physics_ColliderSphere : Physics_ColliderBase
{
    // Start is called before the first frame update
    public float radius = 1.0f;


    public override CollisionShape GetCollisionShape()
    {
        return CollisionShape.Sphere;
    }
}