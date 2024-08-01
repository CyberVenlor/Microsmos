using Godot;
using System;
using System.Collections.Generic;

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
			//DrawAABB();
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
			//CCD连续检测


			Vector2 position = point.Position + point.Velocity * delta;//质点现在的坐标
			List<Body> bodies = GetOverlapAABB();
			if (bodies.Count > 0) {
				foreach (Body body in bodies) {
					//桥接循环边
					Point[] points = new Point[body.Points.Length + 1];
					body.Points.CopyTo(points, 0);
					points[points.Length - 1] = points[0];
					for (int p = 0; p < points.Length - 1; p++) {
						Vector2 p0 = points[p].Position, p1 = points[p + 1].Position;
						if (FindIntersection(point.Position, position, p0, p1) is Vector2 intersection) {
							point.Velocity *= -1; //= point.Velocity.Reflect((p1 - p0).Normalized().Rotated(Mathf.Pi));
							GD.Print(point.Velocity);
							point.Position = intersection + (point.Position - intersection).Normalized();
							break;
						} else {
							point.Position = position;
						}
					}
				}
			} else {
				point.Position = position;
			}
			
			




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

	private Vector2? FindIntersection(Vector2 p0, Vector2 p1, Vector2 q0, Vector2 q1)//from chatgpt
    {
        Vector2 r = p1 - p0;
        Vector2 s = q1 - q0;
        float rxs = Cross(r, s);
        float qpxr = Cross(q0 - p0, r);
        if (rxs == 0 && qpxr == 0){
            return null;
        }
        if (rxs == 0 && qpxr != 0){
            return null;
        }
        float t = Cross(q0 - p0, s) / rxs;
        float u = Cross(q0 - p0, r) / rxs;
        if (rxs != 0 && 0 <= t && t <= 1 && 0 <= u && u <= 1){
            return p0 + t * r;
        }
        return null;
    }

    private static float Cross(Vector2 v1, Vector2 v2)
    {
        return v1.X * v2.Y - v1.Y * v2.X;
    }
}

