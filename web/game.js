const Action = {
    body_create: 0,
    body_state: 1,
    object_create: 2,
    key: 3
}

function Game(player_name) {
    let frame = 0;
    let actions = {};
    let packets = [];
    return {
        player_name: player_name,
        objects: {},
        get_frame: () => {
            return frame;
        },
        set_frame: n => {
            frame = n;
        },
        inc_frame: () => {
            if (frame) {
                frame++;
            }
        },
        push_packet: (type, data) => {
            packets.push({type: type, data: data});
        },
        clear_packets: () => {
            packets = [];
        },
        get_packets: () => {
            return packets;
        },
        push_action: (type, data) => {
            if (!actions[data.frame]) {
                actions[data.frame] = [];
            }
            actions[data.frame].push({type: type, data: data})
            console.log(Object.keys(Action)[type] + " at frame " + data.frame + ". current is " + frame + ". time: " + performance.now());
        },
        process_actions: callback => {
            if (actions[frame]) {
                for (let action of actions[frame]) {
                    callback(action);
                }
                actions[frame] = null;
            }
        }
    }
}
