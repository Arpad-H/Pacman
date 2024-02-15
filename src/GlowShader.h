#ifndef GlowShader_hpp
#define GlowShader_hpp

#include <stdio.h>
#include <assert.h>
#include "PhongShader.h"


class GlowShader : public PhongShader
{
public:
    enum {
        DETAILTEX0=0,
        DETAILTEX1,
        DETAILTEX2,
        DETAILTEX3,
        DETAILTEX_COUNT
    };
    
    GlowShader(const std::string& AssetDirectory);
    virtual ~GlowShader() {}
    virtual void activate(const BaseCamera& Cam) const;
    virtual void deactivate() const;
    
    const Texture* detailTex(unsigned int idx) const { assert(idx<DETAILTEX_COUNT); return DetailTex[idx]; }
    const Texture* mixTex() const { return MixTex; }

    void detailTex(unsigned int idx, const Texture* pTex) { assert(idx<DETAILTEX_COUNT); DetailTex[idx] = pTex; }
    void mixTex(const Texture* pTex) { MixTex = pTex; }

    void scaling(const Vector& s) { Scaling = s; }
    const Vector& scaling() const { return Scaling; }

private:
    void activateTex(const Texture* pTex, GLint Loc, int slot) const;

    const Texture* MixTex;
    const Texture* DetailTex[DETAILTEX_COUNT];
    Vector Scaling;
    int FactorK = 100;
    // shader locations
    GLint MixTexLoc;
    GLint DetailTexLoc[DETAILTEX_COUNT];
    GLint ScalingLoc;
    GLint FactorKLoc;
};

#endif /* GlowShader_hpp */
