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
    }
	//基础弹簧
	public struct Spring {
		public int P1;//point在Entity.Points中的索引
		public int P2;
		public float Strength;
		public float Damping;
	}
	//存储基础值
	public Point[] Points = [];
	public Spring[] Springs = [];

    //ready
    public override void _Ready() {
		//初始化点等数据
        if (resEntity != null) InitFromResEntity(resEntity);
		DrawEntity();
    }

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
			Springs[i].P1 = resEntity.Springs[i].P1;
			Springs[i].P2 = resEntity.Springs[i].P2;
			Springs[i].Strength = resEntity.Springs[i].Strength;
			Springs[i].Damping = resEntity.Springs[i].Damping;
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
						Colors.White
					);
					GD.Print(1);
				}
			}
			//
			if (drawPoint) {
				foreach (Point point in Points) {
					DrawCircle(point.Position, point.Mass, Colors.Red);
				}
			}
		};
	}
}
