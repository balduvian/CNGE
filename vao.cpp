
#include "vao.h"

namespace CNGE7
{
	Attribute::Attribute(int _per_vertex, int length, float _data[])
	{
		per_vertex = _per_vertex;
		size = length * sizeof(float);
		data = std::unique_ptr<float[]>(_data);

		location = 0; // will be set by VAO

		glGenBuffers(1, &buffer);
	}

	/// the vertices in a vao are an attribute
	/// use this function to shortcut the creation
	Attribute* Attribute::create_vertex_attribute(int _length, float _data[])
	{
		return new Attribute(3, _length, _data);
	}

	Attribute::~Attribute()
	{
		glDeleteBuffers(1, &buffer);
	}

	VAO::VAO(
		int _draw_mode,
		Attribute* _vertex_attrib,
		int _index_count, int _indices[],
		int _attrib_count, Attribute _attribs[]
	) :
		index_count(_index_count),
		draw_mode(_draw_mode),
		vertex_attrib(std::unique_ptr<Attribute>(_vertex_attrib)),
		attribs(std::unique_ptr<Attribute[]>(_attribs))
	{
		// create this vao
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// the first attribute is of course vertices
		add_attribute(*vertex_attrib, 0);

		// add the rest of the custom attributes
		for (auto i = 0; i < _attrib_count; ++i)
			add_attribute(attribs[i], i + 1);

		// add index buffer
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _index_count * sizeof(int), _indices, GL_STATIC_DRAW);
	}

	VAO::~VAO()
	{
		// attributes get deleted automatically

		// delete index buffer
		glDeleteBuffers(1, &ibo);

		// delete the whole thing
		glDeleteVertexArrays(1, &vao);
	}

	/// actually draws this vao
	void VAO::render()
	{
		glBindVertexArray(vao);
		glDrawElements(draw_mode, index_count, GL_UNSIGNED_INT, 0);
	}

	/// internally adds a vertex attribute to this vao
		/// creates and returns the opengl buffer
	void VAO::add_attribute(Attribute& _attrib, int _location)
	{
		// make the attribute buffer current
		glBindBuffer(GL_ARRAY_BUFFER, _attrib.buffer);

		// draw the float array from the attribute into the buffer
		glBufferData(GL_ARRAY_BUFFER, _attrib.size, _attrib.data.get(), GL_STATIC_DRAW);

		// tell opengl where and how to use the data
		glVertexAttribPointer(_location, _attrib.per_vertex, GL_FLOAT, false, 0, nullptr);

		// activate the attribute
		glEnableVertexAttribArray(_location);

		// set the internal location of the attrib
		_attrib.location = _location;
	}

}
