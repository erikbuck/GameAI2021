using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Fader : MonoBehaviour
{
    public float fadeIncrement = 0.05f;
   
	IEnumerator FadeGradual() 
	{
        UnityEngine.UI.Image image = GetComponent<UnityEngine.UI.Image>();
		for (float ft = 1f; ft >= 0; ft -= fadeIncrement) 
		{
			Color c = image.color;
			c.a = ft;
			image.color = c;
			yield return null;
		}
		image.enabled = false;
	}

	void Start () {
       	UnityEngine.UI.Image image = GetComponent<UnityEngine.UI.Image>();
		image.enabled = false;
	}

    public void Present() {
        UnityEngine.UI.Image image = GetComponent<UnityEngine.UI.Image>();
        Color c = image.color;
        c.a = 1.0f;
        image.color = c;
		image.enabled = true;
    }
    
    public void Fade() {
        StartCoroutine("FadeGradual");
    }
}
