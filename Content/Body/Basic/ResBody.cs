using Godot;

//用于Body读取Point等文件
[GlobalClass]
public partial class ResBody : Resource {
    [Export] public virtual ResPoint[] resPoints {
        get => null;
        set {
            //init points
            Points = new Point[value.Length];
            for (int i = 0; i < value.Length; i++) {
                Points[i].Mass = value[i].Mass;
                Points[i].Position = value[i].Position;
            }
        }
    }
    [Export] public virtual ResEdge[] resEdges {
        get => null;
        set {
            //init edges
            Edges = new Edge[value.Length];
            for (int i = 0; i < value.Length; i++) {
                Edges[i].P0 = value[i].P0;
                Edges[i].P1 =value[i].P1;
                Edges[i].Elasticity = value[i].Elasticity;
            }
        }
    }

    public Point[] Points;
    public Edge[] Edges;
}