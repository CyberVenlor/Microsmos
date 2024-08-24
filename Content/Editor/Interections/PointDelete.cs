using Godot;
using System;

public partial class PointDelete : Button
{
	public override void _Ready()
	{
		Editor editor = GetNode<Editor>(Editor.NodePath);
		Pressed += () => editor.PointRemove();
	}
}
