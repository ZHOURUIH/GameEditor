using System;
using UnityEngine;

public class MathUtility
{
	public static int getMin(int a, int b) { return a < b ? a : b; }
	public static float getMin(float a, float b) { return a < b ? a : b; }
	public static uint getMin(uint a, uint b) { return a < b ? a : b; }
	public static long getMin(long a, long b) { return a < b ? a : b; }
	public static int getMax(int a, int b) { return a > b ? a : b; }
	public static float getMax(float a, float b) { return a > b ? a : b; }
	public static long getMax(long a, long b) { return a > b ? a : b; }
	public static uint getMax(uint a, uint b) { return a > b ? a : b; }
}