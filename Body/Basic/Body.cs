using System.Collections.Generic;
using System.Linq;
using Godot;

public abstract partial class Body : Node2D {
    //每个Body都具有一个包含点初始化数据的资源文件
	[Export] protected ResBody resBody;

    //基础质点
	public struct Point {
        public float Mass;
        public Vector2 Position;//一律使用全局坐标，area2d必须坐标为(0,0)
        public Vector2 Velocity;

		public float DistanceTo(Point point) {
			return Position.DistanceTo(point.Position);
		}
    }

    //存储质点
    public Point[] Points = [];

    //AABB
    private Rect2 AABB;//以GlobalPosition方式存储
    static List<Body> bodies = [];

    //ready
    public override void _Ready() {
        InitFromRes();
        bodies.Add(this);//添加AABB
    }

    public virtual void InitFromRes() {
		//init points
        if (resBody is ResBody) {
            Points = new Point[resBody.Points.Length];
            for (int i = 0; i < resBody.Points.Length; i++) {
                Points[i].Mass = resBody.Points[i].Mass;
                Points[i].Position = GlobalTransform * resBody.Points[i].Position;
            }
        }
        GlobalTransform = new Transform2D(0, Vector2.Zero);
	}

    public void CalcAABB() { //TODO
        if (Points.Length > 0) AABB = new Rect2(Points[0].Position, Vector2.Zero);
        foreach (Point point in Points) {
            AABB = AABB.Expand(point.Position);
        }
    }

    public List<Body> GetOverlapAABB() { //TODO
        List<Body> bodies = [];
        foreach (Body body in Body.bodies) {
            if (body != this && body.AABB.Intersects(AABB))
                bodies.Add(body);
        }
        return bodies;
    }

    public void DrawPolygon() {
        //draw polyline
        Vector2[] points = new Vector2[Points.Length];
        for (int i = 0; i < Points.Length; i++) {
            points[i] = Points[i].Position;
        }
        DrawColoredPolygon(points, Colors.DimGray);
    }
    public void DrawPoint() {
        foreach (Point point in Points) {
            DrawCircle(point.Position, point.Mass, Colors.White);
        }
    }
    public void DrawAABB() {
        DrawColoredPolygon(
            [
                AABB.Position,new Vector2(AABB.End.X, AABB.Position.Y),
                AABB.End, new Vector2(AABB.Position.X, AABB.End.Y)
            ],
            GetOverlapAABB().Count > 0 ? Colors.DarkRed : Colors.Black
        );
    }
}