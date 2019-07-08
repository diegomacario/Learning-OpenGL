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
                const glm::vec3&              scalingFactors);
   virtual ~GameObject3D() = default;

   GameObject3D(const GameObject3D&) = default;
   GameObject3D& operator=(const GameObject3D&) = default;

   GameObject3D(GameObject3D&& rhs) noexcept;
   GameObject3D& operator=(GameObject3D&& rhs) noexcept;

   glm::mat4 getModelMatrix() const;

private:

   std::shared_ptr<Model> mModel;

   glm::vec3              mPosition;
   float                  mAngleOfRotInDeg;
   glm::vec3              mAxisOfRot;
   glm::vec3              mScalingFactors;
   glm::mat4              mModelMat;

   void calculateModelMatrix();
};

#endif
