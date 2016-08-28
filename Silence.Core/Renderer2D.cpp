
#include "Renderer2D.h"

Renderer2D::Renderer2D()
{
}

Renderer2D::~Renderer2D()
{
}

void Renderer2D::createRenderer()
{
    shader.compile();
}

void Renderer2D::renderButtons(Button2D * buttons2D, int sz)
{
    for (auto i = 0; i < sz; i++)
    {
        renderButton(&buttons2D[i]);
    }
}

void Renderer2D::renderTexture(Texture2D * texture)
{
    if (texture != nullptr)
    {
        shader.prepare(texture->getDataID(), texture->getTextureID());
        shader.run(6);
    }
    else
    {
        throw Error(Component::Renderer, "NULL texture object was passed to the renderer", Author::William);
    }
}

void Renderer2D::setAlpha(float alpha)
{
    const auto program = shader.getProgram();
    program->setFloat("alpha", alpha);
}

void Renderer2D::renderLabel(Text2D* text)
{
    if (text != nullptr)
    {
        shader.prepare(text->getDataID(), text->getTextureID());
        shader.run(6);
    }
    else
    {
        throw Error(Component::Renderer, "NULL text object was passed to the renderer", Author::William);
    }
}

void Renderer2D::setMatrixForObject(const char * name, glm::mat4& mat)
{
    const auto program = shader.getProgram();
    program->setMatrix(const_cast<char *>(name), glm::value_ptr(mat));
}

void Renderer2D::renderButton(Button2D* button)
{
    const auto buttonTexture = button->getTexture();
    const auto buttonLabel = button->getText();

    if (buttonTexture != nullptr) 
    {
        renderTexture(buttonTexture);
    }

    if (buttonLabel != nullptr) 
    {
        renderLabel(buttonLabel);
    }
}

void Renderer2D::prepare()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    shader.getProgram()->bindShader();
}

void Renderer2D::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::present()
{
    shader.getProgram()->unbindShader();
    
    glDisable(GL_BLEND);
}