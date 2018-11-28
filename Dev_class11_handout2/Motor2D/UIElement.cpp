#include "j1App.h"
#include "UIElement.h"
//#include "j1Collision.h"
#include "j1Render.h"

UIElement::UIElement(iPoint pos, UI_ELEMENT_TYPE type, SDL_Rect area) : position(pos), type(type), area(area)
{
	
}

UIElement::~UIElement()
{
	/*if (collider != nullptr)
		collider->to_delete = true;*/
}

/*const Collider* UIElement::GetCollider() const
{
	return collider;
}*/

void UIElement::Draw(SDL_Texture* texture, float speed)
{
	/*if (collider != nullptr)
		collider->SetPos(position.x, position.y);*/

	App->render->Blit(texture, position.x, position.y, &area, speed, 0.0f);
}