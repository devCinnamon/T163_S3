using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Physics_Object : MonoBehaviour
{

    public float mass = 1.0f;
    public  Vector3 velocity = Vector3.zero;
    public Physics_System physicsSystem;
    public float gravityScale = 1.0f;
    public bool lockPosition = false;
    public bool gravityLock = false;
    public Physics_ColliderBase shape = null;
    public float bounciness = 1.5f;
  
    private void Start()
    {
        //If an object has Physics_System, make it a physics object also.

        physicsSystem = FindObjectOfType<Physics_System>();
        physicsSystem.physicsObjects.Add(this);

        //If object is an physics_object, add its shape type.
        shape = GetComponent<Physics_ColliderBase>();
    }

 

}