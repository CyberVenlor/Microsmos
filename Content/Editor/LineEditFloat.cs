using Godot;
using System;

public partial class LineEditFloat : LineEdit
{
	public override void _Ready(){
		TextSubmitted += TextInputed;
		FocusExited += () => TextInputed(Text);
	}

	void TextInputed(String text){
		if (text.IsValidFloat()) Text = text.ToFloat().ToString();
		else Text = "0";
	}
}
