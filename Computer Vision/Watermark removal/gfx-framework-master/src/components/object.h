#include "components/simple_scene.h"
#include <map>
#include <string>

#ifndef _opengl_object
#define _opengl_object

namespace gfxc {
	/* Basic hierarchical 3D object representation
		Keeps and maneuvres flags
		Updates itself recursively
		Renders itself recursively*/
	class Object {
	protected:
		struct MeshPos {
			Mesh* mesh;
			glm::mat4 scal;
			glm::mat4 rot;
			glm::mat4 pos;
		};
		glm::mat4 scal, rot, pos;
		std::string _name;
	public:
		struct flags {
			bool todel;
			bool pause;
		};
		struct flags fls;
	protected:
		SimpleScene* scene;
		Object* par;
		std::map<std::string, Object*> kids;
		std::map<std::string, MeshPos> parts;
		/* Local logic and animation function; override as necessary */
		virtual void onUpdate(float dt) {}
		/* Local on click logic; override as necessary */
		virtual void onClick() {}
		/* Local click check; override as necessary */
		virtual bool wasclicked(glm::vec3 cpos, glm::vec3 cdir) { return false; }
	public:
		SimpleScene* getscene() { return scene; }
		/* Updates the object recursively, calling the local onUpdate() function
			Also checks for flags from the precedent iteration before updating kids, but after updating this
			Does not render objects, as recursivity means meshes would be rendered as overlapping and majorly decrease performance and artifacts*/
		void update(float dt);
		/* Takes name and owner scene */
		Object(std::string, gfxc::SimpleScene*);
		Object(std::string, gfxc::SimpleScene*, Object*);
		/* Copy constructor */
		Object(const gfxc::Object&);
		/* Deletes all child objects before this */
		~Object();
		/* Renames object */
		void rename(std::string newn) { _name = newn; }
		/* Adds mesh to part map, with internal mesh name as descriptor*/
		void add(Mesh* part, glm::mat4 scal = glm::um4, glm::mat4 rot = glm::um4, glm::mat4 pos = glm::um4) {
			add("", part, scal, rot, pos);
		}
		/* Adds mesh to parts map, with prefix added to internal mesh name for the descriptor */
		void add(std::string prefix, Mesh* part, glm::mat4 scal = glm::um4, glm::mat4 rot = glm::um4, glm::mat4 pos = glm::um4);
		/* Adds object to children map, with object name used as descriptor */
		void add(Object* kid) { kids[kid->name()] = kid; }
		/* Gets kid with given name */
		Object* get(std::string name) { if (!kids.count(name)) return nullptr; return kids[name]; }
		/* Multiplies translation matrix */
		void move(glm::mat4 tr) { pos *= tr; }
		/* Sets translation matrix */
		void setpos(glm::mat4 p) { pos = p; }
		/* Multiplies scale matrix */
		void size(glm::mat4 rs) { scal *= rs; }
		/* Sets shape matrix */
		void setscale(glm::mat4 sh) { scal = sh; }
		/* Multiplies rotation matrix */
		void spin(glm::mat4 rs) { rot *= rs; }
		/* Sets shape matrix */
		void setrot(glm::mat4 sh) { rot = sh; }
		/* Get global position as matrix */
		glm::mat4 getglobmod() { return (par == nullptr ? glm::um4 : par->getglobmod()) * pos * rot * scal; }
		const std::string name() { return _name; };
		/* Removes any objects (also deleted) and / or meshes with the passed name as descriptor */
		void remove(std::string name);
		/* Detaches and returns an object with the given descriptor */
		Object* orphan(std::string);
		/* Detaches and returns a mesh with the given descriptor */
		Mesh* detach(std::string);
		/* Renders the object recursively; parTrans is the parent transformation and is applied before local ones*/
		void render(Shader* shader, glm::mat4 parTrans = glm::um4);
		void render(Shader shader, glm::mat4 parTrans = glm::um4) { render(&shader, parTrans); }
		/* Checks if object is clicked on and recurses for kids */
		void clicker(glm::vec3 pos, glm::vec3 dir);
		/* Gets position of object */
		glm::vec3 getpos() { return glm::vec3(pos * glm::vec4(glm::vec3(0.0f), 1.0f)); }
		/* Matrix getters */
		glm::mat4 getlocalpos() { return pos; }
		glm::mat4 getlocalrot() { return rot; }
		glm::mat4 getlocalscal() { return scal; }
	};
};
#endif // !_opengl_object
