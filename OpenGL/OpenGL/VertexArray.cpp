#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElement();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		//enable vertexattrib
		GLCall(glEnableVertexAttribArray(i));
		//index 0 because its first attirbute
		//2 coordinate position so size is 2
		//type of positions is floats
		//no normalization yet
		//amount of offset to get to the next vertex = 2 floats the x and y
		//no offset
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
			layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
