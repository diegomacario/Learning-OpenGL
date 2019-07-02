#ifndef MOVABLE_GAME_OBJECT_2D_H
#define MOVABLE_GAME_OBJECT_2D_H

#include "game_object_2D.h"

class MovableGameObject2D : public GameObject2D
{
public:

   MovableGameObject2D(const std::shared_ptr<Texture>& texture,
                       const glm::vec2&                position,
                       float                           angleOfRotation,
                       const glm::vec2&                scaling,
                       const glm::vec2&                velocity);
   ~MovableGameObject2D() = default; // TODO: Should the destructor be virtual?

   MovableGameObject2D(const MovableGameObject2D&) = default;
   MovableGameObject2D& operator=(const MovableGameObject2D&) = default;

   MovableGameObject2D(MovableGameObject2D&& rhs) noexcept;
   MovableGameObject2D& operator=(MovableGameObject2D&& rhs) noexcept;

private:

   glm::vec2 velocity; // TODO: Should this be divided into speed and direction?
};

#endif
