#include "settings.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QUrl>

#include "application.h"

using namespace Qlam;

Settings::Settings( QObject * parent )
: QObject(parent),
  m_filePath(),
  m_dbPath(),
  m_updateServerType(OfficialMirror),
  m_updateMirror(),
  m_customUpdateServer(),
  m_modified(false) {
	construct();
}


Settings::Settings( const QString & filePath, QObject * parent )
: QObject(parent),
  m_filePath(filePath),
  m_dbPath(),
  m_updateServerType(OfficialMirror),
  m_updateMirror(),
  m_customUpdateServer(),
  m_modified(false) {
	construct();
}


void Settings::construct( void ) {
	load();
	connect(this, SIGNAL(databasePathChanged( QString )), this, SIGNAL(changed()));
	connect(this, SIGNAL(updateServerTypeChanged( UpdateServerType )), this, SIGNAL(changed()));
	connect(this, SIGNAL(updateMirrorChanged( QString )), this, SIGNAL(changed()));
	connect(this, SIGNAL(customUpdateServerChanged( QString )), this, SIGNAL(changed()));
	connect(this, SIGNAL(customUpdateServerChanged( QUrl )), this, SIGNAL(changed()));
}


bool Settings::setUpdateMirror( const QString & mirror ) {
	if(mirror != m_updateMirror) {
		/* TODO validate mirror */
		m_updateMirror = mirror;
		m_modified = true;
		Q_EMIT(updateMirrorChanged(mirror));
	}

	return true;
}

QUrl Settings::updateServer( void ) const {
	if(Custom == updateServerType()) {
		return customUpdateServer();
	}

	return QUrl(QString("http://db.%1.clamav.net").arg(updateMirror()));
}


void Settings::load( void ) {
	QSettings * s = nullptr;

	if(m_filePath.isEmpty()) {
		qDebug() << "loading settings from default path";
		s = new QSettings(QSettings::IniFormat, QSettings::UserScope, qlamApp->organizationName(), qlamApp->applicationName() + ".appconfig");
	}
	else {
		qDebug() << "loading settings from" << m_filePath;
		s = new QSettings(m_filePath, QSettings::IniFormat);
	}

	setDatabasePath(s->value("database.path", QString()).toString());
	setUpdateServerType(stringToUpdateServerType(s->value("updateserver.type", "OfficialMirror").toString()));
	setUpdateMirror(s->value("updateserver.mirror", "").toString());
	setCustomUpdateServer(s->value("updateserver.customserver.url", "").toString());
	m_modified = false;
	Q_EMIT(changed());
qDebug() << "loaded settings from" << s->fileName();

	delete s;
}


void Settings::saveCopyAs( const QString & path ) const {
	QSettings * s = nullptr;

	if(m_filePath.isEmpty()) {
		qDebug() << "saving to default path";
		s = new QSettings(QSettings::IniFormat, QSettings::UserScope, qlamApp->organizationName(), qlamApp->applicationName() + ".appconfig");
	}
	else {
		qDebug() << "saving to" << path;
		s = new QSettings(path, QSettings::IniFormat);
	}

qDebug() << "saved settings to" << s->fileName();
	s->setValue("database.path", databasePath());
	s->setValue("updateserver.type", updateServerTypeToString(updateServerType()));
	s->setValue("updateserver.mirror", updateMirror());
	s->setValue("updateserver.customserver.url", customUpdateServer().toString());

	delete s;
}


QString Settings::updateServerTypeToString( const Settings::UpdateServerType & type ) {
	switch(type) {
		case OfficialMirror:
			return QString::fromUtf8("OfficialMirror");

		case Custom:
			return QString::fromUtf8("Custom");
	}

	return QString();
}


Settings::UpdateServerType Settings::stringToUpdateServerType( const QString & type ) {
	if("OfficialMirror" == type) {
		return OfficialMirror;
	}
	else if("Custom" == type) {
		return Custom;
	}

	return UpdateServerType(-1);
}