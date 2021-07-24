using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tile : MonoBehaviour
{
    private int[] stepsFromExit = new int[2];

    public int[] StepsFromExit { get => stepsFromExit; }

    public GameObject label;

    public void ResetSteps() {
        StepsFromExit[0] = -1;
        StepsFromExit[1] = -1;
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        TextMesh textMesh = label.GetComponent<TextMesh>();
        textMesh.text = stepsFromExit[1].ToString();
    }
}
