// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

/**
 * The collision detection algorithm was inspired largely by the following sources
 * ===========================================================================
 * - https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/565282#565282
 *   post of Gareth
 * - https://gist.github.com/hamaluik/e69f96e253a190273bf0
 *   AABB.hx of Kenton Hamaluik
 * - https://github.com/kikito/bump.lua
 *   bump.lua, copyright (c) 2014 Enrique García Cota
 * - https://box2d.org/
 *   (c) by Erin Catto
*/

#pragma once

#include "Vector2T.hpp"
#include "RectT.hpp"
#include "layer/SprLayer.hpp"
#include "CollisionSolver.hpp"

#include <forward_list>
#include <unordered_set>


namespace dang
{
    class ColSpr;
    using spColSpr = std::shared_ptr<ColSpr>;
    class SpriteObject;
    using spSprObj = std::shared_ptr<SpriteObject>;

    class ColSprLayer : public SprLayer
    {
    public:
        ColSprLayer();
        explicit ColSprLayer(const tmx_layer* l);
        ~ColSprLayer() override = default;

        virtual void        addSprite(spColSpr s);

        // override from SprLayer
        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

        void    resetZoneBit(const RectF& vp) override;
        void    cleanSpritelist() override;

        float   aaLoSH(const ColSpr& me, const ColSpr& target);
        float   loS(const ColSpr& me, const ColSpr& target);

    protected:
        CollisionSolver _cs;

        void coreUpdate(uint32_t dt, const Gear &gear) override;

    private:
        // may not be used in this layer type, use with param ColSpr instead
        void        addSprite(spSprObj s) override {};
#ifdef DANG_DEBUG_DRAW
        int _dbg_mem{0};
#endif
    };

}


