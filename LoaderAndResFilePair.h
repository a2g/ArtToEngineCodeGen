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

#pragma once

#include <QFile>
#include <QTextStream>
#include <QPoint>
#include <QVector>
#include <iostream>
#include <QMessageBox>
#include <QDir>
#include <QMap>
#include "FIRST.h"
#include "Crop.h"
#include "GetObjectPlusAnim.h"
#include "GetRealObjectSeg.h"
#include <QPair>
#include <algorithm>

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class LoaderAndResFilePair 
                {
                    mutable QString package;
                    mutable QString maxFileSeg;
                    mutable QString pixelSeg;
                    mutable QString sceneFolder;
                    mutable QString animFolder;

                    QString theJavaClassNamePrefix;
                    QString javaClassName;

                    QString casesConcatenated;
                    QString resourcesConcatenated;


                    QVector<QString> caseStatements;
                    QMap<QString, QString> resourceDeclarations;
                    QVector<QPair<QString, int> > animImages;
                    QVector<QPair<QString, int> > invImages;
			
			bool isGwt;

                public:
                    LoaderAndResFilePair(){}
                    LoaderAndResFilePair(const QString& sceneFolder, const QString& pixelSeg, const QString maxFileSeg, QString animFolder, const QString package)
                            : package(package)
                            , maxFileSeg(maxFileSeg)
                            , pixelSeg(pixelSeg)
                            , sceneFolder(sceneFolder)
                            , animFolder(animFolder)
			    , isGwt(false)

                    {

                    }
                    ~LoaderAndResFilePair()
                    {
                        //  writeToFile();
                    }

                    void setJavaClassNamePrefix(QString theJavaClassNamePrefix)
                    {
                        this->theJavaClassNamePrefix = theJavaClassNamePrefix;
                    }

                    QString getJavaClassNamePrefix()
                    {
                        return theJavaClassNamePrefix;
                    }

                    QString getJavaClassName()
                    {
                        return javaClassName;
                    }

                    QString makePath(QString fullOfSlashes)
                    {
                        fullOfSlashes.replace('.','/');
                        return fullOfSlashes;
                    }

                    void addAnimImage(QString pngPath, int idForObj)
                    {
                        animImages.push_back(QPair<QString,int>(pngPath,idForObj));
                    }

                    void addInvImage(QString pngPath, int idForInv)
                    {
                        invImages.push_back(QPair<QString,int>(pngPath,idForInv));
                    }

                    void cropImagesAndConstructDeclarations(QString find, QString replace)
                    {
                        int caseTally = 0;
                        for(int j=0;j<animImages.size();j++)
                        {
                            QPair<QString, int> pair = animImages[j];
                            QStringList list = pair.first.split("/");
                            QString root;
                            int i=0;
                            for(;i<list.count()-1;i++)
                            {
                                root+=list[i]+"/";
                            }

                            QString animFolder = QDir(root).canonicalPath();
                            QString maxFileSeg = list[i-4];
                            QString pixelSeg   = list[i-3];
                            QString objectSeg  = list[i-2];
                            QString animSeg    = list[i-1];
                            QString pngPath    = pair.first;
                            int idForObject    = pair.second;
                            

                            QString pngByItself = "cropped_" + QDir(pngPath).dirName();
                            QString pngSaveTo = animFolder +"/" + pngByItself;
                            pngSaveTo = pngSaveTo.replace(find,replace);
                            // do the cropping


                            QPoint offset = crop(pngPath, pngSaveTo, Qt::red);

                            // add case and resource
                            QString objectPlusAnim = getObjectPlusAnim(objectSeg, animSeg);
                            QString resourceName = QString("%1__%2_%3()").arg(getRealObjectSeg(objectSeg)).arg(animSeg).arg(caseTally++);
                            resourceDeclarations[resourceName] = pngSaveTo;
                            addCaseStatementForAnim(objectSeg, offset, getRealObjectSeg(objectSeg), animSeg, idForObject, objectPlusAnim, resourceName);
                        }

                        for(int j=0;j<invImages.size();j++)
                        {
                            QPair<QString, int> pair = invImages[j];
                            QString pngPath    = pair.first;
                            pngPath = pngPath.replace(find,replace);
                            QStringList list = pngPath.split("/");
                            QString root;
                            int i=0;
                            for(;i<list.count()-1;i++)
                            {
                                root+=list[i]+"/";
                            }
                            
                            QString animFolder = root;
                            QString maxFileSeg = list[i-3];
                            QString pixelSeg   = list[i-2];
                            QString objectSeg  = list[i-1];

                            int idForObject    = pair.second;
                            
                            QString justThePNG = QDir(pngPath).dirName();
                            QString resourceName = QString("inv_%1()").arg(objectSeg);

                            QDir dir(pngPath);

                            bool isDirMade = dir.mkpath(root);
                            isDirMade=isDirMade;
                            bool isCopyOk = QFile::copy(pair.first, pngPath);
                            isCopyOk = isCopyOk;
                            resourceDeclarations[resourceName] = pngPath;
                            addCaseStatementForInv(objectSeg, idForObject, resourceName);
                        }
                    }

                    bool isEmpty()
                    {
                        bool isEmpty = animImages.size()==0 && invImages.size()==0;
                        return isEmpty;
                    }

                private: 
                    bool isNeeded()
                    {
                        bool isNeeded = (maxFileSeg.contains("_Objects")|| javaClassName!=FIRST);
                        return isNeeded;
                    }

                    bool isInventory()
                    {
                        bool isInventory = (maxFileSeg.contains("_Inventory"));
                        return isInventory;
                    }

                    void addCaseStatementForAnim(QString objectSeg, QPoint offset, QString realObjectSeg, QString animSeg, int idForObj, QString objectPlusAnim, QString resourceName)
                    {
                        int prefix = objectSeg.mid(1,2).toInt();

                        int x = offset.x();
                        int y = offset.y();
                        QString oPlusA = objectPlusAnim.toUpper();
                        QString caseStatement = QString("case %1: return api.addImageForASceneObject(lh, %2,%3,%4, \"%getValue(), new %9PackagedImage(res.%10));\n").arg(caseStatements.size()).arg(prefix).arg(x).arg(y).arg(realObjectSeg.toUpper()).arg(animSeg.toUpper()).arg(idForObj).arg(oPlusA).arg(isGwt? "GWT" : "Swing").arg(resourceName);
                        caseStatements.push_back(caseStatement);
                    }

                    void addCaseStatementForInv(QString invSeg, int idForInv,QString resourceName)
                    {
                        QString caseStatement = QString("case %1: return api.addImageForAnInventoryItem(lh, \"%2\",%3,new %4PackagedImage(res.%5));\n").arg(caseStatements.size()).arg(invSeg.toUpper()).arg(idForInv).arg(isGwt? "GWT" : "Swing").arg(resourceName);
                        caseStatements.push_back(caseStatement);
                    }

                    void writeGwtBundle(QTextStream& f, QString bundleJavaClassName, int start, int end)
                    {
                        QStringList packageSegs = package.split(".");

                        f << ("package "+package+"." + maxFileSeg +"." + pixelSeg +";\n");
                        f << ("\n");
                        
                        f << ("import com.github.a2g.core.gwt.factory.GWTPackagedImage;\n");
                        f << ("import com.github.a2g.core.interfaces.ImageAddAPI;\n");
			f << ("import com.google.gwt.resources.client.ClientBundle;\n");
                        f << ("import com.google.gwt.event.dom.client.LoadHandler;\n");
                        f << ("import com.google.gwt.core.client.GWT;\n");
                        f << ("import com.google.gwt.resources.client.ImageResource;\n");
                        if(animFolder!=NULL)
                            f << ("import "+package+"." + animFolder +".a;\n");
                        f << ("\n");
                        f << ("public class "+bundleJavaClassName+"\n");
                        f << ("{\n");
                        f << ("    public interface MyRes extends ClientBundle");
                        f << ("    {\n");
                        f << ("        public static final "+bundleJavaClassName+".MyRes RESOURCE =  GWT.create("+bundleJavaClassName+".MyRes.class);\n");
                        f << ("        \n");
                        for(QMap<QString,QString>::iterator iter=resourceDeclarations.begin();iter!=resourceDeclarations.end();iter++)
                        {
                            QString resourceName = iter.key();
                            QString fullPath = iter.value();
                            QStringList pngSegs = fullPath.split("/");
                            QString relativePath = packageSegs[0];
                            QStringList::iterator iter = std::find(pngSegs.begin(), pngSegs.end(), packageSegs[1]);
                            if(iter!=pngSegs.end())
                            {
                                while(iter!=pngSegs.end())
                                {
                                    relativePath+="/" + *iter;
                                    iter++;
                                }
                                QString resrc = ("        @Source(\"" + relativePath + "\")\n");
                                resrc = resrc + ("        abstract ImageResource "+resourceName+";\n");

                                f << resrc;
                            }
                            else
                            {
                                std::cout << "\n " + package.toStdString() + "this path doesn't contain package" + fullPath.toStdString() ;
                            }
                        }
                        f << "    }"                                                                               "\n";
                        f << QString("    public static boolean addImage(ImageAddAPI api, LoadHandler lh, int i)\n");
                        f << "    {"                                                                               "\n";
                        f << "        final MyRes res = MyRes.RESOURCE;"                                           "\n";
                        f << "        switch(i){"                                                                  "\n";
                        for(int j=start;j<end;j++)
                        {
                            f << caseStatements[j];
                        }
                        f << "        }"                                                                           "\n";
                        f << "        return false;"                                                               "\n";
                        f << "    }"                                                                               "\n";
                        f << "}"                                                                                   "\n";

                    }

                    bool writeGwtLoader(QTextStream& f, QString loaderJavaClassName, const std::vector<std::pair<int,QString> >&  list)
                    {
                        f << ("package "+package+"." + maxFileSeg +"." + pixelSeg +";\n");
                        f << ("\n");
			f << ("import com.github.a2g.core.interfaces.LoadAPI;\n");
                        f << ("import com.github.a2g.core.interfaces.InternalAPI;\n");
                        f << ("import com.google.gwt.event.dom.client.LoadHandler;\n");
                        f << ("import com.google.gwt.core.client.GWT;\n");
                        f << ("import com.google.gwt.core.client.RunAsyncCallback;\n");
                        f << ("import com.google.gwt.core.client.Scheduler;\n");


                        f << ("\n");
                        f << ("public class "+loaderJavaClassName+" implements LoadAPI\n");
                        f << ("{\n");
                        f << ("  @Override\n");
                        f << QString("  public boolean isInventory(){ return %1;}\n").arg(isInventory()? "true" : "false");
                        f << ("  @Override\n");
                        f << QString("  public int getNumberOfBundles(){ return %1;}\n").arg(list.size());
                        f << ("  \n");
                        f << ("  @Override\n");
                        f << ("  public int getNumberOfImagesInBundle(int bundleNumber)\n");
                        f << ("  {\n");
                        f << ("    switch(bundleNumber)\n");
                        f << ("    {\n");

                        for(unsigned int i=0;i<list.size();i++)
                        {
                            f << QString("    case %1: return %2; \n").arg(i).arg(list[i].first);
                        }
                        f << ("    }\n");
                        f << ("    return 0;\n");
                        f << ("  }\n");
                        f << ("  @Override\n");
                        f << ("  public int loadImageBundle(final LoadHandler lh, final InternalAPI api, final int bundleNumber, final int CHUNK, final int milliseconds)\n");
                        f << ("  {\n");
                        f << ("    switch(bundleNumber)\n");
                        f << ("    {\n");
                        int offset = 0;
                        for(unsigned int i=0;i<list.size();i++)
                        {
                            f << QString("    case %1: \n").arg(i);
                            f << QString("    GWT.runAsync\n");
                            f << QString("    (\n");
                            f << QString("      new RunAsyncCallback()\n");
                            f << QString("      {\n");
                            f << QString("        public void onSuccess()\n");
                            f << QString("        {\n");
                            f << QString("          // Schedule the IncrementalCommand instance to run when\n");
                            f << QString("          // control returns to the event loop by returning 'true'\n");
                            f << QString("          Scheduler.get().scheduleFixedDelay\n");
                            f << QString("          (\n");
                            f << QString("            new Scheduler.RepeatingCommand()\n");
                            f << QString("            {\n");
                            f << QString("              int counter= 0;\n");
                            f << QString("              @Override\n");
                            f << QString("              public boolean execute()\n");
                            f << QString("              {\n");
                            f << QString("                %1.addImage(api, lh, counter+%2);\n").arg(list[i].second).arg(offset);
                            f << QString("                return (++counter<%1);\n").arg(list[i].first);
                            f << QString("              }\n");
                            f << QString("            }\n");
                            f << QString("            ,milliseconds\n");
                            f << QString("          );\n");
                            f << QString("        }\n");
                            f << QString("        public void onFailure(Throwable caught)\n");
                            f << QString("        {\n");
                            f << QString("		   lh.onLoad(null);\n");
                            f << QString("        }\n");
                            f << QString("      }\n");
                            f << QString("    );\n");// end GWT.runAsync
                            f << QString("    return 0;\n");
                            offset += list[i].first;
                        }
                        f << ("}\n"); // end switch
                        f << ("return 0;\n");
                        f << ("}\n");// end function
                        f << ("}\n");// end class
                        return true;
                    }


                    void writeSwingBundle(QTextStream& f, QString bundleJavaClassName, int start, int end)
                    {
                        f << ("package "+package+"." + maxFileSeg +"." + pixelSeg +";\n");
                        f << ("\n");
			f << ("import com.github.a2g.core.interfaces.ImageAddAPI;\n");
                        f << ("import com.github.a2g.core.swing.factory.SwingPackagedImage;\n");
                        f << ("import com.google.gwt.event.dom.client.LoadHandler;\n");
                        if(animFolder!=NULL)
                            f << ("import "+package+"." + animFolder +".a;\n");
                        f << ("\n");
                        f << ("public class "+bundleJavaClassName+"\n");
                        f << ("{\n");
                        f << ("    public static class MyRes");
                        f << ("    {\n");
                        f << ("        public static final MyRes RESOURCE = new MyRes();");
                        f << ("        \n");

                        for(QMap<QString,QString>::iterator iter=resourceDeclarations.begin();iter!=resourceDeclarations.end();iter++)
                        {
                            QString resourceName = iter.key();
                            QString fullPath = iter.value();

                            f << ("        public String "+resourceName+"{ return \"" + fullPath + "\";}\n");
                        }
                        f << "    }"                                                                                "\n";

                        f << "    public static boolean addImage(ImageAddAPI api, LoadHandler lh, int i)"      "\n";
                        f << "    {"                                                                               "\n";
                        f << "        final MyRes res = MyRes.RESOURCE;"                                           "\n";
                        f << "        switch(i){"                                                                  "\n";
                        for(int j=start;j<end;j++)
                        {
                            f << caseStatements[j];
                        }
                        f << "        }"                                                                           "\n";
                        f << "        return false;"                                                               "\n";
                        f << "    }"                                                                               "\n";
                        f << "}"                                                                                   "\n";

                    }

                    bool writeSwingLoader(QTextStream& f, QString loaderJavaClassName, const std::vector<std::pair<int,QString> >&  list)
                    {
                        f << ("package "+package+"." + maxFileSeg +"." + pixelSeg +";\n");
                        f << ("\n");
                        f << ("import javax.swing.Timer;\n");
                        f << ("import java.awt.event.ActionEvent;\n");
                        f << ("import java.awt.event.ActionListener;\n");
                        f << ("import com.google.gwt.event.dom.client.LoadHandler;\n");
                        f << ("import com.github.a2g.core.interfaces.LoadAPI;\n");
                        f << ("import com.github.a2g.core.interfaces.InternalAPI;\n");


                        f << ("\n");
                        f << ("public class "+loaderJavaClassName+" implements LoadAPI\n");
                        f << ("{\n");
                        f << ("  Timer timer;\n");
                        f << ("  @Override\n");
                        f << QString("  public boolean isInventory(){ return %1;}\n").arg(isInventory()? "true" : "false");
                        f << ("  @Override\n");
                        f << QString("  public int getNumberOfBundles(){ return %1;}\n").arg(list.size());
                        f << ("  \n");
                        f << ("  @Override\n");
                        f << ("  public int getNumberOfImagesInBundle(int bundleNumber)\n");
                        f << ("  {\n");
                        f << ("    switch(bundleNumber)\n");
                        f << ("    {\n");

                        for(unsigned int i=0;i<list.size();i++)
                        {
                            f << QString("    case %1: return %2; \n").arg(i).arg(list[i].first);
                        }
                        f << ("    }\n");
                        f << ("    return 0;\n");
                        f << ("  }\n");
                        f << ("  @Override\n");
                        f << ("  public int loadImageBundle(final LoadHandler lh, final InternalAPI api, final int bundleNumber, final int CHUNK, final int milliseconds)\n");
                        f << ("  {\n");
                        f << ("    switch(bundleNumber)\n");
                        f << ("    {\n");
                        int offset = 0;
                        for(unsigned int i=0;i<list.size();i++)
                        {
                            f << QString("    case %1: \n").arg(i);
                            f << QString("      timer = new Timer\n");
                            f << QString("      (\n");
                            f << QString("        milliseconds \n");
                            f << QString("        , new ActionListener()\n");
                            f << QString("        {\n");
                            f << QString("          int counter= 0;\n");
                            f << QString("          @Override\n");
                            f << QString("          public void actionPerformed(ActionEvent e)\n");
                            f << QString("          {\n");
                            f << QString("            %1.addImage(api, lh, counter+%2);\n").arg(list[i].second).arg(offset);
                            f << QString("            counter++;\n");
                            f << QString("            if(counter==%1)\n").arg(list[i].first);
                            f << QString("               timer.stop();\n");
                            f << QString("          }\n");
                            f << QString("        }\n");
                            f << QString("      );\n");
                            f << QString("      timer.setInitialDelay(milliseconds);\n");
                            f << QString("      timer.start();\n");
                            f << QString("      return 0;\n");
                            offset += list[i].first;
                        }
                        f << ("    }\n");
                        f << ("    return 0;\n");
                        f << ("  }\n");


                        f << ("}\n");
                        return true;
                    }

                    bool writeToFile(QString find, QString replace, bool isGwt)
                    {
			this->isGwt=isGwt;
                        cropImagesAndConstructDeclarations(find, replace);

                        if(caseStatements.size()==0)
                            return false;

                        package = package.replace(find,replace);
                        sceneFolder =sceneFolder.replace(find,replace);
                        std::vector<std::pair<int,QString> > list;
                        int total = resourceDeclarations.size();
                        int numberOfBundles = 10;
                        int maximumFeasibleSceneObjects = 80;
                        double fincrement = total/numberOfBundles; 
                        double fend = fincrement;
                        int start = 0;
                        int end = static_cast<int>(fend);
                        if(total < maximumFeasibleSceneObjects)
                        {
                            numberOfBundles = 1;
                            end = total;
                        }
                        const char* names[10]={"0","1","2","3","4","5","6","7","8","9"};
                        for(int i=0;i<numberOfBundles;i++)
                        {
                            QString bundleJavaClassName = theJavaClassNamePrefix + names[i] + "Bundle";
                            QDir dir;
                            dir.mkpath(sceneFolder+"/"+ pixelSeg);
                            QFile file(sceneFolder+"/"     + pixelSeg + "/" + bundleJavaClassName + ".java");
                            if (!file.open(QFile::WriteOnly | QFile::Truncate))
                                return false;

                            QTextStream f(&file);
                            if(isGwt)
                            {
                                writeGwtBundle(f, bundleJavaClassName, start, end);
                            }
                            else
                            {
                                writeSwingBundle(f, bundleJavaClassName, start, end);                                                                                   "\n";
                            }
                            file.close();
                            QString longName = package+"." + maxFileSeg +"." + pixelSeg+"."+bundleJavaClassName;
                            list.push_back(std::pair<int,QString>(end-start, longName));
                            fend = fend + fincrement;
                            start = end;
                            end = static_cast<int>(fend);
                        }

                        // now to write the loader
                        QString loaderJavaClassName = theJavaClassNamePrefix + "Loader";
                        QDir dir;
                        dir.mkpath(sceneFolder+"/"+ pixelSeg);
                        QFile loader( sceneFolder+"/"+ pixelSeg+"/"+loaderJavaClassName + ".java");
                        if (!loader.open(QFile::WriteOnly | QFile::Truncate))
                            return false;

                        QTextStream f(&loader);
                        bool result = isGwt? writeGwtLoader(f, loaderJavaClassName, list) : writeSwingLoader(f, loaderJavaClassName, list);
                        (result=!!result);
                        loader.close();

                        return true;
                    }

                    friend class OutputFiles;
                };
            }
        }
    }
}

