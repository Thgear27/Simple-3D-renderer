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
        "obj/floor_diffuse.tga",
        floor_nm_tangent,
        african_head_spec
    );
    Model modelo2(
        african_head,
        Model::Format::with_vt,
        african_head_texture_img,
        african_head_nm_tangent,
        african_head_spec
    );
    Model modelo1(
        ae_inner,
        Model::Format::with_vt,
        "obj/african_head/african_head_eye_inner_diffuse.tga",
        "obj/african_head/african_head_eye_inner_nm_tangent.tga",
        "obj/african_head/african_head_eye_inner_spec.tga"
    );
    // Model modelo2(
    //     diablo3pose,
    //     Model::Format::with_vt,
    //     diablo3pose_texture_img,
    //     diablo3pose_nrmTangent,
    //     "obj/diablo3_pose/diablo3_pose_spec.tga"
    // );

    Renderer renderer(modelo2, img);
    renderer.setLightDirection(vec3f { 1, 1, 1 });
    renderer.setViewport(0, 0, width, height);
    renderer.setProyection(10);
    renderer.setCamera(vec3f { 0.1, 0.3, 1 }, vec3f { 0, 0, 0 }, vec3f { 0, 1, 0 });

    Timer time;    
    time.reset();
    renderer.render();
    renderer.reInitialize(modelo1);
    renderer.render();
    renderer.reInitialize(piso);
    renderer.setLightDirection(vec3f { 1, 1, 1 });
    renderer.render();
    std::cout << time.elapsed() << '\n';
    renderer.generateImg("test5.tga");

    return 0;
}