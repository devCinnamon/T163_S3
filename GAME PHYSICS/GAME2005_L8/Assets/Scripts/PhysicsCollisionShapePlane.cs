using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum Axis
{
    X,
    Y,
    Z
}

public class PhysicsCollisionShapePlane : PhysicsCollisionShape
{
    public Axis align = Axis.Y;
    // Start is called before the first frame update
    public override PhysicsColliderShape GetColliderShape()
    {
        return PhysicsColliderShape.Plane;
    }

    public Vector3 getNormal()
    {

        switch ( align )
        {
            case (Axis.X):
            {
                return transform.right;
            }
            case (Axis.Y):
            {
                return transform.up;
            }
            case (Axis.Z):
            {
                return transform.forward;
            }
            default:
            {
                throw new System.Exception("Invalid plane alignment");
            }
        }
    }
}
