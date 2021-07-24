using System.Diagnostics;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Collections;

public class PlayArea : MonoBehaviour {

	public Transform floor;
	public Tile prefabTile;
	public Wall prefabWall;
	public Wall prefabEdgeWall;

	public Meanie prefabMeanie;
	public int widthTiles = 25;
	public int heightTiles = 15;

	public Fader imageBlocking;

	public struct Position {
		public int x;
		public int z;
	} 

	public Vector3[] entries = new Vector3[2];
	public Vector3[] exits = new Vector3[2];

	Dictionary<Position, Tile> tiles = new Dictionary<Position, Tile>();
	Dictionary<Position, Wall> walls = new Dictionary<Position, Wall>();

	List<Meanie> meanies = new List<Meanie>();

	void AddAdjacentTiles(ref List<Tile> l, int index, int x, int z, int distance) {
        Position[] candidatePositions = new Position[4] {
            new Position{x=x-1, z=z},
            new Position{x=x+1, z=z},
            new Position{x=x, z=z-1},
            new Position{x=x, z=z+1},
        };
		foreach(Position p in candidatePositions) {
			if(tiles.ContainsKey(p) && !walls.ContainsKey(p)) {
				Tile t = tiles[p];
				if(0 > t.StepsFromExit[index]) {
					t.StepsFromExit[index] = distance;
					l.Add(t);
				}
			}
		}
	}

	bool FindPath(int index) {
		Vector3 entry 				= entries[index];
		Vector3 exit 				= exits[index];
		List<Tile> tilesToConsider 	= new List<Tile>();
		bool foundEntry 			= false;
		Position p 					= new Position{x=(int)exit.x, z=(int)exit.z};

		if(!tiles.ContainsKey(p)) {
			UnityEngine.Debug.Log("Exit is not in play area");
		} else {
			Tile t 					= tiles[p];
			int distance 			= 0;
			t.StepsFromExit[index] 	= 0;
			tilesToConsider.Add(t);

			while(0 < tilesToConsider.Count) {
				t = tilesToConsider[0];
				tilesToConsider.RemoveAt(0);
				distance = t.StepsFromExit[index];
				
				int x = (int)t.transform.position.x;
				int z = (int)t.transform.position.z;
				if(x == (int)entry.x && z == (int)entry.z) {
					foundEntry = true;
					break;
				}
				AddAdjacentTiles(ref tilesToConsider, index, x, z, distance+1);
			}
		}
		return foundEntry;
	}

	void PathAdvance(ref Dictionary<Position, Tile> stepsTiles) {
		UnityEngine.Debug.Log("!!!PathAdvance");
	}
	
	void AddMeanies() {
		Meanie mA = Instantiate(prefabMeanie, entries[0], Quaternion.identity, transform);
		mA.ExitIndex = 0;
		meanies.Add(mA);
		Meanie mB = Instantiate(prefabMeanie, entries[1], Quaternion.identity, transform);
		mB.ExitIndex = 1;
		meanies.Add(mB);
	}

	IEnumerator AdvanceMeaniesGradual() 
	{
		while(true) {
			foreach(Meanie meanie in meanies) {
				meanie.PathAdvance(ref tiles);
			}
			yield return null;
		}
	}

	IEnumerator AddMeaniesGradual() 
	{
		//AddMeanies();
 		yield return new WaitForSeconds(.2f);
	}

	void TryToAddUserWall(Wall prefab, Position p) {
		Wall w = Instantiate(prefab, new Vector3(p.x, 0.0f, p.z), 
				Quaternion.identity, transform);
		walls.Add(p, w);
		//UnityEngine.Debug.Log("Add "+p.x.ToString()+", "+p.z.ToString());

		// Make sure added wall is not blocking
		foreach(KeyValuePair<Position, Tile> pair in tiles) {
			pair.Value.ResetSteps();
		}
		bool foundA = FindPath(0);
		bool foundB = FindPath(1);

		if(!foundA || !foundB) {
			UnityEngine.Debug.Log("Blocking");
			imageBlocking.Present();
			imageBlocking.Fade();
			Destroy(w.gameObject);
			walls.Remove(p);
			//UnityEngine.Debug.Log("Destroy "+p.x.ToString()+", "+p.z.ToString());
		}
	}

	void ProcessUserInputHit(Transform hitTransform) {
		int x = (int)(hitTransform.position.x + 0.5f);
		int z = (int)(hitTransform.position.z + 0.5f);
		Position p = new Position{x=x, z=z};

		if(walls.ContainsKey(p)) {
			Wall w = walls[p];
			if(w.isRemovable) {
				Destroy(w.gameObject);
				walls.Remove(p);
				// Initialize paths
				foreach(KeyValuePair<Position, Tile> pair in tiles) {
					pair.Value.ResetSteps();
				}
				FindPath(0);
				FindPath(1);
				UnityEngine.Debug.Log("Destroy "+p.x.ToString()+", "+p.z.ToString());
			}
		} else {
			if(x > 0 && x < (widthTiles-1) && z > 0 && z < (heightTiles-1)) {
				// Wall to add is not on outer edge
				TryToAddUserWall(prefabWall, p);
			}
		}
	}

	void InitPlayArea() {
		for(int i = 0; i < widthTiles; ++i) {
			for(int j = 0; j < heightTiles; ++j) {
                Tile f = Instantiate(prefabTile, new Vector3(i, 0, j), Quaternion.identity, floor);
				Position p = new Position{x=i, z=j};
				tiles.Add(p, f);

				int openingWidth = widthTiles / 5;
				int openingHeight = heightTiles / 5;
				if(i == 0 || i == (widthTiles - 1) || j == 0 || j == (heightTiles-1)) {
					if((i >= (widthTiles/2 + openingWidth) || i <= (widthTiles/2 - openingWidth)) &&
					   (j >= (heightTiles/2 + openingHeight) || j <= (heightTiles/2 - openingHeight))) {
                		Wall w = Instantiate(prefabEdgeWall, new Vector3(i, 0, j), Quaternion.identity, floor);
						walls.Add(p, w);
					}
				}
			}
		}
	}

	// Use this for initialization
	void Start () {
		InitPlayArea();

		// Initialize paths
		foreach(KeyValuePair<Position, Tile> pair in tiles) {
			pair.Value.ResetSteps();
		}
		FindPath(0);
		FindPath(1);

		StartCoroutine("AddMeaniesGradual");
		StartCoroutine("AdvanceMeaniesGradual");
	}
	
	// Update is called once per frame
	void Update () {		
		if (Input.GetMouseButtonDown(0)) {
			Camera cam = Camera.main;
			Ray ray = cam.ViewportPointToRay(cam.ScreenToViewportPoint(Input.mousePosition));
			RaycastHit hit;
			if (Physics.Raycast(ray, out hit, 1000)) {
				ProcessUserInputHit(hit.transform);
			}
		}
	}

    public void StartNextWave() {
		UnityEngine.Debug.Log("StartNextWave");
		AddMeanies();
    }
}
