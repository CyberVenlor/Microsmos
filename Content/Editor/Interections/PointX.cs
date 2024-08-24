using Godot;
using System;

public partial class PointX : LineEditFloat
{
    public override void _Ready(){
        base._Ready();
		if (editor.SelectedPoint is Point point) Text = point.Position.X.ToString();
    }

    protected override void TextInputed(string text){
        base.TextInputed(text);
		if (editor.SelectedPoint != null) editor.SelectedPoint = (Point)editor.SelectedPoint with {Position = Position with {X = Text.ToFloat()}};
    }
}
