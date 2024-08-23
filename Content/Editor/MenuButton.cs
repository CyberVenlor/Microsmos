using System;
using System.Linq;
using Godot;
using Godot.Collections;

public partial class MenuButton : Button
{
	[Export] int menuCatagory = 0;
	[Export] Control Menu;

	static Array<MenuButton> menuButtons = new Array<MenuButton>();

	public MenuButton(){
		menuButtons.Add(this);
	}

	void ProcessAllButtons(Action<MenuButton> action){
		foreach (MenuButton button in menuButtons){
			if (button.menuCatagory == menuCatagory) action.Invoke(button);
		}
	}

    public override void _Ready(){
		if (Menu == null) GD.PushError("menu not set to a control");
        ButtonDown += () => {
			ProcessAllButtons((button) => button.Menu.Visible = false );
			Menu.Visible = true;

			ProcessAllButtons((button) => button.Disabled = false );
			Disabled = true;
		};
    }
}
