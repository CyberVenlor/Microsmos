using Godot;

//基础质点
[GlobalClass]
public partial class ResPoint : Resource {
    [Export] public float Mass;
    [Export] public Vector2 Position;
}