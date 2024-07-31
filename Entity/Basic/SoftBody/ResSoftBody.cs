using Godot;

//用于Softbody读取Point位置等文件
[GlobalClass]
public partial class ResSoftBody : ResBody {
    [Export]public ResSpring[] Springs;
}