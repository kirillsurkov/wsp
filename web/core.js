const WIDTH = 512;
const HEIGHT = 384;

function start_game(server_ip, player_name) {
    const time_step = 1 / 60;

    let game = new Game(player_name);
    let game_renderer = new GameRenderer(game, WIDTH, HEIGHT);
    let game_controller = new GameController(game, time_step);
    let game_networking = new GameNetworking(game);
    game_networking.connect(server_ip);
    document.getElementsByTagName("body")[0].append(game_renderer.dom);

    let last_time = window.performance.now();
    let fps_timer = 0;

    (function render_loop() {
        let now = window.performance.now();
        let delta = (now - last_time) / 1000;
        last_time = now;

        fps_timer += delta;
        while (fps_timer >= time_step) {
            fps_timer -= time_step;
            game_controller.update(time_step);
            game_networking.send_packets();
            game_renderer.render();
        }

        requestAnimationFrame(render_loop);
    })();

    document.addEventListener("keydown", e => {
        game_controller.action_key(e.keyCode, true);
    });
    document.addEventListener("keyup", e => {
        game_controller.action_key(e.keyCode, false);
    });
}
