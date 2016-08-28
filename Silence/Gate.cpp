
#include "Gate.h"
#include "Silence.h"

Gate::Gate() :
    exited(false),
    leave(false)
{
}

Gate::~Gate()
{
}

void Gate::create(LocalAssetManager * package, World& world, SceneManager * scenes)
{
    gate.setModel(package->newModel("data/models/gate/SecurityGate"));

    auto mat = glm::mat4(1.0);
    mat = glm::scale(mat, vec3(0.25, 0.25, 0.25));
    mat = glm::translate(mat, vec3(0.0, 90.0, 0.0));
    mat = glm::rotate(mat, -90.0f, vec3(0.0, -1.0, 0.0));
    mat = glm::translate(mat, vec3(-4050.0, 0.0, 0.0));
    
    world.onHit(gate.createbox(mat), [&](Camera *, SolidBox *) 
    {
        if (leave) 
        {
            exited = true;
            scenes->switchScene(static_cast<int>(SceneID::Gameover));
            leave = false;
        }
    });
}

bool Gate::hasHit()
{
    return exited;
}

void Gate::update(Key * key)
{
    leave = key->hasBeenPickedUp();
}

void Gate::render(ForwardRenderer& renderer)
{
    matrices.push();
    matrices.scale(vec3(0.25, 0.25, 0.25));
    matrices.translate(vec3(0.0, 90.0, 0.0));
    matrices.rotate(-90.0f, vec3(0.0, -1.0, 0.0));
    matrices.translate(vec3(-4100.0, 0.0, 0.0));
    
    renderer.setModelMatrix(matrices);
    renderer.renderModel(&gate);

    matrices.pop();
}

void Gate::reset()
{
}