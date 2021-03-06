#include<Box2D/Box2D.h>
#include <stdio.h>

extern "C" {
  extern bool b2ParticleSystemQuery(const void* particleSystem, double index);
}

class ParticleQueryAABBCallback : public b2QueryCallback {
public:
  bool ReportFixture(b2Fixture* fixture) {
    return false;
  }
    
  bool ReportParticle(const b2ParticleSystem* particleSystem, int32 index) {
    return b2ParticleSystemQuery(particleSystem, index);
  }
};

ParticleQueryAABBCallback particleQueryAABBCallback;

class ParticleQueryShapeCallback : public b2QueryCallback {
public:
  ParticleQueryShapeCallback(const b2Shape* m_shape, const b2Transform* m_transform) :
    m_shape(m_shape), m_transform(m_transform) {}
  
  bool ReportFixture(b2Fixture* fixture) {
    return false;
  }
  
  bool ReportParticle(const b2ParticleSystem* particleSystem, int32 index) {
    if (m_shape->TestPoint(*m_transform, particleSystem->GetPositionBuffer()[index])) {
      return b2ParticleSystemQuery(particleSystem, index);
    }
    
    return true;
  }
  
private:
  const b2Shape* m_shape;
  const b2Transform* m_transform;
};

double b2ParticleSystem_CreateParticle(void* particleSystem,
    //particleDef
    double colorR, double colorB, double colorG, double colorA,
    double flags, double group, double lifetime, double positionX,
    double positionY, double userData, double velocityX, double velocityY) {
  b2ParticleDef def;
  def.color = b2ParticleColor(colorR, colorG, colorB, colorA);
  def.flags = flags;
  def.group = (b2ParticleGroup*)&group;
  def.lifetime = lifetime;
  def.position = b2Vec2(positionX, positionY);
  def.userData = (double*)&userData;
  def.velocity = b2Vec2(velocityX, velocityY);

  return ((b2ParticleSystem*)particleSystem)->CreateParticle(def);
}

const void* b2ParticleSystem_GetParticleHandleFromIndex(void* particleSystem, double index) {
  return ((b2ParticleSystem*)particleSystem)->GetParticleHandleFromIndex((int)index);
}

// Shapes array is not currently supported for b2ParticleSystems

void* b2ParticleSystem_GetColorBuffer(void* particleSystem) {
  return ((b2ParticleSystem*)particleSystem)->GetColorBuffer();
}
double b2ParticleSystem_GetParticleCount(void* particleSystem) {
  return ((b2ParticleSystem*)particleSystem)->GetParticleCount();
}

double b2ParticleSystem_GetParticleLifetime(void* particleSystem, double index){
  return ((b2ParticleSystem*)particleSystem)->GetParticleLifetime((int)index);
}

void* b2ParticleSystem_GetPositionBuffer(void* particleSystem) {
  return ((b2ParticleSystem*)particleSystem)->GetPositionBuffer();
}
void* b2ParticleSystem_GetVelocityBuffer(void* particleSystem) {
  return ((b2ParticleSystem*)particleSystem)->GetVelocityBuffer();
}
void b2ParticleSystem_SetDamping(void* particleSystem, double damping) {
  ((b2ParticleSystem*)particleSystem)->SetDamping(damping);
}
void b2ParticleSystem_SetDensity(void* particleSystem, double density) {
  ((b2ParticleSystem*)particleSystem)->SetDensity(density);
}

void b2ParticleSystem_SetGravityScale(void* particleSystem, double gravityScale) {
  ((b2ParticleSystem*)particleSystem)->SetGravityScale(gravityScale);
}

void b2ParticleSystem_SetMaxParticleCount(void* particleSystem, double count) {
  ((b2ParticleSystem*)particleSystem)->SetMaxParticleCount((int)count);
}

void b2ParticleSystem_SetParticleLifetime(void* particleSystem, double index, double lifetime){
  ((b2ParticleSystem*)particleSystem)->SetParticleLifetime((int)index, lifetime);
}

void b2ParticleSystem_SetRadius(void* particleSystem, double radius) {
  ((b2ParticleSystem*)particleSystem)->SetRadius(radius);
}

void b2ParticleSystem_QueryAABB(
    void* particleSystem,
    double aabbLowerBoundX, double aabbLowerBoundY,
    double aabbUpperBoundX, double aabbUpperBoundY) {
  b2AABB aabb;
  aabb.lowerBound = b2Vec2(aabbLowerBoundX, aabbLowerBoundY);
  aabb.upperBound = b2Vec2(aabbUpperBoundX, aabbUpperBoundY);
  ((b2ParticleSystem*)particleSystem)->QueryAABB(&particleQueryAABBCallback, aabb);
}

void b2ParticleSystem_QueryShapeAABB(void* particleSystem, void* shape,
                                     void* xf) {
  ((b2ParticleSystem*)particleSystem)->QueryShapeAABB(
      &particleQueryAABBCallback,
      *((b2Shape*)shape),
      *((b2Transform*)xf));
}

void b2ParticleSystem_QueryShape(void* particleSystem, void* shape, void* xf) {
  ParticleQueryShapeCallback callback = ParticleQueryShapeCallback(
      (b2Shape*)shape,
      (b2Transform*)xf);
  ((b2ParticleSystem*)particleSystem)->QueryShapeAABB(
      &callback,
      *((b2Shape*)shape),
      *((b2Transform*)xf));
}

void b2ParticleSystem_ParticleHandlesApplyForce(
    void* particleSystem,
    void** particleHandles, double nParticles,
    double forceX, double forceY) {
  forceX /= nParticles;
  forceY /= nParticles;
  for (int i = 0; i < (int) nParticles; ++i) {
    b2ParticleHandle* particleHandle = ((b2ParticleHandle**)particleHandles)[i];
    ((b2ParticleSystem*)particleSystem)->ParticleApplyForce(particleHandle->GetIndex(), b2Vec2(forceX, forceY));
  }
}

void b2ParticleSystem_ParticleHandlesGetPosition(
    void* particleSystem,
    void** particleHandles, double nParticles,
    float* arr) {
  float sumX = 0.0;
  float sumY = 0.0;
  b2Vec2* positionBuffer = ((b2ParticleSystem*)particleSystem)->GetPositionBuffer();
  for (int i = 0; i < (int) nParticles; ++i) {
    b2ParticleHandle* particleHandle = ((b2ParticleHandle**)particleHandles)[i];
    int particleIndex = particleHandle->GetIndex();
    sumX += positionBuffer[particleIndex].x;
    sumY += positionBuffer[particleIndex].y;
  }
  arr[0] = sumX / nParticles;
  arr[1] = sumY / nParticles;
}
