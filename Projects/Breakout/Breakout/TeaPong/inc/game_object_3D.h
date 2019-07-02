#ifndef GAME_OBJECT_3D_H
#define GAME_OBJECT_3D_H

#include <glm/glm.hpp>

#include <memory>

#include "model.h"

class GameObject3D
{
public:

   GameObject3D(const std::shared_ptr<Model>& model,
                const glm::vec3&              position,
                float                         angleOfRotation,
                const glm::vec3&              axisOfRotation,
                const glm::vec3&              scaling);
   ~GameObject3D() = default; // TODO: Should the destructor be virtual?

   GameObject3D(const GameObject3D&) = default;
   GameObject3D& operator=(const GameObject3D&) = default;

   GameObject3D(GameObject3D&& rhs) noexcept;
   GameObject3D& operator=(GameObject3D&& rhs) noexcept;

private:

   std::shared_ptr<Model> mModel;

   glm::vec3 mPosition;
   float     mAngleOfRotation;
   glm::vec3 mAxisOfRotation;
   glm::vec3 mScaling;
};

#endif
