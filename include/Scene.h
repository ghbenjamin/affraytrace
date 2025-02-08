#pragma once

#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

#include "SceneObject.h"
#include "Material.h"
#include "Camera.h"


namespace affraytrace
{



template <typename T>
concept IsSceneObject = std::is_base_of_v<SceneObject, T>;

template <typename T>
concept IsMaterial = std::is_base_of_v<Material, T>;

class Scene
{
public:
    Scene( CameraData const& camera_data );

    static std::variant<Scene, std::exception> load_scene( std::string const& filename );

    template <typename T, typename... Args> requires IsSceneObject<T>
    void addObject( std::string const& material_name, Args&&... args)
    {
        Material* material_ptr = m_materials.at(material_name).get();

        std::unique_ptr<SceneObject> obj = std::make_unique<T>( material_ptr, std::forward<Args>(args)... );
        m_sceneObjects.push_back( std::move(obj) );
    }

    template <typename T, typename... Args> requires IsMaterial<T>
        void addMaterial( std::string const& name, Args&&... args)
    {
        m_materials.emplace( name, std::make_unique<T>( std::forward<Args>(args)... ));
    }



    void render_to_file( std::string const& filename );

    [[nodiscard]]
    std::optional<HitData> hit( Ray const& ray, Interval const& tValid ) const;


private:

    std::shared_ptr<Random> m_randomState;
    Camera m_camera;
    std::unordered_map<std::string, std::unique_ptr<Material>> m_materials;
    std::vector<std::unique_ptr<SceneObject>> m_sceneObjects;
};

}