#include <jagpdf\api.h>
#
using namespace jag;

int main() {
	pdf::Document doc(pdf::create_file("hello.pdf"));
	doc.page_start(597.6, 848.68);
	doc.page().canvas().move_to(10, 800);
	doc.page().canvas().line_to(590, 800);
	doc.page().canvas().path_paint("s");
	doc.page().canvas().text(50, 750, "Page1");
	doc.page_end();
	doc.finalize();
}