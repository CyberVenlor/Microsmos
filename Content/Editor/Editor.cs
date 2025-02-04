using Godot;
using System;
using System.Buffers;
using System.Collections.Generic;

public partial class Editor : Control
{
	public const string NodePath = "/root/Editor";
	//BodyData
	Pool<Point> points = new();
	Pool<Edge> edges = new();
	Pool<Spring> springs = new();

	List<int> selected {
		get => _selected;
		set {
			SelectionChanged?.Invoke();
			_selected = value;
		}
	}
	List<int> _selected = new();

	public Point? SelectedPoint {
		get {
			if (selected.Count >= 1) return points[selected[0]];
			else return null;
		}
		set {
			if (value != null && selected.Count >= 1){
				points[selected[0]] = (Point)value;
			}
		}
	}
	public Edge? SelectedEdge {
		get {
			if (FindEdge() is int index) return edges[index];
			else return null;
		}
		set {
			if (value != null) if (FindEdge() is int index) edges[index] = (Edge)value;
		}
	}
	public Spring? SelectedSpring {
		get {
			if (FindSpring() is int index) return springs[index];
			else return null;
		}
		set {
			if (value != null) if (FindSpring() is int index) springs[index] = (Spring)value;
		}
	}

	//file
	enum FileMenu {
		New,
		Load,
		Save
	}
	enum FileDialogType{
		Load,
		Save
	}
	FileDialogType fileDialogType;

	//control
	Vector2 mousePositionDiff;
	public event Action SelectionChanged;

	//Children
	Node menuBar;
	FileDialog fileDialog;
	Node2D displayer;
	HSlider scaler;
	CanvasItem point;
	CanvasItem edge;
	CanvasItem spring;

	public override void _Ready(){
		menuBar = GetNode("MenuBar");
		fileDialog = GetNode<FileDialog>("FileDialog");
		displayer = GetNode<Node2D>("Sprite2D");
		scaler = GetNode<HSlider>("Scaler");
		point = GetNode<CanvasItem>("MainPanel/Content/Point");
		edge = GetNode<CanvasItem>("MainPanel/Content/Edge");
		spring = GetNode<CanvasItem>("MainPanel/Content/Spring");
		InitMenu();
		InitFileDialog();
		scaler.ValueChanged += (double value) => displayer.Scale = Vector2.One * (float)value;
		displayer.Draw += DrawBody;
	}

    public override void _Process(double delta){
		if (Input.IsMouseButtonPressed(MouseButton.Middle)){
			displayer.GlobalPosition = GetGlobalMousePosition() - mousePositionDiff;
		}
        displayer.QueueRedraw();
    }

    public override void _UnhandledInput(InputEvent @event){
        if (@event is InputEventMouseButton inputEventMouseButton && @event.IsPressed()){
			switch (inputEventMouseButton.ButtonIndex){
				case MouseButton.Right:{
					points.Add(new Point(){ Position = displayer.GetLocalMousePosition(), Mass = 1});
					break;
				}
				case MouseButton.Left:{
					if (!Input.IsKeyPressed(Key.Shift)) selected = new List<int>();
					for (int i= 0; i < points.Count; i++){
						if (points.IsActive(i) && displayer.GetLocalMousePosition().DistanceTo(points[i].Position) < 3){
							if (!selected.Remove(i)) selected.Add(i);
							break;
						}
					}
					break;
				}
				case MouseButton.WheelUp:{
					scaler.Value += 0.2;
					break;
				}
				case MouseButton.WheelDown:{
					scaler.Value -= 0.2;
					break;
				}
				case MouseButton.Middle:{
					mousePositionDiff = GetGlobalMousePosition() - displayer.GlobalPosition;
					break;
				}
			}
		}
    }

    public void PointRemove(){
		if (selected.Count >= 1){
			points.Remove(selected[0]);
			selected.RemoveAt(0);
		}
	}

	public void EdgeAssign(){
		if (FindEdge() == null) edges.Add(new Edge(){P0 = (uint)selected[0], P1 = (uint)selected[1], Reflection = 1});
	}

	public void EdgeDelete(){
		if (FindEdge() is int index) edges.Remove(index);
	}

	public void SpringAssign(){
		if (FindSpring() == null) springs.Add(new Spring(){P0 = (uint)selected[0], P1 = (uint)selected[1], Strength = 1, Damping = 0.1f});
	}

	public void SpringDelete(){
		if (FindSpring() is int index) springs.Remove(index);
	}

	void DrawBody(){
		const float  pointSize = 2f;
		foreach (Point point in points) displayer.DrawCircle(point.Position, pointSize, Colors.Black);
		foreach (int index in selected) displayer.DrawCircle(points[index].Position, pointSize, Colors.Orange);
		if (selected.Count >= 1) displayer.DrawCircle(points[selected[0]].Position, pointSize, Colors.OrangeRed);
		if (point.Visible){
			//
		}else if (edge.Visible){
			if (selected.Count >= 2) displayer.DrawCircle(points[selected[1]].Position, pointSize, Colors.OrangeRed);
			foreach (Edge edge in edges){
				displayer.DrawLine(points[(int)edge.P0].Position, points[(int)edge.P1].Position, Colors.Green);
			}
		} else if (spring.Visible){
			if (selected.Count >= 2) displayer.DrawCircle(points[selected[1]].Position, pointSize, Colors.OrangeRed);
			foreach (Spring spring in springs){
				displayer.DrawLine(points[(int)spring.P0].Position, points[(int)spring.P1].Position, Colors.Blue);
			}
		}
	}

    void InitMenu(){
		//file menu
		PopupMenu file = menuBar.GetChild<PopupMenu>(0);
		file.IdPressed += (id) => {
			switch ((FileMenu)id) {
				case FileMenu.New:{
					GD.Print("new file created");
					points = new Pool<Point>();
					edges = new Pool<Edge>();
					springs = new Pool<Spring>();
					break;
				}
				case FileMenu.Load:{
					fileDialog.Visible = true;
					fileDialog.FileMode = FileDialog.FileModeEnum.OpenFile;
					fileDialogType = FileDialogType.Load;
					break;
				}
				case FileMenu.Save:{
					fileDialog.Visible = true;
					fileDialog.FileMode = FileDialog.FileModeEnum.SaveFile;
					fileDialogType = FileDialogType.Save;
					break;
				}
				default: break;
			}
		};
	}

	void InitFileDialog(){
		fileDialog.FileSelected += (filePath) => {
			switch (fileDialogType){
				case FileDialogType.Load:{
					LoadFile(filePath);
					break;
				}
				case FileDialogType.Save:{
					SaveFile(filePath);
					break;
				}
			}
		};
	}

	int? FindEdge(){
		if (selected.Count >= 2){
			for (int i = 0; i < edges.Count; i++){
				if (edges.IsActive(i)){
					Edge edge = edges[i];
					if (((int)edge.P0 == selected[0] && (int)edge.P1 == selected[1]) || ((int)edge.P0 == selected[1] && (int)edge.P1 == selected[0])){
						return i;
					}
				}
			}
		}
		return null;
	}

	int? FindSpring(){
		if (selected.Count >= 2){
			for (int i = 0; i < springs.Count; i++){
				if (springs.IsActive(i)){
					Spring spring = springs[i];
					if (((int)spring.P0 == selected[0] && (int)spring.P1 == selected[1]) || ((int)spring.P0 == selected[1] && (int)spring.P1 == selected[0])){
						return i;
					}
				}
			}
		}
		return null;
	}

	void LoadFile(string filePath){
		GD.Print("loading:", filePath);
		using (FileAccess fileAccess = FileAccess.Open(filePath, FileAccess.ModeFlags.Read)){
			points = new Pool<Point>();
			edges = new Pool<Edge>();
			springs = new Pool<Spring>();
			//Point
			uint pointCount = fileAccess.Get32();
			for (uint i = 0; i < pointCount; i++){
				points.Add(
					new Point(){
						Position = new Vector2(fileAccess.GetFloat(), fileAccess.GetFloat()),
						Mass = fileAccess.GetFloat()
					}
				);
			}
			//Edge
			uint edgeCount = fileAccess.Get32();
			for (uint i = 0; i < edgeCount; i++){
				edges.Add(
					new Edge(){
						P0 = fileAccess.Get32(),
						P1 = fileAccess.Get32(),
						Reflection = fileAccess.GetFloat()
					}
				);
			}
			//Spring
			uint springCount = fileAccess.Get32();
			for (uint i = 0; i < springCount; i++){
				springs.Add(
					new Spring(){
						P0 = fileAccess.Get32(),
						P1 = fileAccess.Get32(),
						Strength = fileAccess.GetFloat(),
						Damping = fileAccess.GetFloat()
					}
				);
			}
		}
	}

	void SaveFile(string filePath){
		GD.Print("saving:", filePath);
		using (FileAccess fileAccess = FileAccess.Open(filePath, FileAccess.ModeFlags.Write)){
			//Point
			fileAccess.Store32((uint)points.Count);
			foreach (Point point in points){
				fileAccess.StoreFloat(point.Position.X);
				fileAccess.StoreFloat(point.Position.Y);
				fileAccess.StoreFloat(point.Mass);
			}
			//Edge
			fileAccess.Store32((uint)edges.Count);
			foreach (Edge edge in edges){
				fileAccess.Store32(edge.P0);

				fileAccess.Store32(edge.P1);
				fileAccess.StoreFloat(edge.Reflection);
			}
			//Spring
			fileAccess.Store32((uint)springs.Count);
			foreach (Spring spring in springs){
				fileAccess.Store32(spring.P0);
				fileAccess.Store32(spring.P1);
				fileAccess.StoreFloat(spring.Strength);
				fileAccess.StoreFloat(spring.Damping);
			}
		}
	}
}