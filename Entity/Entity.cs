using Godot;
using System;
using System.Reflection.Metadata;

public partial class Entity : Area2D { //使用area2d作为零时的AABB箱
	//每个Entity都具有一个包含点初始化数据的资源文件
	[Export] ResEntity resEntity;
	//基础质点
	public struct Point {
        public float Mass;
        public Vector2 Position;
        public Vector2 Velocity;

		public float DistanceTo(Point point) {
			return Position.DistanceTo(point.Position);
		}
    }
	//基础弹簧
	public struct Spring {
		public int P1;//point在Entity.Points中的索引
		public int P2;
		public float Length;
		public float Strength;
		public float Damping;
	}
	//存储基础值
	public Point[] Points = [];
	public Spring[] Springs = [];

    //ready and phy
    public override void _Ready() {
		//初始化点等数据
        if (resEntity != null) InitFromResEntity(resEntity);
		DrawEntity();
    }

    public override void _PhysicsProcess(double delta)
    {
        SimulateSpring((float)delta);
		SimulatePoint((float)delta);
		QueueRedraw();
    }

	//
    public void InitFromResEntity(ResEntity resEntity) {
		//init points
		Points = new Point[resEntity.Points.Length];
		for (int i = 0; i < resEntity.Points.Length; i++) {
			Points[i].Mass = resEntity.Points[i].Mass;
			Points[i].Position = resEntity.Points[i].Position;
		}
		//init springs
		Springs = new Spring[resEntity.Springs.Length];
		for (int i = 0; i < resEntity.Springs.Length; i++) {
			ref Spring spring = ref Springs[i];
			ResSpring resSpring = resEntity.Springs[i];
			spring.P1 = resSpring.P1;
			spring.P2 = resSpring.P2;
			spring.Length = Points[spring.P1].DistanceTo(Points[spring.P2]) * resSpring.LengthMultiplier;
			spring.Strength = resSpring.Strength;
			spring.Damping = resSpring.Damping;
		}
	}

	public void DrawEntity(bool drawPoint = true, bool drawSpring = true) {
		Draw += () => {
			//
			if (drawSpring) {
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
			//
			if (drawPoint) {
				foreach (Point point in Points) {
					DrawCircle(point.Position, point.Mass, Colors.White);
				}
			}
		};
	}

	private void SimulatePoint(float delta) {
		for (int i = 0; i < Points.Length; i++) {
			Points[i].Position += Points[i].Velocity * delta;
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
