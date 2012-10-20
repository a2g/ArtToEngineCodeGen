/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#include <QtGui/QApplication>
#include <QMainWindow>
#include "FolderTraverser.h"

using namespace com::github::a2g::generator;

static void MessageFilterForSuppressingQtMessages(QtMsgType type, const char * msg)
{
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", msg);
        abort();
    }
}

void InstallDummyMessageHandlerToSuppressQtMessagesInTheDebugOutput()
{
    qInstallMsgHandler(MessageFilterForSuppressingQtMessages);
}
void PopulateFileSystemFromRealSystemRecursively(FileSystem* fs, QString path)
{
    // the only filter that would be useful here would be ".png" AND folders - but it's not possible.f
    QStringList dirs = QDir(path).entryList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    int count = dirs.count();
    for(int i=0;i<count;i++)
    {
        QString name = path + "/" + dirs.at(i);
        bool isFolder = !name.contains(".");
        bool isPNG = name.contains(".png", Qt::CaseInsensitive);
            
        if(isFolder)
        {
            fs->add(name);   
            PopulateFileSystemFromRealSystemRecursively(fs, name);
        }
        else if(isPNG)
        {
            fs->add(name);
        }
    }
}

void ProcessRawFilesToResources(QString arg, QString findMe, bool isGwt)
{
    OutputFiles output;
    {
        FileSystem fileSystem;
        PopulateFileSystemFromRealSystemRecursively(&fileSystem,arg );
        {
            FolderTraverser trav(fileSystem, output);
            trav.SearchForRootOfResourcesAndGenerateIfFound(arg, "com", findMe);
        }
    }

    output.writeAll(findMe, "resource", isGwt);
}

int main(int argc, char *argv[])
{
    InstallDummyMessageHandlerToSuppressQtMessagesInTheDebugOutput();
    QApplication app(argc, argv);
    QString arg = "D:/Conan/Swing/src";
    if(argc>1)
    {
        arg = argv[1];
    }
    {
        ProcessRawFilesToResources(arg, "rawgwt", true);
        ProcessRawFilesToResources(arg, "rawawt", false);
    }
    return 0;

    /*
    QMainWindow x;
    
    QString filter = "*.htm *.html *.txt *.xml";
    FileBrowser* win = new FileBrowser(filter, &x);
    x.setCentralWidget(win);
    x.show();
    return app.exec();
    */
    return 0;
}



