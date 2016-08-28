
#include "Silence.h"
#include <iomanip>

std::string decimalToString(double value, int decimals)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(decimals) << value;
    auto s = ss.str();

    if (decimals > 0 && s[s.find_last_not_of('0')] == '.') 
    {
        s.erase(s.size() - decimals + 1);
    }

    return s;
}

Gameover::Gameover(OperatingSystem * system)
    : survivorTimer(nullptr)
{
    package = system->acquireAssetManager()->grabLocalManager();
    package->grab({
        "data/textures/button.png",
        "data/textures/menu3.png",
        "data/textures/box.png"
    });

    scenes = system->acquireSceneManager();
}

Gameover::~Gameover()
{
    SAFE_RELEASE(package);
}

void Gameover::onEnter(int i)
{
    auto forestScene = static_cast<Forest *>(scenes->getScene(int(SceneID::Forest)));
    survivorTimer = forestScene->getForestTime();
    timeSurvived = "Time Survived : " + decimalToString(survivorTimer->elapsed(Seconds), 2).append("s");
}

void Gameover::onCreate()
{
    background.setTexture(package->newTexture("data/textures/menu3"));
    background.setArea(glm::vec4(0, 0, 1920, 1080));
    
    boxart.setTexture(package->newTexture("data/textures/box"));
    boxart.setArea(glm::vec4(1100, 220, 700, 470));

    header.setFont(package->newFont("data/fonts/Calibri", 200, { 255, 255, 255 }), "Thanks for Playing");
    header.setArea(vec2(1920 / 2, 900), Alignment::Center);

    survivorTimer = static_cast<Forest *>(scenes->getScene(int(SceneID::Forest)))->getForestTime();
    auto timeSurvived = "Time Survived : " + decimalToString(survivorTimer->elapsed(Seconds), 2).append("s");

    time.setFont(package->newFont("data/fonts/Calibri", 50, { 255, 255, 255 }), timeSurvived.c_str());
    time.setArea(vec2(200, 550), Alignment::Right);

    message.setFont(package->newFont("data/fonts/Calibri", 50, { 255, 255, 255 }), "Look for the full game sometime soon");
    message.setArea(vec2(200, 450), Alignment::Right);

    back.setButtonTexture(package->newTexture("data/textures/button"));
    back.setButtonText(package->newFont("data/fonts/Calibri", 40, { 255, 255, 255 }), "Back to Menu");
    back.setArea(glm::vec4(400, 200, 400, 225), Alignment::Center);

    renderer2D.createRenderer();
}

void Gameover::onGamepadButton(int key, int state)
{
    if (key == SDL_CONTROLLER_BUTTON_A && state == GamepadButtonPressed)
    {
        scenes->switchScene(int(SceneID::Menu));
    }

    if (key == SDL_CONTROLLER_BUTTON_B && state == GamepadButtonPressed)
    {
        scenes->switchScene(int(SceneID::Menu));
    }
}

void Gameover::onGameEvent(SDL_Event& e)
{
    if (back.isPressed(e))
    {
        scenes->switchScene(int(SceneID::Menu));
    }
}

void Gameover::onUpdate()
{
    time.setFont(package->newFont("data/fonts/Calibri", 50, { 255, 255, 255 }), timeSurvived.c_str());
    time.setArea(vec2(200, 550), Alignment::Right);
}

void Gameover::onRender()
{
    auto projectionMatrix = glm::mat4();
    auto modelMatrix = glm::mat4();

    projectionMatrix = glm::ortho(0.0, 1920.0, 0.0, 1080.0);

    renderer2D.prepare();
    renderer2D.setAlpha(1.0f);
    renderer2D.setMatrixForObject("projection", projectionMatrix);
    renderer2D.setMatrixForObject("model", modelMatrix);
    renderer2D.renderTexture(&background);
    renderer2D.renderTexture(&boxart);
    renderer2D.renderLabel(&message);
    renderer2D.renderLabel(&header);
    renderer2D.renderLabel(&time);
    renderer2D.renderButton(&back);
    renderer2D.present();
}