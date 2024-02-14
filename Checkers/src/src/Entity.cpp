#include "..\include\Entity.h"

Entity::Entity()
{
	x = 0.f;
	y = 0.f;
}

Entity::~Entity()
{
	//dtor
}

bool Entity::operator==(const Entity& other)
{
	return x == other.x && y == other.y;
}

bool Entity::collisionRight(const Entity& other)
{
	return (fabs(x + width - other.x) <= epsilon && fabs(y - other.y) <= epsilon);
}

bool Entity::collisionLeft(const Entity& other)
{
	return (fabs(x - (other.x + width)) <= epsilon && fabs(y - other.y) <= epsilon);
}

bool Entity::collisionDown(const Entity& other)
{
	return (fabs(y + height - other.y) <= epsilon && fabs(x - other.x) <= epsilon);
}

bool Entity::collisionUp(const Entity& other)
{
	return (fabs(y - (other.y + height)) <= epsilon && fabs(x - other.x) <= epsilon);
}

bool Entity::collision(const Entity& other)
{
	return ((y <= other.y + other.height)
			   && (y >= other.y - height)
			   && (x <= other.x + other.width)
			   && (x >= other.x - width))
		|| ((other.y <= y + height) && (other.y >= y - other.height) && (other.x <= x + width) && (other.x >= x - other.width));
}
