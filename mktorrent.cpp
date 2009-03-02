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

#include <unistd.h>
#include <QTextStream>
#include <QFileInfo>
#include <btversion.h>
#include <util/error.h>
#include <torrent/torrentcreator.h>
#include <math.h>

void usage(const char *name) {
	QTextStream(stdout) << "Usage: " << name << " -f target(s) [-o outfile] [-t tracker] [-w webseed] [-s chunk_size] [-n name] [-c comment] [-d] [-p] [-q]" << endl;
}

int main(int argc, char *argv[]) {
	int opt;
	QString target, name, comment="", output="";
	QStringList trackers;
	KUrl::List webseeds;
	unsigned int chunk_size = 512;
	bool priv=false, decentralized=false, quiet=false;

	while((opt=getopt(argc, argv, "f:t:w:s:n:c:dpqo:")) != -1) {
		switch(opt) {
			case 'f':
			target = optarg;
			break;
		case 't':
			trackers.push_back(optarg);
			break;
		case 'w':
			webseeds.push_back(KUrl(optarg));
			break;
		case 's':
			chunk_size = atoi(optarg);
			break;
		case 'n':
			name = optarg;
			break;
		case 'c':
			comment = optarg;
			break;
		case 'd':
			decentralized = true;
			break;
		case 'p':
			priv = true;
			break;
		case 'q':
			quiet = true;
			break;
		case 'o':
			output = optarg;
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

	if(output == "") {
		output = target + ".torrent";
	}

	if(name == "") {
		name = QFileInfo(target).fileName();
	}

	try {
		bt::SetClientInfo("KTorrent", bt::MAJOR, bt::MINOR, bt::RELEASE, bt::VERSION_TYPE, "KT");
		bt::TorrentCreator tc(target, trackers, webseeds, chunk_size, name, comment, priv, decentralized);

		for (int current = 1; !tc.calculateHash(); current++) {
			if (!quiet) {
				QTextStream(stdout) << "\r" << name << "\t\t" << 100 * current / tc.getNumChunks() << "%";
			}
		}
		tc.saveTorrent(output);
		if (!quiet) {
			QTextStream(stdout) << "\r" << name << "\t\t100%" << endl;
		}
	} catch (bt::Error &e) {
 		QTextStream(stderr) << e.toString() << endl;
		return 1;
	}

	return 0;
}
