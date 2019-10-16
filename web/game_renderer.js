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
                    let geometry = new THREE.Geometry();
                    for (let shape of obj.shapes) {
                        let geom = null;
                        switch (shape.type) {
                            case "box": {
                                geom = new THREE.BoxGeometry(shape.size[0], shape.size[1], shape.size[2]);
                                break;
                            }
                            case "sphere": {
                                geom = new THREE.SphereGeometry(shape.size[0], 16, 16);
                                break;
                            }
                            case "cylinder": {
                                geom = new THREE.CylinderGeometry(shape.size[0], shape.size[2], shape.size[1], 16);
                                break;
                            }
                        }
                        geometry.merge(geom, new THREE.Matrix4().makeTranslation(shape.position.x, shape.position.y, shape.position.z));
                    }
                    let material = new THREE.MeshStandardMaterial({color: 0x77ff77});
                    let mesh = new THREE.Mesh(geometry, material);
                    scene.add(mesh);
                    mesh.receiveShadow = true;
                    mesh.castShadow = true;
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
