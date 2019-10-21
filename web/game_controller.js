function GameController(game, time_step) {
    const Keymap = {
        87: "forward",
        65: "left",
        83: "backward",
        68: "right",
        32: "jump",
        17: "duck"
    };

    const Keys = {
        forward: 0,
        left: 1,
        down: 2,
        right: 3,
        jump: 4,
        duck: 5
    };

    const ShapeTypes = {
        0: "sphere",
        1: "box",
        2: "cylinder"
    };

    let world = new OIMO.World({gravity: [0, -10, 0], info: false, random: false, worldscale: 1, timeStep: time_step});
    let bodies = {};

    return {
        update: delta => {
            let update_world = true;
            game.process_actions(action => {
                console.log("processing frame " + game.get_frame());
                switch (action.type) {
                    case Action.body_create: {
                        let oimo_types = [];
                        let oimo_sizes = [];
                        let oimo_shape_positions = [];
                        for (let part of action.data.shapes) {
                            oimo_types.push(ShapeTypes[part.type]);
                            for (let x of part.size) {
                                oimo_sizes.push(x);
                            }
                            for (let x of part.position) {
                                oimo_shape_positions.push(x);
                            }
                        }
                        bodies[action.data.id] = world.add({type: oimo_types, move: action.data.moving, size: oimo_sizes, pos: action.data.position, posShape: oimo_shape_positions, allowSleep: false});
                        let shapes = [];
                        for (let shape = bodies[action.data.id].shapes, shape_i = oimo_types.length - 1; shape != null; shape = shape.next, shape_i--) {
                            shapes.unshift({type: oimo_types[shape_i], position: shape.relativePosition, size: action.data.shapes[shape_i].size});
                        }
                        game.objects[action.data.id] = {
                            shapes: shapes,
                            position: action.data.position
                        };
                        break;
                    }
                    case Action.body_state: {
                        update_world = false;
                        for (state of action.data.objects) {
                            let body = bodies[state.id];
                            if (!body) {
                                console.log("BODY " + state.id + " IS UNDEFINED");
                                continue;
                            }
                            body.position.set(state.position[0], state.position[1], state.position[2]);
                            body.orientation.set(state.rotation[0], state.rotation[1], state.rotation[2], state.rotation[3]);
                            body.linearVelocity.set(state.linear_speed[0], state.linear_speed[1], state.linear_speed[2]);
                            body.angularVelocity.set(state.angular_speed[0], state.angular_speed[1], state.angular_speed[2]);
                        }
                        break;
                    }
                    case Action.key: {
                        
                        break;
                    }
                }
            });

            if (update_world) {
                world.step();
            }
            game.inc_frame();

            for (id in game.objects) {
                let body = bodies[id];
                let obj = game.objects[id];
                obj.position = body.position;
                obj.rotation = body.quaternion;
                obj.linear_speed = body.linearVelocity;
                obj.angular_speed = body.angularVelocity;
            }
        },
        action_key: (code, down) => {
            let key = Keymap[code];
            if (key) {
                game.push_action(Action.key, {action: Keys[key], down: down});
            }
        }
    }
}
