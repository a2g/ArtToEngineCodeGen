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
#include "SOURCEIM.h"
#include "time.h" //clock_t clock (); time_t time(time_t*), double difftime (time_t, time_t);
#include "IsPngOrBmp.h"
#include "QDebug.h"
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
    // the only filter that would be useful here would be files with our desired image extensions AND folders - but it's not possible
    // so we go thru them all and look for either
    QStringList dirs = QDir(path).entryList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    int count = dirs.count();
    for(int i=0;i<count;i++)
    {
        QString name = path + "/" + dirs.at(i);
        bool isFolder = !name.contains(".");

        if(isFolder)
        {
            fs->add(name);   
            populateFileSystemFromRealSystemRecursively(fs, name);
        }
        else if(IsPngOrBmp(name))
        {
            fs->add(name);
        }
    }
}

void processRawFilesToResources(QString arg, bool isDummyRun)
{
    OutputFiles output;
    {
        clock_t t1 = clock();

        FileSystem fileSystem;
        populateFileSystemFromRealSystemRecursively(&fileSystem,arg);
        //fileSystem.dump();


        qDebug() << "\n";
        clock_t t2 = clock();
        qDebug() << "Adding folders " << (t2-t1)/1000.0 << " seconds\n";

        {
            FolderTraverser trav(fileSystem, output);
            trav.generateFilesFromSourceFolderOrASubFolderThereof(arg);
        }
        clock_t t3 = clock();
        qDebug() << "Planning output " << (t3-t2)/1000.0 << " seconds \n";

        output.writeAll(SOURCEIM, "visuals", fileSystem.isGwt(), isDummyRun);
        //                ^          ^
        //               search    replace
        clock_t t4 = clock();
        qDebug() << "Writing output " << (t4-t3)/1000.0 << " seconds \n";
        qDebug() << "-----------------------------------------------\n";
        qDebug() << "Total " << (t4-t1)/1000.0 << " seconds \n";
        qDebug() << "( for path: " << arg.toStdString().c_str() << " )\n";

        for(int i=0;i<fileSystem.getNumberOfItems();i++)
        {
            qDebug() <<  fileSystem.getItem(i).toAscii().data() << "\n";
        }
    }
}

int main(int argc, char *argv[])
{
  //  installDummyMessageHandlerToSuppressQtMessagesInTheDebugOutput();
    QApplication app(argc, argv);
    QString arg;
    if(argc>1)
    {
        arg = argv[1];
    }

    {
        processRawFilesToResources(arg, argc>2);
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



