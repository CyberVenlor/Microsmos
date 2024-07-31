using Godot;

public partial class SoftBody : Body { //使用area2d作为零时的AABB箱
	//基础弹簧
	public struct Spring {
		public int P1;//point在Entity.Points中的索引
		public int P2;
		public float Length;
		public float Strength;
		public float Damping;
	}
	//存储基础值
	public Spring[] Springs = [];

    //ready and phy
    public override void _Ready() {
        base._Ready();
		Draw += () => {
			DrawAABB();
			DrawPolygon();
			DrawSpring();
			DrawPoint();
		};
    }

    public override void _PhysicsProcess(double delta)
    {
		base._PhysicsProcess(delta);
        SimulateSpring((float)delta);
		SimulatePoint((float)delta);
		CalcAABB();
		QueueRedraw();
    }

	//
    public override void InitFromRes() {
		base.InitFromRes();
		//init springs
		Springs = new Spring[((ResSoftBody)resBody).Springs.Length];
		for (int i = 0; i < ((ResSoftBody)resBody).Springs.Length; i++) {
			ref Spring spring = ref Springs[i];
			ResSpring resSpring = ((ResSoftBody)resBody).Springs[i];
			spring.P1 = resSpring.P1;
			spring.P2 = resSpring.P2;
			spring.Length = Points[spring.P1].DistanceTo(Points[spring.P2]) * resSpring.LengthMultiplier;
			spring.Strength = resSpring.Strength;
			spring.Damping = resSpring.Damping;
		}
	}

	public void DrawSpring() {
        foreach (Spring spring in Springs) {
            DrawLine(
                Points[spring.P1].Position,
                Points[spring.P2].Position,
                Colors.DarkBlue.Lerp (
                    Colors.OrangeRed, 
                    Mathf.Abs(Points[spring.P1].DistanceTo(Points[spring.P2])- spring.Length) / spring.Length * 2
                )
            );
        }		
	}

	private void SimulatePoint(float delta) {
		for (int i = 0; i < Points.Length; i++) {
			ref Point point = ref Points[i];
			point.Position += point.Velocity * delta;
			/////point.Velocity += new Vector2(0, 5);//////
		}
	}

	private void SimulateSpring(float delta) {
		foreach (Spring spring in Springs) {
			ref Point p1 = ref Points[spring.P1];
			ref Point p2 = ref Points[spring.P2];

			Vector2 force = (p2.Position - p1.Position).Normalized()
				* (p1.DistanceTo(p2) - spring.Length) 
				* spring.Strength
				+ (p2.Velocity - p1.Velocity) * spring.Damping;//阻尼

			p1.Velocity += force * delta / p1.Mass;
			p2.Velocity -= force * delta / p2.Mass;
		}
	}
}
