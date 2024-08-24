using Godot;
using System;

public partial class LineEditFloat : LineEdit
{
	protected Editor editor;

	public override void _Ready(){
		editor = GetNode<Editor>(Editor.NodePath);
		TextSubmitted += TextInputed;
		FocusExited += () => TextInputed(Text);
	}

	protected virtual void TextInputed(String text){
		if (text.IsValidFloat()) Text = text.ToFloat().ToString();
		else Text = "0";
	}
}
