#include "Functionality.h"

bool Functionality::isFound(irr::core::list<Node*>& selectedNodes, irr::scene::ISceneNode* selected)
{
	irr::core::list<Node*>::Iterator IT = selectedNodes.begin();
	for (; IT != selectedNodes.end(); IT++)
	{
		if (selected == (*IT)->getISceneNode())
		{
			return true;
		}
	}
	return false;
}

void Functionality::highlightHoveredNode(
	irr::core::list<Node*>& selectedNodes,
	irr::scene::ISceneNode *& selected,
	irr::scene::ISceneNode *& lastHovered,
	irr::video::ITexture* selectedNodeTexture,
	irr::video::ITexture* defaultNodeTexture,
	irr::video::ITexture* highlightSelectedNodeTexture,
	irr::video::ITexture* highlightDefaultNodeTexture,
	bool& isNodeHovered,
	irr::IrrlichtDevice *& device,
	irr::scene::ISceneCollisionManager *& CollMan
)
{
	if (selected = CollMan->getSceneNodeFromScreenCoordinatesBB(device->getCursorControl()->getPosition(), 0, true))
	{
		if (isFound(selectedNodes, selected))
		{
			if (lastHovered)
			{
				if (isFound(selectedNodes, lastHovered))
				{
					if (lastHovered != selected)
					{
						selected->setMaterialTexture(0, highlightSelectedNodeTexture);
						lastHovered->setMaterialTexture(0, selectedNodeTexture);
					}
				}
				else
				{
					if (lastHovered != selected)
					{
						selected->setMaterialTexture(0, highlightDefaultNodeTexture);
						lastHovered->setMaterialTexture(0, defaultNodeTexture);
					}
				}
			}
			lastHovered = selected;
			lastHovered->setMaterialTexture(0, highlightSelectedNodeTexture);
			isNodeHovered = true;
		}
		else
		{
			if (lastHovered)
			{
				if (isFound(selectedNodes, lastHovered))
				{
					if (lastHovered != selected)
					{
						selected->setMaterialTexture(0, highlightSelectedNodeTexture);
						lastHovered->setMaterialTexture(0, selectedNodeTexture);
					}
				}
				else
				{
					if (lastHovered != selected)
					{
						selected->setMaterialTexture(0, highlightDefaultNodeTexture);
						lastHovered->setMaterialTexture(0, defaultNodeTexture);
					}
				}
			}
			lastHovered = selected;
			lastHovered->setMaterialTexture(0, highlightDefaultNodeTexture);
			isNodeHovered = true;
		}
	}

	if (isNodeHovered)
	{
		if (!selected)
		{
			if (lastHovered)
			{
				if (isFound(selectedNodes, lastHovered))
				{
					lastHovered->setMaterialTexture(0, selectedNodeTexture);
				}
				else
				{
					lastHovered->setMaterialTexture(0, defaultNodeTexture);
				}

				isNodeHovered = false;
			}
		}
	}
}

irr::video::ITexture * Functionality::generatePlainColorTexture(irr::video::SColor color, irr::video::IVideoDriver * driver)
{
	irr::video::ITexture * texture = driver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(128, 128));
	driver->setRenderTarget(texture);
	driver->draw2DRectangle(color, irr::core::rect<irr::s32>(irr::core::position2d<irr::s32>(0, 0), irr::core::position2d<irr::s32>(128, 128)));
	driver->setRenderTarget(0);
	return texture;
}

