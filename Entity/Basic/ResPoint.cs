using Godot;

//基础质点
[GlobalClass]
public partial class ResPoint : Resource {
    [Export] public float Mass = 1f;
    [Export] public Vector2 Position;
}