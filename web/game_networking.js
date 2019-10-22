const Protocol = {
    json: 0,
    binary: 1
};

const msg_in = {
    login: 1,
    body_create: 4,
    body_state: 5
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
    let socket;

    let on_connect = () => {
        game.push_packet(packets_out.login(game.player_name));
    };

    let on_message = message => {
        let messages = [];
        switch (protocol) {
            case Protocol.json: {
                messages = JSON.parse(message.data);
                break;
            }
            case Protocol.binary: {
                break;
            }
        }
        console.log(messages);
        for (let message of messages) {
            switch (message.type) {
                case msg_in.login: {
                    game.set_frame(message.data.frame - 5);
                    break;
                }
                case msg_in.body_create: {
                    game.push_action(Action.body_create, message.data);
                    break;
                }
                case msg_in.body_state: {
                    game.push_action(Action.body_state, message.data);
                    break;
                }
            }
        }
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
                    break;
                }
            }
            console.log(buffer);
            socket.send(buffer);
            game.clear_packets();
        }
    };
}
