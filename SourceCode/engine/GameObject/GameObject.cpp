#include "GameObject.h"
#include <iostream>

int GameObject::id_counter_ = 0;

GameObject::GameObject(const std::string &name) :
	instance_id_(id_counter_++),	// ID‚ğŠ„‚èU‚è’l‚ğ‰ÁZ
	name_(name)
{
}

GameObject::~GameObject()
{
}