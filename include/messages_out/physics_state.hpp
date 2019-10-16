#pragma once

#include "messages_out/message.hpp"
#include "physics.hpp"

#include <glm/glm.hpp>

namespace message::out {
	class physics_state_t : public message_t {
	private:
		int m_frame;
		std::unordered_map<int, physics_t::object_state_t> m_state;

	protected:
	    virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

	public:
		physics_state_t(int frame, const std::unordered_map<int, physics_t::object_state_t>& state);
		virtual ~physics_state_t();
	};
}
