const Protocol = {
    json: 0,
    binary: 1
};

const msg_in = {
    login: {
        id: 1,
        create_binary: function(data) {
            return {
                frame: data.frame
            };
        }
    },
    body_create: {
        id: 4,
        create_binary: function(data) {
            console.log("CREATE");
            console.log(new Uint8Array(data.data.buffer));
            let res = {
                id: data.data.getUint16(data.offset, true),
                moving: !!data.data.getUint8(data.offset + 2),
                position: [
                    data.data.getFloat32(data.offset + 3, true),
                    data.data.getFloat32(data.offset + 7, true),
                    data.data.getFloat32(data.offset + 11, true)
                ],
                rotation: [
                    data.data.getFloat32(data.offset + 15, true),
                    data.data.getFloat32(data.offset + 19, true),
                    data.data.getFloat32(data.offset + 23, true),
                    data.data.getFloat32(data.offset + 27, true)
                ],
                rotation_factor: [
                    data.data.getFloat32(data.offset + 31, true),
                    data.data.getFloat32(data.offset + 35, true),
                    data.data.getFloat32(data.offset + 39, true)
                ],
                shapes: []
            };
            data.offset += 43;
            let shapes_count = data.data.getUint8(data.offset++);
            for (let i = 0; i < shapes_count; i++) {
                res.shapes.push({
                    type: data.data.getUint8(data.offset),
                    position: [
                        data.data.getFloat32(data.offset + 1, true),
                        data.data.getFloat32(data.offset + 5, true),
                        data.data.getFloat32(data.offset + 9, true)
                    ],
                    rotation: [
                        data.data.getFloat32(data.offset + 13, true),
                        data.data.getFloat32(data.offset + 17, true),
                        data.data.getFloat32(data.offset + 21, true),
                        data.data.getFloat32(data.offset + 25, true)
                    ],
                    size: [
                        data.data.getFloat32(data.offset + 29, true),
                        data.data.getFloat32(data.offset + 33, true),
                        data.data.getFloat32(data.offset + 37, true)
                    ]
                });
                data.offset += 41;
            }
            return res;
        }
    },
    body_state: {
        id: 5,
        create_binary: function(data) {
            let res = {
                objects: []
            };
            let objects_count = data.data.getUint32(data.offset, true);
            data.offset += 4;
            for (let i = 0; i < objects_count; i++) {
                res.objects.push({
                    id: data.data.getUint16(data.offset, true),
                    sleeping: !!data.data.getUint8(data.offset + 2),
                    position: [
                        data.data.getFloat32(data.offset + 3, true),
                        data.data.getFloat32(data.offset + 7, true),
                        data.data.getFloat32(data.offset + 11, true)
                    ],
                    rotation: [
                        data.data.getFloat32(data.offset + 15, true),
                        data.data.getFloat32(data.offset + 19, true),
                        data.data.getFloat32(data.offset + 23, true),
                        data.data.getFloat32(data.offset + 27, true)
                    ],
                    linear_speed: [
                        data.data.getFloat32(data.offset + 31, true),
                        data.data.getFloat32(data.offset + 35, true),
                        data.data.getFloat32(data.offset + 39, true)
                    ],
                    angular_speed: [
                        data.data.getFloat32(data.offset + 43, true),
                        data.data.getFloat32(data.offset + 47, true),
                        data.data.getFloat32(data.offset + 51, true)
                    ]
                });
                data.offset += 55;
            }
            return res;
        }
    }
};

const msg_out = {
    login: 1
};

const packets_out = {
    login: function(name) {
        return {
            type: msg_out.login,
            data: {
                name: name
            },
            keys: ["name"]
        }
    }
}

function GameNetworking(game, protocol) {
    // TODO: Make better
    let parse_messages = {}
    parse_messages[Protocol.json] = function(data) {
        return JSON.parse(new TextDecoder("UTF-8").decode(data));
    };
    parse_messages[Protocol.binary] = function(data) {
        let info = {
            data: data,
            offset: 0
        };
        let messages = [];
        let count = info.data.getUint8(info.offset++);
        console.log("count: " + count);
        a=[];
        for (let i = 0; i < count; i++) {
            let type = info.data.getUint8(info.offset);
            let frame = info.data.getUint32(info.offset + 1, true);
            info.offset += 5;
            let message_info = Object.values(msg_in).find(message => message.id == type);
            if (message_info) {
                let message = message_info.create_binary(info);
                message.frame = frame;
                messages.push({type: type, data: message});
                a.push(i);
            } else {
                console.log(type + " not found at i = " + i);
            }
        }
        console.log(a);
        return messages;
    };

    let socket;

    let on_connect = () => {
        game.push_packet(packets_out.login(game.player_name));
    };

    let on_message = message => {
        let data = new DataView(message.data);
        let messages = parse_messages[protocol](data);
        console.log("received messages");
        console.log(messages);
        for (let message of messages) {
            switch (message.type) {
                case msg_in.login.id: {
                    game.set_frame(message.data.frame - 5);
                    break;
                }
                case msg_in.body_create.id: {
                    game.push_action(Action.body_create, message.data);
                    break;
                }
                case msg_in.body_state.id: {
                    game.push_action(Action.body_state, message.data);
                    break;
                }
            }
        }
        console.log("on_message done");
    };

    let binary_buffer = new DataView(new ArrayBuffer(102400)); // 100K

    return {
        connect: ip => {
            socket = new WebSocket("ws://" + ip);
            socket.binaryType = "arraybuffer";
            socket.onopen = on_connect;
            socket.onmessage = on_message;
        },
        send_packets: () => {
            let packets = game.get_packets();
            if (!packets.length) {
                return;
            }
            let buffer;
            switch (protocol) {
                case Protocol.json: {
                    let out_buffer = [];
                    for (let packet of packets) {
                        out_buffer.push(packet.keys.reduce((res, key) => (res.data[key] = packet.data[key], res), {type: packet.type, data: {}}));
                    }
                    buffer = JSON.stringify(out_buffer);
                    break;
                }
                case Protocol.binary: {
                    let offset = 0;
                    binary_buffer.setUint8(offset++, packets.length);
                    for (let packet of packets) {
                        binary_buffer.setUint8(offset++, packet.type);
                        for (let key of packet.keys) {
                            let value = packet.data[key];
                            switch (typeof value) {
                                case "string": {
                                    binary_buffer.setUint8(offset++, value.length);
                                    for (let i = 0; i < value.length; i++) {
                                        binary_buffer.setUint8(offset++, value.charCodeAt(i));
                                    }
                                    break;
                                }
                                case "number": {
                                    if (value % 1 === 0) {
                                        binary_buffer.setInt32(offset, value);
                                    } else {
                                        binary_buffer.setFloat32(offset, value);
                                    }
                                    offset += 4;
                                    break;
                                }
                            }
                        }
                    }
                    buffer = new ArrayBuffer(offset);
                    let buffer_view = new Uint8Array(buffer);
                    buffer_view.set(new Uint8Array(binary_buffer.buffer, 0, offset));
                    console.log("sending bytes");
                    console.log(buffer_view);
                    break;
                }
            }
            socket.send(buffer);
            game.clear_packets();
        }
    };
}
