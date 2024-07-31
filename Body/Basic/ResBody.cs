using Godot;

//用于Body读取Point等文件
[GlobalClass]
public partial class ResBody : Resource {
    [Export]public ResPoint[] Points;
}