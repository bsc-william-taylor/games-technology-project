
#include "Trees.h"

Trees::Trees()
{
}

Trees::~Trees()
{
}

void Trees::setup(LocalAssetManager * package, Heightmap * map, World& world)
{
	auto tree = package->getM("data/models/tree/tree1a_lod0");
	TextureAsset * texture = package->getT("data/textures/pathways");
	treeModels.setModel(tree);

	for (int x = 0; x < texture->getWidth(); x++) {
		for (int y = 0; y < texture->getHeight(); y++) {
			RGBQUAD colour = texture->getPixelColour(x, y);

			if (((GLfloat)colour.rgbGreen) >= 200) {
				int index = matrices.size();

				int _x = 64.0 - x;
				int _y = y - 64.0;

				int sc = (rand() % 5);

				matrices.push_back(glm::mat4());
				matrices[index] = glm::translate(matrices[index], glm::vec3(-_x, map->getHeightAt(x, y), _y) * glm::vec3(16, 1, 16));
				matrices[index] = glm::scale(matrices[index], glm::vec3(5.0 + sc, 5.0 + (rand() % 5), 5.0 + sc));

				world.onHit(treeModels.createCylinder(matrices[index], 10.0), [](Camera * c, SolidCylinder * s) {
					c->cancelMovement();
				});
			}
		}
	}

	std::cout << "Trees : " << matrices.size() << std::endl;
	treeModels.setMatrices({ matrices });
}

void Trees::render(ForwardRenderer& renderer) 
{
	renderer.renderModelSet(&treeModels);
}