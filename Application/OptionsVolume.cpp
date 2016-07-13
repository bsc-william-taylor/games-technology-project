
#include "OptionsVolume.h"

OptionsVolume::OptionsVolume()
{
	colour = { 255, 255, 255 };
	volume = 0;
}

OptionsVolume::~OptionsVolume()
{

}

void OptionsVolume::onGamepadButton(int key, int state, int elementID)
{
	if (elementID == 4)
	{
		colour = { 255, 0, 0 };

		if (key == SDL_CONTROLLER_BUTTON_DPAD_LEFT && state == GAMEPAD_BUTTON_PRESSED && volume >= 5) {
			volume -= 5;

			BASS_SetVolume((double)(volume / 100.0));
		}

		if (key == SDL_CONTROLLER_BUTTON_DPAD_RIGHT && state == GAMEPAD_BUTTON_PRESSED && volume <= 95) {
			volume += 5;

			BASS_SetVolume((double)(volume / 100.0));
		}
	}
	else
	{
		colour = { 255, 255, 255 };
	}
}

void OptionsVolume::create(LocalAssetManager * package)
{
	this->package = package;

	volume = (int)(BASS_GetVolume() * 100.0f);

	while (volume % 5 != 0) {
		++volume;
		if (volume > 100) break;
	}

	BASS_SetVolume((float)(volume / 100.0));

	volumeSize.setFont(package->getL("data/fonts/Calibri", 60, { 255, 255, 255 }), "Game Volume");
	volumeSize.setArea(glm::vec2(650, 250), ALIGNMENT::CENTER);

	volumeSizeLabel.setFont(package->getL("data/fonts/Calibri", 60, colour ), std::to_string(volume).append("%").c_str());
	volumeSizeLabel.setArea(glm::vec2(1250, 250), ALIGNMENT::CENTER);

	volumeSizeButtons[0].setButtonTexture(package->getT("data/textures/left"));
	volumeSizeButtons[0].setArea(glm::vec4(1250 - 300, 200, 70, 100), ALIGNMENT::CENTER);
	volumeSizeButtons[1].setButtonTexture(package->getT("data/textures/right"));
	volumeSizeButtons[1].setArea(glm::vec4(1250 + 250, 200, 70, 100), ALIGNMENT::CENTER);
}

void OptionsVolume::render(Renderer2D * renderer)
{
	renderer->renderButtons(volumeSizeButtons, 2);
	renderer->renderLabel(&volumeSizeLabel);
	renderer->renderLabel(&volumeSize);
}

void OptionsVolume::event(SDL_Event& e)
{
	if (volumeSizeButtons[0].isPressed(e) && volume >= 5)
	{
		volume -= 5;

		BASS_SetVolume((double)(volume / 100.0));
	}

	if (volumeSizeButtons[1].isPressed(e) && volume <= 95)
	{
		volume += 5;

		BASS_SetVolume((double)(volume / 100.0));
	}
}

void OptionsVolume::update()
{
	volumeSizeLabel.setFont(package->getL("data/fonts/Calibri", 60, colour), std::to_string(volume).append("%").c_str());
	volumeSizeLabel.setArea(glm::vec2(1250, 250), ALIGNMENT::CENTER);
}