/*
 *
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


#include <QDir>
#include "getFoldersToProcess.h"
#include "SOURCEIMAGES.h"
#include "VISUALS.h"
#include "time.h" //clock_t clock (); time_t time(time_t*), double difftime (time_t, time_t);
#include "IsPngOrBmp.h"
#include "QDebug.h"
#include <QtGlobal> //qInstallMessageHandler
#include <QApplication>
#include "isPngOrBmp.h"
#include "WriterSwing.h"
#include "WriterGwt.h"
#include "buildDom.h"
#include "buildDom2.h"
#include "cropAllFramesAndUpdateImagePaths.h"
#include "buildFilenamesAndContents.h"
#include "saveOutFilenamesAndContents.h"
#include "debugapi.h"
using namespace com::github::a2g::generator;



#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void messageFilterForSuppressingQtMessages(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
#else
void messageFilterForSuppressingQtMessages(QtMsgType type, const char *msg)
{
#endif
    QByteArray localMsg = msg.toLocal8Bit();
    switch(type) {
    case QtDebugMsg:
        fprintf(stderr,"Debug: %s (%s:%u, %s)\n",localMsg.constData(),context.file,context.line,context.function);
        OutputDebugStringA(localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr,"Info: %s (%s:%u, %s)\n",localMsg.constData(),context.file,context.line,context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr,"Warning: %s (%s:%u, %s)\n",localMsg.constData(),context.file,context.line,context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr,"Critical: %s (%s:%u, %s)\n",localMsg.constData(),context.file,context.line,context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr,"Fatal: %s (%s:%u, %s)\n",localMsg.constData(),context.file,context.line,context.function);
        abort();
    }
}
 
void populateFileSystemFromRealSystemRecursively(FileSystem* fs, QString path)
{
    // the only filter that would be useful here would be files with our desired image extensions AND folders - but it's not possible
    // so we go thru them all and look for either
    QStringList dirs = QDir(path).entryList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot, QDir::Name|QDir::IgnoreCase);
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
        else if(isPngOrBmp(name))
        {
            fs->add(name);
        }
    }
}
 
int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageFilterForSuppressingQtMessages);
    QApplication app(argc, argv);
    QString arg;
    if(argc>1)
    {
        arg = argv[1];
    }

    QDir dir(arg);
    if(!dir.exists(arg))
        return 0; //quick exit. don't want to unit test this because it deals with files on disk

    {
        clock_t t1 = clock();

        FileSystem fileSystem;
        populateFileSystemFromRealSystemRecursively(&fileSystem,arg);

        qInfo() << "blah\n";
        clock_t t2 = clock();
        qDebug() << "Adding folders " << (t2-t1)/1000.0 << " seconds\n";



        QStringList folders = getFoldersToProcess(arg,fileSystem);
        int foldersSize = folders.size();
        for(int i=0;i<foldersSize;i++)
        {

            bool isGwt = !folders[i].contains("Swing");
            auto dom1 = buildDom(folders[i],fileSystem);
           
         
            cropAllFramesAndUpdateImagePaths(dom1->asRef());

            std::string bit = dom1.get()->locationPath.toStdString();
            dom1.get()->locationPath.replace(SOURCEIMAGES.toUpper(),VISUALS.toUpper());//update location - maybe do in the crop phase?
            dom1.get()->fullLocationPackage.replace(SOURCEIMAGES.toLower(),VISUALS.toLower());//update location - maybe do in the crop phase?

            static const WriterSwing swing;
            static const WriterGwt gwt;

            auto dom2 = buildDom2(dom1->asRef());
            auto x = buildFilenamesAndContents(dom2->asRef(),true? (IWriter&)swing : (IWriter&)gwt);
          

            int size = x.size();
            for(int i=0;i<size;i++)
            {
                qDebug() <<  x[i].second.c_str() << "\n";
            }

            saveOutFilenamesAndContents(x);
        }
        clock_t t3 = clock();
        qDebug() << "Planning output " << (t3-t2)/1000.0 << " seconds \n";


        //                 ^              ^
        //               search        replace
        clock_t t4 = clock();
        qDebug() << "Writing output " << (t4-t3)/1000.0 << " seconds \n";
        qDebug() << "-----------------------------------------------\n";
        qDebug() << "Total " << (t4-t1)/1000.0 << " seconds \n";
        qDebug() << "( for path: " << arg.toStdString().c_str() << " )\n";

        // dont need to dump the filesystem
        //for(int i=0;i<fileSystem.getNumberOfItems();i++)
        //{
        //    qDebug() <<  fileSystem.getItem(i).toUtf8().data() << "\n";
        //}
    }
    return 0;
}



