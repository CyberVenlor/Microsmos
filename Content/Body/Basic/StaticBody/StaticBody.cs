using Godot;

public partial class StaticBody : Body {
    //ready
    public override void _Ready() {
        base._Ready();
        Draw += () => {
			DrawPolygon();
			DrawPoint();
		};
        CalcAABB();
    }

    public override void _PhysicsProcess(double delta) {
        base._PhysicsProcess(delta);
        QueueRedraw();
    }
}