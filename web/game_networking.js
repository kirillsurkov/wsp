const msg_in = {
    login: 1,
    body_create: 4,
    body_state: 5
};

const msg_out = {
    login: 1
};

function GameNetworking(game) {
    let socket;

    let on_connect = () => {
        game.push_packet(msg_out.login, {"name": game.player_name});
    };

    let on_message = message => {
        messages = JSON.parse(message.data);
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

    return {
        connect: ip => {
            socket = new WebSocket("ws://" + ip);
            socket.onopen = on_connect;
            socket.onmessage = on_message;
        },
        send_packets: () => {
            let packets = game.get_packets();
            if (packets.length) {
                socket.send(JSON.stringify(packets));
                game.clear_packets();
            }
        }
    };
}
