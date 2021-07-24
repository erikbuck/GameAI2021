using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Meanie : MonoBehaviour
{
    int destinationX;
    int destinationZ;

    private int exitIndex = 0;

    public int ExitIndex { get => exitIndex; set => exitIndex = value; }

    Tile FindNextTile(int x, int z, ref Dictionary<PlayArea.Position, Tile> stepsTiles)
    {
        int fewestStepsFromExit = 1000000;
        Tile result = null;
        PlayArea.Position[] candidatePositions = new PlayArea.Position[8] {
            new PlayArea.Position{x=x-1, z=z},
            new PlayArea.Position{x=x+1, z=z},
            new PlayArea.Position{x=x, z=z-1},
            new PlayArea.Position{x=x, z=z+1},
            new PlayArea.Position{x=x-1, z=z-1},
            new PlayArea.Position{x=x-1, z=z+1},
            new PlayArea.Position{x=x+1, z=z-1},
            new PlayArea.Position{x=x+1, z=z+1},
        };

        foreach (PlayArea.Position nextP in candidatePositions)
        {
            if (stepsTiles.ContainsKey(nextP))
            {
                // UnityEngine.Debug.Log("---:"+nextP.x.ToString()+", "+nextP.z.ToString()+
                //     " "+transform.position.x.ToString()+", "+transform.position.z.ToString());
                Tile t = stepsTiles[nextP];
                int steps = t.StepsFromExit[ExitIndex];
                if (0 <= steps && fewestStepsFromExit > steps)
                {
                    if (nextP.x != x && nextP.z != z)
                    {   // This is a diagonal move, so verify no cattycorner walls
                        PlayArea.Position otherPA = new PlayArea.Position{ x=nextP.x, z=z};
                        PlayArea.Position otherPB = new PlayArea.Position{ x=x, z=nextP.z};

                        if ((!stepsTiles.ContainsKey(otherPA) ||
                          0 > stepsTiles[otherPA].StepsFromExit[ExitIndex]) ||
                           (!stepsTiles.ContainsKey(otherPB) ||
                          0 > stepsTiles[otherPB].StepsFromExit[ExitIndex]))
                        {
                            // There is a cattycorner wall
                        }
                        else
                        {
                            fewestStepsFromExit = steps;
                            result = t;
                        }
                    }
                    else
                    {   // This is a horizontal or vertical move
                        fewestStepsFromExit = steps;
                        result = t;
                    }
                }
            }
        }

        return result;
    }

    public void PathAdvance(ref Dictionary<PlayArea.Position, Tile> stepsTiles)
    {
        //UnityEngine.Debug.Log("PathAdvance");
        int x = (int)(transform.position.x + 0.5f);
        int z = (int)(transform.position.z + 0.5f);
        Tile nextTile = FindNextTile(x, z, ref stepsTiles);

        if (null != nextTile)
        {
            destinationX = (int)(nextTile.transform.position.x + 0.5f);
            destinationZ = (int)(nextTile.transform.position.z + 0.5f);
            Vector3 direction = (new Vector3(
                destinationX, 0, destinationZ) - transform.position).normalized;
            transform.position = transform.position + (direction * 1.0f / 60.0f);
            //UnityEngine.Debug.Log(destinationX.ToString()+", "+destinationZ.ToString());
        }
        else
        {
            UnityEngine.Debug.Log("No next position!!!");
        }
    }

    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }
}
