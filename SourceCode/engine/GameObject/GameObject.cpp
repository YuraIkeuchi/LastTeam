#include "GameObject.h"

#include <iostream>

#include "scene/BaseScene.h"

int GameObject::id_counter_ = 0;

GameObject::GameObject(const std::string &name) :
	instance_id_(id_counter_++),	// ID������U��l�����Z
	name_(name)
{
}

GameObject::~GameObject()
{
}