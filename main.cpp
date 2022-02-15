#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QStyleFactory>
#include <QtGlobal>

#include "mainwindow.h"
#include "gqrx.h"

static void reset_conf(const QString &file_name);
static void list_conf(void);

int main(int argc, char *argv[])
{
    QString         cfg_file;
    std::string     conf;
    std::string     style;
    bool            clierr = false;
    bool            edit_conf = false;
    int             return_code = 0;

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(GQRX_ORG_NAME);
    QCoreApplication::setOrganizationDomain(GQRX_ORG_DOMAIN);
    QCoreApplication::setApplicationName(GQRX_APP_NAME);
 //   QCoreApplication::setApplicationVersion(VERSION);

	MainWindow w(cfg_file, edit_conf);

        if (w.configOk)
        {
            w.show();
            return_code = app.exec();
        }
        else
            {
            return_code = 1;
            
        }
  
	printf(" qqq goodbye \n");

    return  return_code;
}



