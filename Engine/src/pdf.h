#ifndef _PDF_H_
#define _PDF_H_

#include <string>
#include <vector>
#include <IrrIMGUI/IncludeIrrlicht.h>
#include <IrrIMGUI/IncludeIMGUI.h>
#include <IrrIMGUI/IrrIMGUI.h>
#include <IrrIMGUI/IrrIMGUIDebug.h>
#include "CGridSceneNode.h"
#include "NodeTree.h"
#include "jagpdf\api.h"

using std::string;
using std::vector;
using namespace jag;
class PDF
{
	public:
	PDF();
	bool createReport(irr::core::list<Node*> selectedNodes, const jag::pdf::Char* pdfName);
};

#endif
