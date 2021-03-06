﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RandomizedRules : MonoBehaviour {

	public Material material;
	private Texture2D texture;

	private int width = 128;
	private int height = 128;

	private int[] ruleset = new int[] {0, 1, 0, 1, 1, 0, 1, 0};
	private int[] cells;
	private int generation;

	void Start () {
		cells = new int[width];

		texture = new Texture2D(width, height);
		texture.filterMode = FilterMode.Point;

		var pixels = new Color32[width * height];
		texture.SetPixels32(pixels);
		texture.Apply();

		restart();
		UpdateTexture();
	}

	void restart() {
		for (int i = 0; i < cells.Length; i++) {
			cells[i] = 0;
		}
		cells[cells.Length / 2] = 1;
		generation = 0;
	}

	void generate() {
		var nextgen = new int[cells.Length];
		for (int i = 1; i < cells.Length - 1; i++) {
			var left = cells[i-1];
			var me = cells[i];
			var right = cells[i+1];
			nextgen[i] = rules(left, me, right);
		}
		cells = nextgen;
		generation++;
	}

	int rules( int a, int b, int c) {
		if (a == 1 && b == 1 && c == 1) return ruleset[0];
		if (a == 1 && b == 1 && c == 0) return ruleset[1];
		if (a == 1 && b == 0 && c == 1) return ruleset[2];
		if (a == 1 && b == 0 && c == 0) return ruleset[3];
		if (a == 0 && b == 1 && c == 1) return ruleset[4];
		if (a == 0 && b == 1 && c == 0) return ruleset[5];
		if (a == 0 && b == 0 && c == 1) return ruleset[6];
		if (a == 0 && b == 0 && c == 0) return ruleset[7];
		return 0;
	}
	
	void UpdateTexture() {
		var pixels = texture.GetPixels32();
		for (int i = 0; i < width; i++) {
			var c = new Color32(0, 0, 0, 255);
			if (cells[i] == 1) {
				c = new Color32(255, 0, 0, 255);
			}
			pixels[generation * width + i] = c;
		}

		texture.SetPixels32(pixels);
		texture.Apply();

		if (material) {
			material.SetTexture("_MainTex", texture);			
		}
	}

	bool isFinished() {
		return generation > height / 2 - 3;
	}

	void Update () {
		if (!isFinished()) {
			generate();
			UpdateTexture();
		}
	}

	private void OnDestroy() {
		if (texture) {
			Object.Destroy(texture);
			texture = null;
		}
	}

}
