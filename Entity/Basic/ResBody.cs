using Godot;

//用于Softbody读取Point位置等文件
[GlobalClass]
public partial class ResBody : Resource {
    [Export]public ResPoint[] Points;
}