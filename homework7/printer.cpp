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
	if(bulk.size() > 0) {
		printToStream(bulk, std::cout);
	}
}

void FilePrinter::print(CommandBulk& bulk) {
	if(bulk.size() > 0) {
		std::ofstream fout("bulk\\bulk" + std::to_string(bulk.getTimeCreated().count()) + ".txt");
		printToStream(bulk, fout);
	}
}
