#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

#include "SDL/include/SDL_render.h"

#include "UIElement.h"
#include "UIImage.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update guis
bool j1Gui::Update(float dt)
{
	for (uint i = 0; i < ui_elements.Count(); ++i)
		if (ui_elements[i] != nullptr) ui_elements[i]->Update(dt);

	for (uint i = 0; i < ui_elements.Count(); ++i)
		if (ui_elements[i] != nullptr && ui_elements[i]->type == UI_ELEMENT_TYPE::UI_IMAGE)
			ui_elements[i]->Draw(atlas);

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

UIElement* j1Gui::CreateUIElement(iPoint pos, UI_ELEMENT_TYPE type, SDL_Rect area)
{
	UIElement* ret = nullptr;

	switch (type)
	{
	case UI_ELEMENT_TYPE::UI_IMAGE:
		ret = new UIImage(pos);
		break;
	default:
		break;
	}

	if (ret != nullptr)
		ui_elements.PushBack(ret);

	return ret;
}