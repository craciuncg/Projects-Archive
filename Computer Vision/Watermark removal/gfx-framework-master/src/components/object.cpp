#include "object.h"
#include <forward_list>
#include <algorithm>

gfxc::Object::Object(std::string name, gfxc::SimpleScene *scene) :
	kids(), parts() {
	this->_name = name;
	this->scene = scene;
	this->par = nullptr;
	this->fls = { false, false };
	scal = glm::um4;
	rot = glm::um4;
	pos = glm::um4;
}

gfxc::Object::Object(std::string name, gfxc::SimpleScene *scene, Object* parent)
	: Object(name, scene) {
	this->par = parent;
}

gfxc::Object::~Object() {
	auto oit = kids.begin();
	while (oit != kids.end()) {
		delete oit->second;
		oit++;
	}
}

gfxc::Object::Object(const gfxc::Object& ref)
	: parts(ref.parts) {
	_name = ref._name;
	scene = ref.scene;
	scal = glm::um4;
	rot = glm::um4;
	pos = glm::um4;
}

void gfxc::Object::add(std::string prefix, Mesh* part, glm::mat4 scal, glm::mat4 mods, glm::mat4 pos) {
	parts[prefix + part->GetMeshID()] = {part, scal, mods, pos};
}

void gfxc::Object::remove(std::string name) {
	delete kids[name];  kids.erase(name);
	parts.erase(name);
}

gfxc::Object* gfxc::Object::orphan(std::string name) {
	Object* rez = kids[name];
	kids.erase(name);
	return rez;
}

Mesh* gfxc::Object::detach(std::string name) {
	Mesh* rez = parts[name].mesh;
	parts.erase(name);
	return rez;
}

void gfxc::Object::render(Shader *shader, glm::mat4 parTrans) {
	parTrans = parTrans * pos * rot * scal;
	auto oit = kids.begin();
	while (oit != kids.end()) {
		oit->second->render(shader, parTrans);
		oit++;
	}
	auto mit = parts.begin();
	while (mit != parts.end()) {
		scene->RenderMesh(mit->second.mesh, shader, 
			parTrans * mit->second.pos * mit->second.rot * mit->second.scal);
		mit++;
	}
}

void gfxc::Object::update(float dt) {
	if (!fls.pause) {
		onUpdate(dt);
		auto oit = kids.begin();
		while (oit != kids.end()) {
			if (oit->second->fls.todel) {
				remove(oit->second->name());
				oit = kids.begin();
			}
			oit++;
		}
		oit = kids.begin();
		while (oit != kids.end()) {
			oit->second->update(dt);
			oit++;
		}
	}
}

void gfxc::Object::clicker(glm::vec3 pos, glm::vec3 dir) {
	if (wasclicked(pos, dir)) {
		onClick();
		auto oit = kids.begin();
		while (oit != kids.end()) {
			oit->second->clicker(glm::vec4(pos, 1.0f) * scal * rot * this->pos, glm::vec4(dir, 0.0f) * scal * rot * this->pos);
			oit++;
		}
	}
}