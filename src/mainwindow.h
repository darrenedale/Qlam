#ifndef QLAM_MAINWINDOW_H
#define QLAM_MAINWINDOW_H

#include <QtGlobal>
#include <QtWidgets/QMainWindow>

class QStackedWidget;
class QToolButton;

namespace Ui {
	class MainWindow;
}

namespace Qlam {
	class ScanWidget;
	class ScanProfileChooser;
	class UpdateWidget;
	class SettingsDialogue;

	class MainWindow
	: public QMainWindow {
		Q_OBJECT

		public:
			explicit MainWindow( QWidget *parent = nullptr );
			~MainWindow();

		public Q_SLOTS:
//            void startScan();

			bool startScanByProfileName( const QString & profile );
			bool startCustomScan( const QStringList & paths );

		protected:
			virtual void closeEvent( QCloseEvent * ev );

		private Q_SLOTS:
			/* might need this if we want to prompt to save changes in current
			 * widget before switching */
//			void slotSyncStackWithList();
			void slotScanProfileAdded( int i );
			void slotScanProfileChosen( int i );
			void slotSaveProfileButtonClicked();
			void slotScanPathsChanged();
			void slotScanBackButtonClicked();
			void syncScanBackButtonWithStack();
			void slotDisableBackButton();
			void slotEnableBackButton();

		private:
			void addBuiltInWidgets();
			void readSettings();
			void writeSettings() const;

			Ui::MainWindow *ui;
			QStackedWidget * m_scanStack;
			QToolButton * m_scanBackButton;
			ScanWidget * m_scanWidget;
			ScanProfileChooser * m_scanProfileChooser;
			UpdateWidget * m_updateWidget;
			QWidget * m_reportsWidget;
			SettingsDialogue * m_settingsDialogue;
	};
}

#endif // QLAM_MAINWINDOW_H