using Godot;

public abstract partial class Body : Area2D {
    //每个Body都具有一个包含点初始化数据的资源文件
	[Export] protected ResBody resBody;

    //基础质点
	public struct Point {
        public float Mass;
        public Vector2 Position;
        public Vector2 Velocity;

		public float DistanceTo(Point point) {
			return Position.DistanceTo(point.Position);
		}
    }

    //存储基础值
    public Point[] Points = [];

    //ready
    public override void _Ready() {
		//初始化点等数据
        InitFromRes();
    }

    public virtual void InitFromRes() {
		//init points
        if (resBody is ResBody) {
            Points = new Point[resBody.Points.Length];
            for (int i = 0; i < resBody.Points.Length; i++) {
                Points[i].Mass = resBody.Points[i].Mass;
                Points[i].Position = resBody.Points[i].Position;
            }
        }
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
}