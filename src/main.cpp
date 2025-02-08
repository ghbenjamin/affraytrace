#include <iostream>

#include <cxxopts.hpp>
#include <filesystem>


#include "Camera.h"
#include "Image.h"
#include "Material.h"
#include "Vec3.h"
#include "Scene.h"
#include "Utils.h"

using namespace affraytrace;



int do_deubg_scene( cxxopts::ParseResult const& args )
{
    CameraData cameraData;

    cameraData.width = 800;
    cameraData.height = 600;
    cameraData.origin = Vec3d{0, 2, 3};
    cameraData.target = Vec3d{0, 0, -1};
    cameraData.vertical_fov = 45;
    cameraData.samplesPerPixel = 10;


    Scene mainScene{ cameraData };

    mainScene.addMaterial<DiffuseMaterial>( "ground", Colour{0.8, 0.8, 0.0} );
    mainScene.addMaterial<DiffuseMaterial>( "center", Colour{0.1, 0.2, 0.5} );
    mainScene.addMaterial<MetalMaterial>( "left", Colour{0.8, 0.8, 0.8} );
    mainScene.addMaterial<MetalMaterial>( "right", Colour{0.8, 0.6, 0.5} );

    mainScene.addObject<SphereObject>( "ground", Vec3d(0.0, -100.5, -1.0), 100.0 );
    mainScene.addObject<SphereObject>( "center", Vec3d(0.0,    0.0, -1.6), 0.5 );
    mainScene.addObject<SphereObject>( "left", Vec3d(-1.0,    0.0, -0.8), 0.5 );
    mainScene.addObject<SphereObject>( "right", Vec3d(1.5,    0.1, -1.0), 0.6 );

    mainScene.render_to_file( "debug_output.png" );
    return 0;
}

int do_scene_file( cxxopts::ParseResult const& args )
{
    std::string input_filename = args["scene"].as<std::string>();

    auto maybe_scene =  Scene::load_scene(input_filename);

    if ( std::holds_alternative<std::exception>(maybe_scene) )
    {
        std::cerr << "Error: " << std::get<std::exception>(maybe_scene).what() << std::endl;
        return 1;
    }

    std::string output_filename;

    if ( args.count("output") )
    {
        output_filename = args["output"].as<std::string>();
    }
    else
    {
        std::filesystem::path p = input_filename;
        output_filename = p.stem().string() + ".png";
    }

    std::get<Scene>(maybe_scene).render_to_file( output_filename );

    return 0;
}


int main( int argc, char** argv )
{
    log_string( "affraytrace v0.0.1" );

    cxxopts::Options options("affraytrace", "a very very basic raytracer");
    options.add_options()
        ("h,help", "print help message")
        ("scene", "scene file", cxxopts::value<std::string>())
        ("f,fast",  "quick render for testing scene layout", cxxopts::value<bool>()->default_value("false"))
        ( "d,debug", "run the debug scene, not a provided scene", cxxopts::value<bool>()->default_value("false"))
        ("o,output", "output file", cxxopts::value<std::string>())
    ;

    auto cmd_args = options.parse(argc, argv);


    // Early return for explcit help request
    if (cmd_args.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }


    // Very simple argument validation

    if (!cmd_args.count("debug") && !cmd_args.count("scene"))
    {
        log_string("No target scene provided.");
        exit(1);
    }



    if ( cmd_args.count("debug") )
    {
        return do_deubg_scene( cmd_args );
    }
    else
    {
        return do_scene_file( cmd_args );
    }

}


