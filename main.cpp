/*
 * Copyright 2012 Anthony Cassidy
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include <QtGui/QApplication>
#include <QMainWindow>
#include "FolderTraverser.h"
#include "TLD.h"
using namespace com::github::a2g::generator;

static void messageFilterForSuppressingQtMessages(QtMsgType type, const char * msg)
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

void installDummyMessageHandlerToSuppressQtMessagesInTheDebugOutput()
{
    qInstallMsgHandler(messageFilterForSuppressingQtMessages);
}
void populateFileSystemFromRealSystemRecursively(FileSystem* fs, QString path)
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
            populateFileSystemFromRealSystemRecursively(fs, name);
        }
        else if(isPNG)
        {
            fs->add(name);
        }
    }
}

void processRawFilesToResources(QString arg, QString findMe, bool isGwt)
{
    OutputFiles output;
    {
        FileSystem fileSystem;
        populateFileSystemFromRealSystemRecursively(&fileSystem,arg );
        {
            FolderTraverser trav(fileSystem, output);
            trav.searchForAllSubFoldersContainingKeyFolderAndGenerateIfFound(arg, findMe);
        }
    }

    output.writeAll(findMe, "visuals", isGwt);
}

int main(int argc, char *argv[])
{
    installDummyMessageHandlerToSuppressQtMessagesInTheDebugOutput();
    QApplication app(argc, argv);
    QString arg = "D:/Conan/Swing/src";
    if(argc>1)
    {
        arg = argv[1];
    }
    {
        processRawFilesToResources(arg, "gwt-resource", true);
        processRawFilesToResources(arg, "swing-resource", false);
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



