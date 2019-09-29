#pragma once

class object_t {
private:
	int m_id;

public:
	object_t(int id);
	~object_t();

	int get_id() const;
};
