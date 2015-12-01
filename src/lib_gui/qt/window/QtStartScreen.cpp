#include "qt/window/QtStartScreen.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "settings/ApplicationSettings.h"
#include "utility/file/FileSystem.h"
#include "utility/messaging/type/MessageLoadProject.h"

#include "qt/utility/QtDeviceScaledPixmap.h"
#include "qt/utility/utilityQt.h"
#include "isTrial.h"

QtRecentProjectButton::QtRecentProjectButton(const QString &text, QWidget *parent)
	: QPushButton(text, parent)
{
	m_projectFile = text.toStdString();
	this->setText(FileSystem::fileName(text.toStdString()).c_str());
	this->setToolTip(m_projectFile.c_str());
}

void QtRecentProjectButton::handleButtonClick()
{
	MessageLoadProject(m_projectFile, false).dispatch();
};

QtStartScreen::QtStartScreen(QWidget *parent)
	: QtSettingsWindow(parent, 68)
{
	this->raise();
}

void QtStartScreen::setup()
{
	setStyleSheet(utility::getStyleSheet("data/gui/startscreen/startscreen.css").c_str());

	addLogo();

	if(!isTrial())
	{
		m_newProjectButton = new QPushButton("New Project", this);
		m_newProjectButton->setAttribute(Qt::WA_LayoutUsesWidgetRect); // fixes layouting on Mac
		m_newProjectButton->setGeometry(30, 495, 140, 30);
		m_newProjectButton->setObjectName("projectButton");
		connect(m_newProjectButton, SIGNAL(clicked()), this, SLOT(handleNewProjectButton()));

		m_openProjectButton = new QPushButton("Open Project", this);
		m_openProjectButton->setAttribute(Qt::WA_LayoutUsesWidgetRect); // fixes layouting on Mac
		m_openProjectButton->setGeometry(30, 540, 140, 30);
		m_openProjectButton->setObjectName("projectButton");
		connect(m_openProjectButton, SIGNAL(clicked()), this, SLOT(handleOpenProjectButton()));
	}

	QLabel* recentProjectsLabel = new QLabel("Recent Projects: ", this);
	recentProjectsLabel->setGeometry(300, 234, 300, 50);
	recentProjectsLabel->setObjectName("recentLabel");

	int position = 290;
	QIcon cpp_icon("data/gui/startscreen/icon_cpp.png");
	std::vector<FilePath> recentProjects = ApplicationSettings::getInstance()->getRecentProjects();
	for (size_t i = 0; i < recentProjects.size() && i < ApplicationSettings::MaximalAmountOfRecentProjects; i++)
	{
		FilePath project = recentProjects[i];
		QtRecentProjectButton* button = new QtRecentProjectButton(project.str().c_str(), this);
		button->setAttribute(Qt::WA_LayoutUsesWidgetRect); // fixes layouting on Mac
		button->setIcon(cpp_icon);
		button->setIconSize(QSize(25, 25));
		button->setObjectName("recentButton");
		button->minimumSizeHint(); // force font loading
		button->setGeometry(292, position, button->fontMetrics().width(button->text()) + 45, 40);
		connect(button, SIGNAL(clicked()), button, SLOT(handleButtonClick()));
		connect(button, SIGNAL(clicked()), this, SLOT(handleRecentButton()));
		position += 40;
	}

	resize(600, 600);
}

QSize QtStartScreen::sizeHint() const
{
	return QSize(500, 500);
}

void QtStartScreen::handleNewProjectButton()
{
	emit openNewProjectDialog();
}

void QtStartScreen::handleOpenProjectButton()
{
	emit openOpenProjectDialog();
}

void QtStartScreen::handleRecentButton()
{
	emit finished();
}
