#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

#include "UIElement.h"

class UIImage : public UIElement
{
public:
	UIImage(iPoint pos);
	~UIImage() {};

	void Update(float dt);

	void OnCollision(Collider* a, Collider* b);
};

#endif // __UI_IMAGE_H__