#pragma once

#include <vector>
#include <memory>

#include "game_object.hpp"

class physics_object_t : public game_object_t {
public:
    enum class shape_type {
        sphere,
        box,
        cylinder
    };

    struct shape_t {
        shape_type type;
        glm::vec3 position;
        glm::vec4 rotation;
        glm::vec3 size;
    };

    struct body_state_t {
        glm::vec3 position;
        glm::vec4 rotation;
        glm::vec3 linear_speed;
        glm::vec3 angular_speed;
    };

    class body_t {
    private:
        int m_id;
        bool m_moving;
        glm::vec3 m_position;
        glm::vec4 m_rotation;
        glm::vec3 m_rotation_factor;
        std::vector<std::shared_ptr<shape_t>> m_shapes;

    public:
        body_t(int id);
        ~body_t();

        void set_moving(bool moving);
        void set_position(const glm::vec3& position);
        void set_rotation(const glm::vec4& rotation);
        void set_rotation_factor(const glm::vec3& rotation_factor);
        void add_shape(shape_type type, const glm::vec3& position, const glm::vec4& rotation, const glm::vec3& size);

        int get_id() const;
        bool is_moving() const;
        const glm::vec3& get_position() const;
        const glm::vec4& get_rotation() const;
        const glm::vec3& get_rotation_factor() const;
        const std::vector<std::shared_ptr<shape_t>>& get_shapes() const;
    };

protected:
    body_t m_body;

public:
    physics_object_t(int id, int body_id);
    virtual ~physics_object_t() override;

    void set_position(const glm::vec3& position);
    void set_rotation(const glm::vec4& rotation);
    virtual const glm::vec3& get_position() const override;
    virtual const glm::vec4& get_rotation() const override;

    const body_t& get_body() const;
};
