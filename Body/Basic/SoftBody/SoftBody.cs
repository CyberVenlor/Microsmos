using Godot;
using System;

public partial class SoftBody : Body { //使用area2d作为零时的AABB箱
	//基础弹簧
	public struct Spring {
		public int p0;//point在Entity.Points中的索引
		public int p1;
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
		SimulateCollide((float)delta);
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
			spring.p0 = resSpring.P0;
			spring.p1 = resSpring.P1;
			spring.Length = Points[spring.p0].DistanceTo(Points[spring.p1]) * resSpring.LengthMultiplier;
			spring.Strength = resSpring.Strength;
			spring.Damping = resSpring.Damping;
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
			point.Velocity += new Vector2(0, 5);//////
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
		foreach (Body body in GetOverlapAABB()) {
			//桥接循环边
			Point[] points = new Point[body.Points.Length + 1];
			Array.Copy(body.Points, points, body.Points.Length);
			points[points.Length - 1] = points[0];
			//
			for (int t = 0; t < Points.Length; t++) {
				bool inPolygon = false;
				int maxB = 0;
				float maxD = 0;
				Vector2 storedDistance = Vector2.Zero;
				ref Point pt = ref Points[t];//本体上的顶点
				for (int b = 0; b < points.Length - 1; b++) {
					Vector2 p0 = points[b].Position;//碰撞体上的点
					Vector2 p1 = points[b + 1].Position;
					//
					if (IsIntersecting(pt.Position, p0, p1)) inPolygon = !inPolygon;
					Vector2 distance = DistanceToSegment(pt.Position, p0, p1);
					float Length = distance.Length();
					if (maxD < Length) {
						maxB = b;
						maxD = Length;
						storedDistance = distance;
					}
				}
				if (inPolygon) {
					pt.Position += storedDistance;
					pt.Velocity += storedDistance * delta / pt.Mass;
				}
			}
		}
	}
	Vector2 DistanceToSegment(Vector2 pt, Vector2 p0, Vector2 p1) {
        float t = Mathf.Max(0, Mathf.Min(1, (pt.X - p0.X) * (p1.X - p0.X) + (pt.Y - p0.Y) * (p1.Y - p0.Y) / p0.DistanceSquaredTo(p1)));
        Vector2 projection = new Vector2(p0.X + t * (p1.X - p0.X), p0.Y + t * (p1.Y - p0.Y));
        return pt - projection;
    }
	bool IsIntersecting(Vector2 pt, Vector2 p0, Vector2 p1) {
		if (p0.X < pt.X && p1.X < pt.X) {// 检查线段的两个端点是否在射线起点的右侧
			return false; // 线段的两个端点都在射线起点的左侧，不可能相交
		}
		// 计算线段与射线的交点
		float x0 = p0.X, y0 = p0.Y;
		float x1 = p1.X, y1 = p1.Y;
		float a = pt.Y;
		if (x0 == x1) { // 如果线段是垂直的，则交点在 x = x0 或 x = x1
			if (y0 <= a && y1 >= a || y0 >= a && y1 <= a) {// 射线的 Y 值与线段相交
				return x0 >= pt.X;
			}
			return false;
		}
		// 线段的参数化方程
		float t = (a - y0) / (y1 - y0);
		if (t < 0 || t > 1) {
			return false; // 交点不在线段上
		}

		float intersectX = x0 + t * (x1 - x0);
		return intersectX >= pt.X; // 交点是否在射线的范围内
	} 
}
