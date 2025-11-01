#pragma once

#include <IrrIMGUI/IncludeIrrlicht.h>
#include "Node.h"
#ifndef FUNCTIONALITY_H
#define FUNCTIONALITY_H

struct Functionality
{
	static bool isFound(irr::core::list<Node*>& selectedNodes, irr::scene::ISceneNode* selected);
	static void highlightHoveredNode(
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
	);
	static irr::video::ITexture * generatePlainColorTexture(irr::video::SColor color, irr::video::IVideoDriver * driver);
};

#endif // !FUNCTIONALITY_H
