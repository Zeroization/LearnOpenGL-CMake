#pragma once

namespace GLCore
{
	class UUID
	{
	public:
		UUID();

		long operator() () const { return m_UUID; }

	private:
		long m_UUID;
	};
}