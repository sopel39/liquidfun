#ifndef B2PARTICLESYSTEMJSBINDINGS
#define B2PARTICLESYSTEMJSBINDINGS
extern "C" {
double b2ParticleSystem_CreateParticle(void* particleSystem,
    //particleDef
    double colorR, double colorB, double colorG, double colorA,
    double flags, double group, double lifetime, double positionX,
    double positionY, double userData, double velocityX, double velocityY);

    const void* b2ParticleSystem_GetParticleHandleFromIndex(void* particleSystem, double index);
    void* b2ParticleSystem_GetColorBuffer(void* particleSystem);
    double b2ParticleSystem_GetParticleCount(void* particleSystem);
    double b2ParticleSystem_GetParticleLifetime(void* particleSystem, double index);
    void* b2ParticleSystem_GetPositionBuffer(void* particleSystem);
    void* b2ParticleSystem_GetVelocityBuffer(void* particleSystem);
    
    void b2ParticleSystem_SetDamping(void* particleSystem, double damping);
    void b2ParticleSystem_SetDensity(void* particleSystem, double density);
    void b2ParticleSystem_SetGravityScale(void* particleSystem, double gravityScale);
    void b2ParticleSystem_SetMaxParticleCount(void* particleSystem, double count);
    void b2ParticleSystem_SetParticleLifetime(void* particleSystem, double index, double lifetime);
    void b2ParticleSystem_SetRadius(void* particleSystem, double radius);
  
    void b2ParticleSystem_QueryAABB(
        void* particleSystem,
        double aabbLowerBoundX, double aabbLowerBoundY,
        double aabbUpperBoundX, double aabbUpperBoundY);
    void b2ParticleSystem_QueryShapeAABB(void* particleSystem, void* shape, void* xf);
    
    void b2ParticleSystem_ParticleHandlesApplyForce(
        void* particleSystem,
        void** particleHandles, double nParticles,
        double forceX, double forceY);
    void b2ParticleSystem_ParticleHandlesGetPosition(
        void* particleSystem,
        void** particleHandles, double nParticles,
        float* arr);
}
#endif
