/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#include "FolderTraverser.h"
#include "A2GASSERT.h"
#include "FOLDERSONLY.h"
#include <time.h>
#include <locale>
#include <QRgb>
#include <QImage>

#include <QImage>
#include <QRect>
#include <QSet>
#include <QList>
#include <QDomDocument>
#include <QTextStream>
#include <QTextBrowser>
#include <QVariant>
#include "OFile.h"
#include "IFile.h"
#include "LoaderAndResFilePair.h"
#include "AFile.h"
#include "GetObjectPlusAnimHash.h"
//#include "SaveDelayLoadRes.h"


using namespace com::github::a2g::generator;

const char* _00_ANIMATIONS = "_00_Animations";

void FolderTraverser::Generate(QString rootFolder, QString package)
{



    QString bad = ";"; //this is a char that will invalidate a filename, and thus any XFile class if used as param1


    QString animFolder = _00_ANIMATIONS;
    AFile aFile(rootFolder, animFolder, package);

    QStringList foldersOfMaxFile = files.getSubFolders(rootFolder);
    for(QStringList::iterator maxFileFolder=foldersOfMaxFile.begin();maxFileFolder!=foldersOfMaxFile.end();maxFileFolder++)
    {
        QString maxFileSeg = QDir(*maxFileFolder).dirName();
        if(maxFileSeg == "_00_Animations")
            continue;
        bool isInAnObjectsFolder = maxFileSeg.contains("_Main");
        bool isInAnInventoryFolder = maxFileSeg.contains("_Inventory");

        OFile oStream(isInAnObjectsFolder? *maxFileFolder : bad, maxFileSeg, package);
        IFile iStream(isInAnInventoryFolder? *maxFileFolder: bad, maxFileSeg, package);

        QStringList pixelFolders = files.getSubFolders(*maxFileFolder);
        for(QStringList::iterator pixelFolder=pixelFolders.begin();pixelFolder!=pixelFolders.end();pixelFolder++)
        {			
            QString pixelSeg = QDir(*pixelFolder).dirName();


            LoaderAndResFilePair resStream( *maxFileFolder, pixelSeg, maxFileSeg, isInAnInventoryFolder? NULL: animFolder, package);
            LoaderAndResFilePair initialStream( *maxFileFolder, pixelSeg, maxFileSeg, isInAnInventoryFolder? NULL: animFolder, package);

            std::list<QString> objectFolders = files.getSubFolders(*pixelFolder).toStdList();
            for(std::list<QString>::reverse_iterator objectFolder=objectFolders.rbegin();objectFolder!=objectFolders.rend();objectFolder++)
            {
                QString objectSeg = QDir(*objectFolder).dirName();

                if(!isInAnInventoryFolder)
                {
                    int idForObj = oStream.getIdForName(GetRealObjectSeg(objectSeg));

                    QStringList animFolders = files.getSubFolders(*objectFolder);
                    for(QStringList::iterator animFolder = animFolders.begin();animFolder!=animFolders.end();animFolder++)
                    {
                        QString animSeg = QDir(*animFolder).dirName().toLower();

                        int idForObjPlusAnim = GetObjectPlusAnimHash(objectSeg,animSeg);

                        aFile.insert(GetObjectPlusAnim(objectSeg,animSeg).toStdString(), idForObjPlusAnim);
                        QStringList PNGs = files.getSubFiles(*animFolder);

                        for(QStringList::iterator pngLoadMe = PNGs.begin();pngLoadMe!=PNGs.end();pngLoadMe++)
                        {   
                            if(pngLoadMe->right(4) != ".png")
                                continue;
                            if(!pngLoadMe->contains("/orig_"))
                                continue;

                            if(isInAnObjectsFolder && (animSeg=="initial" || animSeg=="blank"))
                            {
                                initialStream.addAnimImage(*pngLoadMe, idForObj);
                            }
                            else
                            {
                                resStream.addAnimImage(*pngLoadMe, isInAnObjectsFolder? idForObj : -1);//if the id is -1 then it signals the engine to look for an existing object to add the anim image to
                            }
                        }

                    }
                }else
                {
                    QStringList PNGs = files.getSubFiles(*objectFolder);
                    for(int i=0;i<PNGs.size();i++)
                    {
                        QString pngPath = PNGs[i];
                        if(pngPath.right(4) != ".png")
                            continue;
                        QString invSeg = objectSeg;
                        int idForInv = iStream.getIdForName(invSeg);

                        resStream.addInvImage(pngPath, idForInv);

                        break;//only take the first png in each folder
                    }
                }
            }

            // set the names in one place;
            // if both the streams have content, and thus will write a valid file, then "first"+"last"
            // but if either is empty, and thus only one file then "only"
            if(initialStream.IsEmpty() || resStream.IsEmpty())
            {
                initialStream.SetJavaClassNamePrefix("Only");
                resStream.SetJavaClassNamePrefix("Only");
                output.addOnly(initialStream);
                output.addOnly(resStream);
            }
            else
            {
                initialStream.SetJavaClassNamePrefix("First");
                resStream.SetJavaClassNamePrefix("Last");
                output.addFirst(initialStream);
                output.addLast(resStream);
            }


        }
        if(iStream.getNumberOfEntries()>0)
        {
            output.addIFile(iStream);
        }
        if(oStream.getNumberOfEntries()>0)
        {
            output.addOFile(oStream);
        }
    }

    if(aFile.getNumberOfEntries()>0)
    {
        output.addAFile(aFile);
    }
}	
void FolderTraverser::SearchForRootOfResourcesAndGenerateIfFound(QString startingPath)
{
    startingPath.replace("\\","/");
    QString startTargetFolder = "com";
    QString endTargetFolder = "resource";
    return SearchRecursively(startingPath, startTargetFolder, endTargetFolder);
}

void FolderTraverser::SearchRecursively(QString folder, QString startTargetFolder, QString endTargetFolder)
{
    QStringList subFolders = files.getSubFolders(folder);
    for(int i=0;i<subFolders.count();i++)
    {
        QString subFolder = subFolders.at(i);
        if(subFolder.contains(_00_ANIMATIONS))
            continue;//

        int start = subFolder.lastIndexOf(startTargetFolder);
        int end = subFolder.lastIndexOf(endTargetFolder);
        if(start!=-1 && end!=-1 && start<=end)
        {
            // we've found the target level, as prescribed by the start and end folders
            QStringList subsubFolders = files.getSubFolders(subFolder);
            for(int j=0;j<subsubFolders.count();j++)
            {
                QString subsubFolder = subsubFolders.at(j);
                if(subsubFolder.contains(_00_ANIMATIONS))
                    continue;// ..or else we keep generating animations folders in animations folders..

                QString folderSeg = QDir(subsubFolder).dirName();
                QString package = subsubFolder.mid(start,end+endTargetFolder.length()+folderSeg.length()+1 );    
                package.replace('/','.');
                Generate(subsubFolder, package);
            }
            // since we've processed all the subfolders at our target, then we return
            // otherwise it will keep recursing through subdirs, and these will keep matching..
            // ...since they contain both the start and the end.
            // and then we'll get Generate called in many places, and _00_Animations folders in many places.
            return;
        }
        //recurse
        SearchRecursively(subFolder, startTargetFolder, endTargetFolder);
    }
}


