using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum CollisionShape
{
    //Type of shape

    Sphere = 0,
    Plane,
    AABB
}

[RequireComponent(typeof(Physics_Object))]


public abstract class Physics_ColliderBase : MonoBehaviour
{
    // Start is called before the first frame update
    public abstract CollisionShape GetCollisionShape();

    public Physics_Object kinematicsObject;


    public void Start()
    {
        //Set all objects with physics_colliderbase to kinematicsObject of type Physics_Object upon starting. Can use this to access Colliderbase.
        kinematicsObject = GetComponent<Physics_Object>();
    }

    //public abstract bool CollideWithSphere(Physics_ColliderSphere);
}
