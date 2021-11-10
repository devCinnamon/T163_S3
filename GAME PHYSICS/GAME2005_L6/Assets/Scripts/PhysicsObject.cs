using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PhysicsObject : MonoBehaviour
{
    public float mass = 1.0f;
    public float gravityScale = 1.0f;

    public Vector3 velocity = Vector3.zero;
    public PhysicsSystem physicsSystem;

    // Start is called before the first frame update
    void Start()
    {
        physicsSystem = FindObjectOfType<PhysicsSystem>();
        physicsSystem.physicsObjects.Add(this);
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = transform.position + velocity * Time.deltaTime;
    }
}