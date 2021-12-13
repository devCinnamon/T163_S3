using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public enum Axis
{
    X = 0,
    Y,
    Z
}
public class Physics_ColliderPlane : Physics_ColliderBase
{


    public Axis alignment = Axis.Y;
    // Start is called before the first frame update


    public override CollisionShape GetCollisionShape()
    {
        return CollisionShape.Plane;
    }


    public Vector3 GetNormal()
    {
        //Get which alignment is set to be the collision alignment.
        switch (alignment)
        {
            case(Axis.Y):
            {
                return transform.up;
            }
            case (Axis.X):
            {
                return transform.right;
            }
            case (Axis.Z):
            {
                return transform.forward;
            }
            default:
            {
                throw new SystemException("Invalid alignment");
            }

        }
       
    }
}