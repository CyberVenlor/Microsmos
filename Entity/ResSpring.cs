using Godot;

//基础弹簧
[GlobalClass]
public partial class ResSpring : Resource {
    [Export] public int P1;
    [Export] public int P2;
    [Export] public float Strength;
    [Export] public float Damping;
}