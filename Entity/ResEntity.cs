using Godot;

//用于Entity读取Point位置等文件
[GlobalClass]
public partial class ResEntity : Resource {
    [Export]public ResPoint[] Points;
    [Export]public ResSpring[] Springs;
}