#include "scanreport.h"

using namespace Qlam;

ScanReport::ScanReport()
: m_outcome(Outcome::Unknown) {
}


void ScanReport::removeInfectedFile(const QString & path) {
    Q_UNUSED(path);
}


void ScanReport::removeInfectedFile(const InfectedFile & infection) {
    Q_UNUSED(infection);
}
