using Godot;
using System;

//基础弹簧
	public struct Spring {
		public int p0;//point在Entity.Points中的索引
		public int p1;
		public float Length;
		public float Strength;
		public float Damping;
	}
public partial class SoftBody : Body { //使用area2d作为零时的AABB箱
	//存储基础值
	public Spring[] Springs;

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
		SimulateCollide((float)delta);
		QueueRedraw();
    }

	//
    public override void InitFromRes() {
		base.InitFromRes();
		if (resBody is ResSoftBody) {
			Springs = new Spring[((ResSoftBody)resBody).Springs.Length];
			((ResSoftBody)resBody).Springs.CopyTo(Springs, 0);
		}
	}

	public void DrawSpring() {
        foreach (Spring spring in Springs) {
            DrawLine(
                Points[spring.p0].Position,
                Points[spring.p1].Position,
                Colors.DarkBlue.Lerp (
                    Colors.OrangeRed, 
                    Mathf.Abs(Points[spring.p0].DistanceTo(Points[spring.p1])- spring.Length) / spring.Length * 2
                )
            );
        }		
	}

	private void SimulatePoint(float delta) {
		for (int i = 0; i < Points.Length; i++) {
			ref Point point = ref Points[i];
			point.Position += point.Velocity * delta;
			//tmp
			point.Velocity += new Vector2(0, 60) * delta;
			if (point.Position.Y>170){
				point.Velocity *= new Vector2(0, -0.5f);
				point.Position.Y=170;
			}
		}
	}

	private void SimulateSpring(float delta) {
		foreach (Spring spring in Springs) {
			ref Point p0 = ref Points[spring.p0];
			ref Point p1 = ref Points[spring.p1];

			Vector2 force = (p1.Position - p0.Position).Normalized()
				* (p0.DistanceTo(p1) - spring.Length) 
				* spring.Strength
				+ (p1.Velocity - p0.Velocity) * spring.Damping;//阻尼

			p0.Velocity += force * delta / p0.Mass;
			p1.Velocity -= force * delta / p1.Mass;
		}
	}

	private void SimulateCollide(float delta) {
	}
}
