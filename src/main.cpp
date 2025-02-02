#include <iostream>


#include "Camera.h"
#include "Image.h"
#include "Vec3.h"
#include "Ray.h"
#include "Scene.h"

using namespace affraytrace;



int main()
{
    Scene mainScene;
    mainScene.addObject<SphereObject>( Vec3d(0, 0, -3), 0.5f );
    // mainScene.addObject<SphereObject>( Vec3d(1,-1,-8), 0.5f );
    mainScene.addObject<SphereObject>( Vec3d(0,-100.5,-1), 100.f );

    Camera camera{ 800, 600, Vec3d(0, 0, 0), 2.f };

    Image rendered = camera.render(mainScene);
    rendered.save("image_v2.png" );

    return 0;

    return 0;
}


