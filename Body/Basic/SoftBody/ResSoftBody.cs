using Godot;

//用于Softbody读取Point，spring等文件
[GlobalClass]
public partial class ResSoftBody : ResBody {
    [Export]public ResSpring[] Springs;
}