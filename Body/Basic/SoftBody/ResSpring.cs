using Godot;

//读取弹簧
[GlobalClass]
public partial class ResSpring : Resource {
    [Export] public int P0;
    [Export] public int P1;
    [Export] public float LengthMultiplier = 1f;
    [Export] public float Strength = 100f;
    [Export] public float Damping = 0.1f;
}