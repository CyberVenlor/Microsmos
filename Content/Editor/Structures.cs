using Godot;

public struct Point {
	public Vector2 Position;
	public float Mass;
}

public struct Edge {
    public uint P0;
    public uint P1;
    public float Reflection;
}

public struct Spring {
    public uint P0;
    public uint P1;
    public float Strength;
    public float Damping;
}