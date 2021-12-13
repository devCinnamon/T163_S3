using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Physics_ColliderAABB : Physics_ColliderBase
{
    public override CollisionShape GetCollisionShape()
    {
        return CollisionShape.AABB;
    }

    public Vector3 GetSize()
    {
        return transform.localScale;
    }

    public Vector3 GetMin()
    {
        return transform.position - GetSize() / 2;
    }

    public Vector3 GetMax()
    {
        return transform.position + GetSize() / 2;
    }
}
