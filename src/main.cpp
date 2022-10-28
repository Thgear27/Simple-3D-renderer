#include "Renderer.hpp"
#include "Timer.hpp"
#include "model.hpp"
#include "tgaimage.h"
#include "objpaths.hpp"
#include <iostream>

constexpr int height { 1200 };
constexpr int width  { 1200 };

const char* whiteTexture = "obj/rabbit/outTexture.tga";

int main() {
    TGAImage img { width, height, TGAImage::Format::RGB };
    // Model modelo1("obj/floor.obj", Model::Format::with_vt, "obj/floor_diffuse.tga", "obj/floor_nm_tangent.tga");

    Model piso(
        floor_obj,
        Model::Format::with_vt,
        "obj/rock-diffuse.tga",
        "obj/rock-normal.tga",
        "obj/rock-diffuse.tga"
    );
    Model modelo2(
        boogie_body,
        Model::Format::with_vt,
        boogie_body_texture_img,
        "obj/boggie/body_nm_tangent.tga",
        "obj/boggie/body_spec.tga"
    );
    Model Cara(
        boogie_head,
        Model::Format::with_vt,
        boogie_head_texture_img,
        "obj/boggie/head_nm_tangent.tga",
        "obj/boggie/head_spec.tga"
    );
    Model Ojos(
        "obj/boggie/eyes.obj",
        Model::Format::with_vt,
        "obj/boggie/eyes_diffuse.tga",
        "obj/boggie/eyes_nm_tangent.tga",
        "obj/boggie/eyes_spec.tga"
    );

    Renderer renderer(modelo2, img);
    renderer.setLightDirection(vec3f { 1, 0, 0 });
    renderer.setViewport(0, 0, width, height);
    renderer.setProyection(10);
    renderer.setCamera(vec3f { 1, 1, 1 }, vec3f { 0, 0, 0 }, vec3f { 0, 1, 0 });

    Timer time;    
    time.reset();
    renderer.render();
    renderer.reInitialize(Ojos);
    renderer.render();
    renderer.reInitialize(Cara);
    renderer.render();
    renderer.reInitialize(piso);
    renderer.render();
    std::cout << time.elapsed() << '\n';
    renderer.generateImg("test6.tga");

    return 0;
}