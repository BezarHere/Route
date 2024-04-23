#pragma once
#include "Bases.h"
#include "Resource.h"
#include "Vector.h"

namespace route
{
	typedef vpid_t FrambufferID;

	enum class FramebufferAttachmentType
	{
		WriteOnly,
		ReadWrite,
	};

	enum class FramebufferAttachmentTarget
	{
		Color,
		Depth,
		Stencil
	};

	struct FramebufferAttachment
	{
		FramebufferAttachmentType type;
		FramebufferAttachmentTarget target;
		RID rid;
	};

	class Framebuffer : GraphicsResource
	{
	public:


	private:
		FrambufferID m_id;
		vector<FramebufferAttachment> m_attachments;
	};

}
