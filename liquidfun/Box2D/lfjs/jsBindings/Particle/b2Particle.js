/// Water particle.
var b2_waterParticle = 0;
/// Removed after next simulation step.
var b2_zombieParticle = 1 << 1;
/// Zero velocity.
var b2_wallParticle = 1 << 2;
/// With restitution from stretching.
var b2_springParticle = 1 << 3;
/// With restitution from deformation.
var b2_elasticParticle = 1 << 4;
/// With viscosity.
var b2_viscousParticle = 1 << 5;
/// Without isotropic pressure.
var b2_powderParticle = 1 << 6;
/// With surface tension.
var b2_tensileParticle = 1 << 7;
/// Mix color between contacting particles.
var b2_colorMixingParticle = 1 << 8;
/// Call b2DestructionListener on destruction.
var b2_destructionListenerParticle = 1 << 9;
/// Prevents other particles from leaking.
var b2_barrierParticle = 1 << 10;
/// Less compressibility.
var b2_staticPressureParticle = 1 << 11;
/// Makes pairs or triads with other particles.
var b2_reactiveParticle = 1 << 12;
/// With high repulsive force.
var b2_repulsiveParticle = 1 << 13;
/// Call b2ContactListener when this particle is about to interact with
/// a rigid body or stops interacting with a rigid body.
/// This results in an expensive operation compared to using
/// b2_fixtureContactFilterParticle to detect collisions between
/// particles.
var b2_fixtureContactListenerParticle = 1 << 14;
/// Call b2ContactListener when this particle is about to interact with
/// another particle or stops interacting with another particle.
/// This results in an expensive operation compared to using
/// b2_particleContactFilterParticle to detect collisions between
/// particles.
var b2_particleContactListenerParticle = 1 << 15;
/// Call b2ContactFilter when this particle interacts with rigid bodies.
var b2_fixtureContactFilterParticle = 1 << 16;
/// Call b2ContactFilter when this particle interacts with other
/// particles.
var b2_particleContactFilterParticle = 1 << 17;

var b2ParticleHandle_index_offset = Offsets.b2ParticleHandle.index;

/** @constructor */
function b2ParticleColor(r, g, b, a) {
  if (r === undefined) {
    r = 0;
  }
  if (g === undefined) {
    g = 0;
  }
  if (b === undefined) {
    b = 0;
  }
  if (a === undefined) {
    a = 0;
  }
  this.r = r;
  this.g = g;
  this.b = b;
  this.a = a;
}

b2ParticleColor.prototype.Set = function(r, g, b, a) {
  this.r = r;
  this.g = g;
  this.b = b;
  this.a = a;
};

/**@constructor*/
function b2ParticleDef() {
  this.color = new b2Vec2();
  this.flags = 0;
  this.group = 0;
  this.lifetime = 0.0;
  this.position = new b2Vec2();
  this.userData = 0;
  this.velocity = new b2Vec2();
}

/**@constructor*/
function b2ParticleHandle(ptr) {
  this.ptr = ptr;
  this.buffer = new DataView(Module.HEAPU8.buffer, ptr);
}

b2ParticleHandle.prototype.GetIndex = function() {
  return this.buffer.getInt32(b2ParticleHandle_index_offset, true);
}

/**@constructor*/
function b2ParticleHandleGroup(particleSystem, particleHandles) {
  this.particleSystem = particleSystem;
  this.particleHandles = particleHandles.slice();
  this.count = particleHandles.length;
  
  // Create array of pointers that reference each row in the data
  var pointers = new Uint32Array(particleHandles.length);
  for (var i = 0; i < particleHandles.length; i++) {
    pointers[i] = particleHandles[i].ptr;
  }
  
  // Allocate bytes needed for the array of pointers
  var nPointerBytes = pointers.length * pointers.BYTES_PER_ELEMENT;
  this.ptr = Module._malloc(nPointerBytes);
  
  // Copy array of pointers to Emscripten heap
  var pointerHeap = new Uint8Array(Module.HEAPU8.buffer, this.ptr, nPointerBytes);
  pointerHeap.set(new Uint8Array(pointers.buffer));
}

b2ParticleHandleGroup.prototype.ApplyForce = function(force) {
  this.particleSystem._ParticleHandleGroupApplyForce(this, force);
}

b2ParticleHandleGroup.prototype.GetPosition = function() {
  return this.particleSystem._ParticleHandleGroupGetPosition(this);
}

b2ParticleHandleGroup.prototype.SetColor = function(r, g, b, a) {
  var colorBuffer = this.particleSystem.GetColorBuffer();
  for (i = 0; i < this.particleHandles.length; i++) {
    var index = this.particleHandles[i].GetIndex() * 4;
    colorBuffer[index] = r;
    colorBuffer[index + 1] = g;
    colorBuffer[index + 2] = b;
    colorBuffer[index + 3] = a;
  }
}

b2ParticleHandleGroup.prototype.Destroy = function() {
  Module._free(this.ptr);
}
