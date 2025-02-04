#include <iostream>


#include "Camera.h"
#include "Image.h"
#include "Material.h"
#include "Vec3.h"
#include "Scene.h"
#include "Utils.h"

using namespace affraytrace;


int main()
{
    Random randomState;

    auto material_ground  = std::make_unique<DiffuseMaterial>( Colour{0.8, 0.8, 0.0} );
    auto material_center  = std::make_unique<DiffuseMaterial>( Colour{0.1, 0.2, 0.5} );
    auto material_left    = std::make_unique<MetalMaterial>( Colour{0.8, 0.8, 0.8} );
    auto material_right   = std::make_unique<MetalMaterial>( Colour{0.8, 0.6, 0.5} );

    Scene mainScene;

    mainScene.addObject<SphereObject>( Vec3d(0.0, -100.5, -1.0), 100.0, material_ground.get() );
    mainScene.addObject<SphereObject>( Vec3d(0.0,    0.0, -1.2), 0.5, material_center.get() );
    mainScene.addObject<SphereObject>( Vec3d(-1.0,    0.0, -0.8), 0.5, material_left.get() );
    mainScene.addObject<SphereObject>( Vec3d(1.5,    0.1, -1.0), 0.6, material_left.get() );

    // mainScene.addObject<SphereObject>( Vec3d(0.0, -100.5, -1.0), 100.0, material_ground.get() );
    // mainScene.addObject<SphereObject>( Vec3d(0.0,    0.0, -1.2), 0.5, material_left.get() );


    CameraData cameraData;
    Camera camera{ randomState, cameraData };

    Image rendered = camera.render(mainScene);
    rendered.save("image_v3.png" );

    return 0;
}


