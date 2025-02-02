#pragma once

#include <memory>
#include <vector>

#include "SceneObject.h"


namespace affraytrace
{


template <typename T>
concept IsSceneObject = std::is_base_of_v<SceneObject, T>;


class Scene
{
public:
    Scene() = default;

    template <typename T, typename... Args> requires IsSceneObject<T>
    void addObject( Args&&... args)
    {
        std::unique_ptr<SceneObject> obj = std::make_unique<T>( std::forward<Args>(args)... );
        m_sceneObjects.push_back( std::move(obj) );
    }

    [[nodiscard]]
    std::optional<HitData> hit( Ray const& ray, Interval const& tValid ) const;

private:
    std::vector<std::unique_ptr<SceneObject>> m_sceneObjects;

};

}