/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include <QTextStream>
#include <QFileInfo>
#include <btversion.h>
#include <util/error.h>
#include <torrent/torrentcreator.h>

void usage(const char *name) {
	QTextStream(stdout) << "Usage: " << name << " -f target [-n] [-h] [-s]" << endl;
}

int main(int argc, char *argv[]) {
	int opt;
	QString target;
	bool print_all=true, print_hash=false, print_name=false, print_size=false;

	while((opt=getopt(argc, argv, "f:nhs")) != -1) {
		switch(opt) {
			case 'f':
			target = optarg;
			break;
		case 'n':
			print_name = true;
			print_all = false;
			break;
		case 'h':
			print_hash = true;
			print_all = false;
			break;
		case 's':
			print_size = true;
			print_all = false;
			break;
		default:
			usage(argv[0]);
			return -1;
		}
	}

	if(target == "") {
		usage(argv[0]);
		return -1;
	}

	try {
		bt::SetClientInfo("KTorrent", bt::MAJOR, bt::MINOR, bt::RELEASE, bt::VERSION_TYPE, "KT");
		bt::Torrent t;
		t.load(target, false);

		if (print_all) {
			QTextStream(stdout) << "FileName:\t\t" << QFileInfo(target).fileName() << endl;
			QTextStream(stdout) << "Name:\t\t" << t.getNameSuggestion() << endl;
			QTextStream(stdout) << "Size:\t\t" << t.getFileLength() << endl;
			if (t.isMultiFile()) {
				QTextStream(stdout) << "Files:\t" << t.getNumFiles() << endl;
			}
			QTextStream(stdout) << "VHT:\t\t" << (t.isPrivate() ? "private" : "enabled") << endl;
			QTextStream(stdout) << "InfoHash:\t" << t.getInfoHash().toString() << endl;
			QTextStream(stdout) << "ChunkSize:\t" << t.getChunkSize() << endl;
			QTextStream(stdout) << "Chunks:\t\t" << t.getNumChunks() << endl;
			QTextStream(stdout) << "Tracker:\t" << t.getTrackerList()->urls.toStringList().join("\n\t\t") << endl;
			QTextStream(stdout) << "WebSeeds:\t" << t.getWebSeeds().toStringList().join("\n\t\t") << endl;
			if (t.getNumDHTNodes() > 0) {
				QTextStream(stdout) << "DHTNodes:\t" << t.getNumDHTNodes() << endl;
			}
		}
		else if (print_name) {
			QTextStream(stdout) << t.getNameSuggestion() << endl;
		}
		else if (print_size) {
			QTextStream(stdout) << t.getFileLength() << endl;
		}
		else if (print_hash) {
			QTextStream(stdout) << t.getInfoHash().toString() << endl;
		}
	} catch (bt::Error &e) {
		QTextStream(stderr) << e.toString() << endl;
		return 1;
	}

	return 0;
}
