#pragma once

struct player_delta_t {
    bool pitch_changed;
    double pitch;

    bool yaw_changed;
    double yaw;

    bool forward_changed;
    bool forward;

    bool backward_changed;
    bool backward;

    bool left_changed;
    bool left;

    bool right_changed;
    bool right;

    bool use_changed;
    bool use;

    bool attack_1_changed;
    bool attack_1;

    bool attack_2_changed;
    bool attack_2;

    bool jump_changed;
    bool jump;

    bool duck_changed;
    bool duck;
};
