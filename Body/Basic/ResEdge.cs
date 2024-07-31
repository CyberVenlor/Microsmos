using Godot;

//用于body读取碰撞边文件]
[GlobalClass]
public partial class ResEdge : Resource {
    [Export] public int P0;
    [Export] public int P1;
    [Export] public float Elasticity = 0.9f;
}