function GameRenderer(game, width, height) {
    let scene = new THREE.Scene();

    let camera = new THREE.PerspectiveCamera(90, width / height);
    camera.position.set(5, 10, -20);
    camera.lookAt(0, 0, -40);

    let renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setSize(width, height);
    renderer.shadowMap.enabled = true;

    let target = new THREE.Object3D();
    target.position.set(0, 0, -40);
    scene.add(target);

    let light = new THREE.SpotLight(0xffffff, 1);
    light.position.set(-10, 20, 0);
    light.castShadow = true;
    light.target = target;
    light.shadow.mapSize.width = 1024;
    light.shadow.mapSize.height = 1024;
    scene.add(light);

    let meshes = {};

    return {
        render: () => {
            for (id in game.objects) {
                let obj = game.objects[id];
                if (!meshes[id]) {
                    let geometry = null;
                    switch (game.objects[id].type) {
                        case "box": {
                            geometry = new THREE.BoxGeometry(obj.size[0], obj.size[1], obj.size[2]);
                            break;
                        }
                        case "cylinder": {
                            geometry = new THREE.CylinderGeometry(obj.size[0], obj.size[1], obj.size[2]);
                            break;
                        }
                        case "sphere": {
                            geometry = new THREE.SphereGeometry(obj.size[0] / 2, 16, 16);
                            break;
                        }
                    }
                    let material = new THREE.MeshStandardMaterial({color: 0x77ff77});
                    let mesh = new THREE.Mesh(geometry, material);
                    mesh.receiveShadow = true;
                    mesh.castShadow = true;
                    scene.add(mesh);

                    meshes[id] = mesh;

                    console.log(meshes);
                }

                let xx = obj.rotation.x - meshes[id].quaternion.x;
                let yy = obj.rotation.y - meshes[id].quaternion.y;
                let zz = obj.rotation.z - meshes[id].quaternion.z;
                let ww = obj.rotation.w - meshes[id].quaternion.w;
                let rr = Math.sqrt(xx*xx + yy*yy + zz*zz + ww*ww);

                meshes[id].material.color.r = (meshes[id].material.color.r + rr) % 1;

                meshes[id].position.set(obj.position.x, obj.position.y, obj.position.z);
                meshes[id].quaternion.set(obj.rotation.x, obj.rotation.y, obj.rotation.z, obj.rotation.w);
            }
            renderer.render(scene, camera);
        },
        dom: renderer.domElement
    }
}
