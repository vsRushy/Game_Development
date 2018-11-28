#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#include "p2DynArray.h"

#include "SDL/include/SDL_rect.h"

#define CURSOR_WIDTH 2

enum class UI_ELEMENT_TYPE
{
	UI_BUTTON,
	UI_LABEL,
	UI_IMAGE,
	UI_CHECKBOX,
	UI_TEXTBOX
};

struct SDL_Texture;
struct Collider;

// TODO 1: Create your structure of classes
class UIElement;
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Update
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	const SDL_Texture* GetAtlas() const;

public:
	UIElement* CreateUIElement(iPoint pos, UI_ELEMENT_TYPE type, SDL_Rect area);
	/*void DeleteUIElement(UIElement* ui_element);
	void DeleteAllUIElements();
	void OnCollision(Collider*, Collider*);*/

private:
	p2DynArray<UIElement*> ui_elements = NULL;

	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__