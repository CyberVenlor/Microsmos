using Godot;

//基础弹簧
[GlobalClass]
public partial class ResSpring : Resource {
    [Export] public int P1;
    [Export] public int P2;
    [Export] public float LengthMultiplier = 1f;
    [Export] public float Strength = 1f;
    [Export] public float Damping = 0.1f;
}