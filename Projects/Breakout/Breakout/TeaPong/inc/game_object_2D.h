#ifndef GAME_OBJECT_2D_H
#define GAME_OBJECT_2D_H

#include <glm/glm.hpp>

#include <memory>

#include "texture.h"

class GameObject2D
{
public:

   GameObject2D(const std::shared_ptr<Texture>& texture,
                const glm::vec2&                posOfTopLeftCornerInPix,
                float                           angleOfRotInDeg,
                float                           widthInPix,
                float                           heightInPix);
   virtual ~GameObject2D() = default;

   GameObject2D(const GameObject2D&) = default;
   GameObject2D& operator=(const GameObject2D&) = default;

   GameObject2D(GameObject2D&& rhs) noexcept;
   GameObject2D& operator=(GameObject2D&& rhs) noexcept;

   std::shared_ptr<Texture> getTexture() const;
   // TODO: Remove?
   /*
   glm::vec2                getPosOfTopLeftCornerInPix() const;
   float                    getAngleOfRotInDeg() const;
   float                    getWidthInPix() const;
   float                    getHeightInPix() const;
   */
   glm::mat4                getModelMatrix() const;

private:

   std::shared_ptr<Texture> mTexture;

   glm::vec2                mPosOfTopLeftCornerInPix;
   float                    mAngleOfRotInDeg;
   float                    mWidthInPix;
   float                    mHeightInPix;
   glm::mat4                mModelMat;

   void calculateModelMatrix();
};

#endif
