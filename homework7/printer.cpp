#include <fstream>
#include "printer.h"

void Printer::printToStream(CommandBulk& bulk, std::ostream& out) {
	out << "bulk:";
	bool first = true;
	for(auto comm : bulk) {
		if(first) {
			first = false;
		}
		else {
			out << ',';
		}
		out << ' ' << comm;
	}
	out << std::endl;
}

void ConsolePrinter::print(CommandBulk& bulk) {
	printToStream(bulk, std::cout);
}

void FilePrinter::print(CommandBulk& bulk) {
	std::ofstream fout("bulk\\bulk" + std::to_string(bulk.getTimeCreated().count()) + ".txt");
	printToStream(bulk, fout);
}
