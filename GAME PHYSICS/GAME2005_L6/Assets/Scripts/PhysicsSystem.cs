using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PhysicsSystem : MonoBehaviour
{
    public Vector3 gravity = new Vector3(0, -9.81f, 0);

    public List<PhysicsObject> physicsObjects = new List<PhysicsObject>();

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
/*        for (int i = 0; i < physicsObjects.Count; i++)
        {
            physicsObjects[i].velocity += gravity * Time.deltaTime;
        }*/

        foreach (PhysicsObject obj in physicsObjects)
        {
            obj.velocity += gravity * Time.deltaTime * obj.gravityScale * obj.mass;
        }
    }
}
