#include "j1App.h"
#include "UIElement.h"
#include "UIImage.h"
//#include "j1Collision.h"
#include "j1Gui.h"

UIImage::UIImage(iPoint pos) : UIElement(position, type, area)
{
	type = UI_ELEMENT_TYPE::UI_IMAGE;
}

void UIImage::Update(float dt)
{

}

void UIImage::OnCollision(Collider* a, Collider* b)
{

}