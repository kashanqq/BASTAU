/************************************************************
 * Headers
 ************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cmath>
#include "pdf.h"
#include "jagpdf\api.h"

/************************************************************
 * Using Declarations
 ************************************************************/

using std::ios;
using std::hex;
using std::setw;
using std::cout;
using std::endl;
using std::setfill;
using std::ofstream;
using std::ifstream;
using std::uppercase;
using std::ostringstream;
using namespace jag;
/************************************************************
 * Static Member Variables
 ************************************************************/

PDF::PDF()
{

}

bool PDF::createReport(irr::core::list<Node*> selectedNodes, const jag::pdf::Char* pdfName)
{
	int Y = 830;
	pdf::Document doc(pdf::create_file(pdfName));
	doc.page_start(597.6, 848.68);
	doc.page().canvas().move_to(10, 800);
	doc.page().canvas().line_to(590, 800);
	doc.page().canvas().path_paint("s");
/*irr::core::list<Node*>::Iterator IT = selectedNodes.begin();
	/*for (int i = 0; IT != selectedNodes.end(); IT++, i++)
	{
		doc.page().canvas().text(50, Y, "" + ((*IT)->data->unitID));
			(*IT)->data->unitName;
				(*IT)->data->headOfUnit;
				((*IT)->getParent() != nullptr ? (*IT)->getParent()->data->unitName : "0");
				std::to_string((*IT)->data->numStaff),
				std::to_string((int)(*IT)->data->annualBudget) + "$",
				std::to_string((int)(*IT)->data->expenses) + "$",
				std::to_string((int)(*IT)->data->income) + "$",
				std::to_string((int)(*IT)->data->loss) + "$",
				(*IT)->data->activityDesc,
				(*IT)->data->keyAchievements,
			
		Y -= 30;
	}*/
	doc.page_end();
	doc.finalize();
}
