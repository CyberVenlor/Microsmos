using Godot;

//用于Softbody读取Point，spring等文件
[GlobalClass]
public partial class ResSoftBody : ResBody {
    [Export] ResSpring[] resSprings;
    public override ResPoint[] resPoints { //因为时序问题，子资源的属性会被先赋值，也可以使用异步代替
        get => base.resPoints;
        set {
            base.resPoints = value;
            //init springs
		    Springs = new Spring[resSprings.Length];
		    for (int i = 0; i < resSprings.Length; i++) {
                ref Spring spring = ref Springs[i];
                ResSpring resSpring = resSprings[i];
                spring.p0 = resSpring.P0;
                spring.p1 = resSpring.P1;
                spring.Length = Points[spring.p0].DistanceTo(Points[spring.p1]) * resSpring.LengthMultiplier;
                spring.Strength = resSpring.Strength;
                spring.Damping = resSpring.Damping;
            }
        }
    }

    public Spring[] Springs;
}