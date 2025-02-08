#include "Scene.h"

#include <filesystem>
#include "tinyxml2.h"
#include "Interval.h"


namespace affraytrace
{
Scene::Scene(CameraData const &camera_data)
    : m_randomState( std::make_shared<Random>() ), m_camera( m_randomState, camera_data) {}

std::variant<Scene, std::exception> Scene::load_scene(std::string const &filename)
{
    // Check that the file is there before bothering to invoke the parser
    if (!std::filesystem::exists( filename ))
    {
        return IOException( "Scene file not found" );
    }

    // Attempt to load the target XML file
    tinyxml2::XMLDocument scene_file;
    if (auto load_success = scene_file.LoadFile( filename.c_str() ); load_success != tinyxml2::XML_SUCCESS )
    {
        return FormatException( std::format("Error loading scene file '{}' with error {}", filename, (int)load_success ) );
    }


    // Load the camera data

    auto scene_root = scene_file.RootElement();

    CameraData camera_data;
    auto camera_root = scene_root->FirstChildElement( "camera" );
    if (auto camera_image = camera_root->FirstChildElement( "image" ); camera_image != nullptr )
    {
        camera_image->QueryIntAttribute("width", &camera_data.width);
        camera_image->QueryIntAttribute("height", &camera_data.height);
    }

    if (auto camera_origin = camera_root->FirstChildElement( "origin" ); camera_origin != nullptr )
    {
        double x, y, z;
        camera_origin->QueryDoubleAttribute( "x", &x );
        camera_origin->QueryDoubleAttribute( "y", &y );
        camera_origin->QueryDoubleAttribute( "z", &z );
        camera_data.origin = { x, y, z };
    }

    if (auto camera_target = camera_root->FirstChildElement( "target" ); camera_target != nullptr )
    {
        double x, y, z;
        camera_target->QueryDoubleAttribute( "x", &x );
        camera_target->QueryDoubleAttribute( "y", &y );
        camera_target->QueryDoubleAttribute( "z", &z );
        camera_data.target = { x, y, z };
    }

    if (auto camera_vfov = camera_root->FirstChildElement( "vfov" ); camera_vfov != nullptr )
    {
        camera_vfov->QueryDoubleAttribute( "theta", &camera_data.vertical_fov );
    }

    if (auto camera_sampling = camera_root->FirstChildElement( "vfov" ); camera_sampling != nullptr )
    {
        camera_sampling->QueryIntAttribute( "rate", &camera_data.samplesPerPixel );
        camera_sampling->QueryIntAttribute( "depth", &camera_data.maxScatteringDepth );
    }


    Scene scene { camera_data };


    // Load the material data

    auto material_root = scene_root->FirstChildElement( "materials" );
    for ( auto curr_node = material_root->FirstChildElement("material");
        curr_node != nullptr; curr_node = curr_node->NextSiblingElement("material") )
    {
        std::string material_name = curr_node->Attribute("name");
        std::string material_type = curr_node->Attribute("type");

        auto const& read_colour = []( auto const& node ) -> Colour {
            auto colour_node = node->FirstChildElement("colour");
            float r = colour_node->FloatAttribute( "r" );
            float g = colour_node->FloatAttribute( "g" );
            float b = colour_node->FloatAttribute( "b" );
            return { r, g, b };
        };

        if ( material_type == "diffuse" )
        {
            Colour colour = read_colour( curr_node );
            scene.addMaterial<DiffuseMaterial>( material_name, colour );
        }
        else if ( material_type == "metallic" )
        {
            Colour colour = read_colour( curr_node );
            scene.addMaterial<MetalMaterial>( material_name, colour );
        }
        else
        {
            throw FormatException( std::format( "Unknown material type '{}'", material_type ) );
        }
    }


    // Load the object data

    auto object_root = scene_root->FirstChildElement( "objects" );
    for ( auto curr_node = object_root->FirstChildElement("object");
           curr_node != nullptr; curr_node = curr_node->NextSiblingElement("object") )
    {
        std::string object_type = curr_node->Attribute("type");

        auto const& read_origin = []( auto const& node ) -> Vec3d {
            auto origin_node = node->FirstChildElement( "origin" );
            double x = origin_node->DoubleAttribute( "x" );
            double y = origin_node->DoubleAttribute( "y" );
            double z = origin_node->DoubleAttribute( "z" );
            return { x, y, z };
        };

        auto const& read_material = []( auto const& node ) -> std::string {
            auto material_node = node->FirstChildElement( "material" );
            return material_node->Attribute("name");
        };

        if ( object_type == "sphere" )
        {
            Vec3d origin = read_origin( curr_node );
            std::string material_name = read_material( curr_node );

            auto size_node = curr_node->FirstChildElement( "size" );
            double radius = size_node->DoubleAttribute( "r" );

            scene.addObject<SphereObject>( material_name, origin, radius );
        }
        else
        {
            throw FormatException( std::format( "Unknown object type '{}'", object_type ) );
        }
    }


    return scene;
}

void Scene::render_to_file( std::string const &filename )
{
    auto image = m_camera.render(*this);
    image.save( filename );
}

std::optional<HitData> Scene::hit(Ray const &ray, Interval const& tValid) const
{
    std::optional<HitData> lastHit;

    double closestHit = tValid.max();

    for ( auto const& object : m_sceneObjects )
    {
        if (std::optional<HitData> currentHit = object->hit(ray, Interval(tValid.min(), closestHit)); currentHit.has_value())
        {
            lastHit = currentHit;
            closestHit = currentHit->t();
        }
    }

    return lastHit;
}

}
