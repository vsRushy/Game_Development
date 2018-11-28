#ifndef __UI_ELEMENT_H__
#define __UI_ELEMENT_H__

#include "p2Point.h"
#include "j1Gui.h"

#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct Collider;

class UIElement
{
protected:
	Collider* collider;

public:
	iPoint position;
	SDL_Rect area;
	UI_ELEMENT_TYPE type;

public:
	UIElement(iPoint pos, UI_ELEMENT_TYPE, SDL_Rect area = {0, 0, 0, 0});
	virtual ~UIElement();

	//const Collider* GetCollider() const;

	virtual void Update(float dt) {}
	virtual void Draw(SDL_Texture* tex, float speed = 1.0f);

	void OnCollision(Collider*, Collider*);
};

#endif // __UI_ELEMENT_H__