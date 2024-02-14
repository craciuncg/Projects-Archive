#ifndef ENTITY_H
#define ENTITY_H

#include "../Constants.h"

class Entity
{
public:
	float x;
	float y;

	size_t width;
	size_t height;

	Entity();
	virtual ~Entity();

	bool operator==(const Entity& other);

	bool collisionUp(const Entity& other);
	bool collisionDown(const Entity& other);
	bool collisionRight(const Entity& other);
	bool collisionLeft(const Entity& other);

	// TODO: design and implement funcions for window collisions

	bool collision(const Entity& other);

protected:
private:
};

#endif // ENTITY_H
