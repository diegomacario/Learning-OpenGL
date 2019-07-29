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
                float                         angleOfRotInDeg,
                const glm::vec3&              axisOfRot,
                float                         scalingFactor);
   ~GameObject3D() = default;

   GameObject3D(const GameObject3D&) = default;
   GameObject3D& operator=(const GameObject3D&) = default;

   GameObject3D(GameObject3D&& rhs) noexcept;
   GameObject3D& operator=(GameObject3D&& rhs) noexcept;

   void      render(const Shader& shader) const;

   glm::mat4 getModelMatrix() const; // TODO: Is this necessary for 3D objects? It makes sense for 2D objects because they use the 2D renderer, but I don't think it's necessary here.

private:

   void      calculateModelMatrix();

   std::shared_ptr<Model> mModel;

   glm::vec3              mPosition;
   float                  mAngleOfRotInDeg;
   glm::vec3              mAxisOfRot;
   float                  mScalingFactor;
   glm::mat4              mModelMatrix;
};

#endif
