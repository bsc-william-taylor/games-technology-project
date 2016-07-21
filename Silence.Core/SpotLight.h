
#include "EngineLayer.h"
#include "GPU.h"

class SILENCE_EXPORT SpotLight
{
private:
    glm::vec3 spotDirection;
    glm::vec3 spotPosition;
    glm::vec3 spotColour;

    bool spotLightActive;

    float linearAttrib;
    float coneCosine;
public:
    SpotLight();
    ~SpotLight();

    void setDirection(glm::vec3 direction);
    void setPosition(glm::vec3 position);
    void setColour(glm::vec3 colour);

    void setAttribuation(float attr);
    void setConeAngle(float angle);
    void send(GPU_Program * p);
    void toggle();
    void turnOff();
    void turnOn();
};