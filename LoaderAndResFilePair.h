/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#pragma once

#include <QFile>
#include <QTextStream>
#include <QPoint>
#include <QVector>
#include <QMessageBox>
#include <QDir>
#include <QMap>
#include "FIRST.h"
#include "Crop.h"
#include "GetObjectPlusAnim.h"
#include "GetRealObjectSeg.h"
#include <QPair>

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
                    bool isGwt;

                    QVector<QString> caseStatements;
                    QMap<QString, QString> resourceDeclarations;
                    QVector<QPair<QString, int> > animImages;
                    QVector<QPair<QString, int> > invImages;

                public:
                    LoaderAndResFilePair(){};
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

                    void SetJavaClassNamePrefix(QString theJavaClassNamePrefix)
                    {
                        this->theJavaClassNamePrefix = theJavaClassNamePrefix;
                    }

                    QString GetJavaClassNamePrefix()
                    {
                        return theJavaClassNamePrefix;
                    }

                    QString GetJavaClassName()
                    {
                        return javaClassName;
                    }

                    QString MakePath(QString fullOfSlashes)
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

                    void CropImagesAndConstructDeclarations(QString find, QString replace)
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


                            QPoint offset = Crop(pngPath, pngSaveTo, Qt::red);

                            // add case and resource
                            QString objectPlusAnim = GetObjectPlusAnim(objectSeg, animSeg);
                            QString resourceName = QString("%1__%2_%3()").arg(GetRealObjectSeg(objectSeg)).arg(animSeg).arg(caseTally++);
                            AddImageResourceForAnim(maxFileSeg, pixelSeg, objectSeg, animSeg, pngSaveTo, resourceName);
                            AddCaseStatementForAnim(objectSeg, offset, GetRealObjectSeg(objectSeg), animSeg, idForObject, objectPlusAnim, resourceName);
                        }

                        for(int j=0;j<invImages.size();j++)
                        {
                            QPair<QString, int> pair = invImages[j];
                            QStringList list = pair.first.split("/");
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
                            QString pngPath    = pair.first;
                            int idForObject    = pair.second;
                            
                            QString justThePNG = QDir(pngPath).dirName();
                            QString resourceName = QString("inv_%1()").arg(objectSeg);
                            AddImageResourceForInv(maxFileSeg, pixelSeg, objectSeg, justThePNG, resourceName);
                            AddCaseStatementForInv(objectSeg, idForObject, resourceName);
                        }
                    }

                    bool IsEmpty()
                    {
                        bool isEmpty = animImages.size()==0 && invImages.size()==0;
                        return isEmpty;
                    }

                private: 
                    bool IsNeeded()
                    {
                        bool isNeeded = (maxFileSeg.contains("_Objects")|| javaClassName!=FIRST);
                        return isNeeded;
                    }

                    bool IsInventory()
                    {
                        bool isInventory = (maxFileSeg.contains("_Inventory"));
                        return isInventory;
                    }

                    void AddImageResourceForInv(QString maxFileSeg, QString pixelSeg, QString objectSeg, QString png, QString resourceName)
                    {
                        QString relativePath = QString(MakePath(package)+"/%1/%2/%3/%4").arg(maxFileSeg).arg(pixelSeg).arg(objectSeg).arg(png);
                        resourceDeclarations[resourceName] = relativePath;
                    }

                    void AddImageResourceForAnim(QString maxFileSeg, QString pixelSeg, QString objectSeg, QString animSeg, QString pngSaveTo, QString resourceName)
                    {	
                        //QString relativePath = QString(MakePath(package)+"/%1/%2/%3/%4/%5").arg(maxFileSeg).arg(pixelSeg).arg(objectSeg).arg(animSeg).arg(pngSaveTo);
                        resourceDeclarations[resourceName] = pngSaveTo;
                    }

                    void AddCaseStatementForAnim(QString objectSeg, QPoint offset, QString realObjectSeg, QString animSeg, int idForObj, QString objectPlusAnim, QString resourceName)
                    {
                        int prefix = objectSeg.mid(1,2).toInt();

                        int x = offset.x();
                        int y = offset.y();
                        QString oPlusA = objectPlusAnim.toUpper();
                        QString caseStatement = QString("case %9: return api.addImageForASceneObject(lh, %1,%2,%3, \"%4\",\"%5\",(short)%6,a.%7, res.%8	);\n").arg(prefix).arg(x).arg(y).arg(realObjectSeg.toUpper()).arg(animSeg.toUpper()).arg(idForObj).arg(oPlusA).arg(resourceName).arg(caseStatements.size());
                        caseStatements.push_back(caseStatement);
                    }

                    void AddCaseStatementForInv(QString invSeg, int idForInv,QString resourceName)
                    {
                        QString caseStatement = QString("case %9: return api.addImageForAnInventoryItem(lh, \"%1\",%2,res.%3);\n").arg(invSeg.toUpper()).arg(idForInv).arg(resourceName).arg(caseStatements.size());
                        caseStatements.push_back(caseStatement);
                    }

                    void writeSwingBundle(QTextStream& f, QString bundleJavaClassName, int start, int end)
                    {

                            f << ("package "+package+"." + maxFileSeg +"." + pixelSeg +";\n");
                            f << ("\n");
                            f << ("import com.github.a2g.bridge.ImageResource;\n");
                            f << ("import com.github.a2g.bridge.LoadHandler;\n");
                            f << ("import com.github.a2g.core.authoredscene.InternalAPI;\n");
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
                                QString relativePath = iter.value();
                                f << ("        public ImageResource "+resourceName+"{ return new ImageResource(\"" + relativePath + "\");}\n");
                            }
                            f << "    }"                                                                                "\n";

                            f << "    public static boolean addResources(LoadHandler lh, InternalAPI api, int i)"      "\n";
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

                            f << "     static int counter = 0;"                                                        "\n";
                            f << "     public static void setCounter(int i){ counter = i;}"                            "\n";
                            f << "     public static void extractAllNow(LoadHandler lh, InternalAPI api)"                            "\n";
                            f << "     {"                                                                              "\n";
                            f << "         setCounter(0);"                                                             "\n";
                            f << "         extractNext(lh, Integer.MAX_VALUE, api);"                                       "\n";
                            f << "     }"                                                                              "\n";
                            f << "     public static boolean extractNext(final LoadHandler lh, final int CHUNK, InternalAPI api)"          "\n";
                            f << "     {"                                                                              "\n";
                            f << "         boolean stillRemaining = true;"                                             "\n";
                            f << ""                                                                                    "\n";
                            f << "         for (int i=0;i<CHUNK && stillRemaining==true;i++)"                          "\n";
                            f << "         {"                                                                          "\n";
                            f << "             stillRemaining = addResources(lh, api,counter);"                            "\n";
                            f << "             counter++;"                                                             "\n";
                            f << "             if(stillRemaining==false)"                                              "\n";
                            f << "                 break;"                                                             "\n";
                            f << "         }"                                                                          "\n";
                            f << "         return stillRemaining;"                                                     "\n";
                            f << "     }"                                                                              "\n";
                            f << "}"                                                                                   "\n";

                    }

                    void writeGwtBundle(QTextStream& f, QString bundleJavaClassName, int start, int end)
                    {

                            f << ("package "+package+"." + maxFileSeg +"." + pixelSeg +";\n");
                            f << ("\n");
                            f << ("import com.github.a2g.bridge.GWT;\n");
                            f << ("import com.github.a2g.bridge.ClientBundle;\n");
                            f << ("import com.github.a2g.bridge.ImageResource;\n");
                            f << ("import com.github.a2g.bridge.LoadHandler;\n");
                            f << ("import com.github.a2g.core.authoredscene.InternalAPI;\n");
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
                                QString relativePath = iter.value();
                                QString resrc = ("        @Source(\"" + relativePath + "\")\n");
                                resrc = resrc + ("        abstract ImageResource "+resourceName+";\n");

                                f << resrc;
                            }
                            f << "    }"                                                                               "\n";
                            f << "    public static boolean addResources(LoadHandler lh, InternalAPI api, int i)"                     "\n";
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

                            f << "     static int counter = 0;"                                                        "\n";
                            f << "     public static void setCounter(int i){ counter = i;}"                            "\n";
                            f << "     public static void extractAllNow(LoadHandler lh, InternalAPI api)"                            "\n";
                            f << "     {"                                                                              "\n";
                            f << "         setCounter(0);"                                                             "\n";
                            f << "         extractNext(lh, Integer.MAX_VALUE, api);"                                       "\n";
                            f << "     }"                                                                              "\n";
                            f << "     public static boolean extractNext(final LoadHandler lh, final int CHUNK, InternalAPI api)"          "\n";
                            f << "     {"                                                                              "\n";
                            f << "         boolean stillRemaining = true;"                                             "\n";
                            f << ""                                                                                    "\n";
                            f << "         for (int i=0;i<CHUNK && stillRemaining==true;i++)"                          "\n";
                            f << "         {"                                                                          "\n";
                            f << "             stillRemaining = addResources(lh, api,counter);"                            "\n";
                            f << "             counter++;"                                                             "\n";
                            f << "             if(stillRemaining==false)"                                              "\n";
                            f << "                 break;"                                                             "\n";
                            f << "         }"                                                                          "\n";
                            f << "         return stillRemaining;"                                                     "\n";
                            f << "     }"                                                                              "\n";
                            f << "}"                                                                                   "\n";

                    }

                    bool writeGwtLoader(QTextStream& f, QString loaderJavaClassName, const std::vector<std::pair<int,QString> >&  list)
                    {
                        f << ("package "+package+"." + maxFileSeg +"." + pixelSeg +";\n");
                        f << ("\n");
                        f << ("import com.github.a2g.core.GWT;\n");
                        f << ("import com.github.a2g.core.RunAsyncCallback;\n");
                        f << ("import com.github.a2g.core.Scheduler;\n");
                        f << ("import com.github.a2g.core.LoadHandler;\n");
                        f << ("import com.github.a2g.core.loader.ImageBundleLoaderAPI;\n");
                        f << ("import com.github.a2g.core.authoredscene.InternalAPI;\n");


                        f << ("\n");
                        f << ("public class "+loaderJavaClassName+" implements ImageBundleLoaderAPI\n");
                        f << ("{\n");
                        f << ("  @Override\n");
                        f << QString("  public boolean isInventory(){ return %1;}\n").arg(IsInventory()? "true" : "false");
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
                        int counter = 0;
                        for(unsigned int i=0;i<list.size();i++)
                        {
                            f << QString("    case %1: \n").arg(i);
                            f << QString("GWT.runAsync\n");
                            f << QString("(\n");
                            f << QString("  new RunAsyncCallback()\n");
                            f << QString("  {\n");
                            f << QString("      public void onSuccess()\n");
                            f << QString("      {\n");
                            //f << QString("		  lh.onLoad(null);\n");
                            f << QString("        // Schedule the IncrementalCommand instance to run when\n");
                            f << QString("        // control returns to the event loop by returning 'true'\n");
                            f << QString("        Scheduler.get().scheduleFixedDelay\n");
                            f << QString("        (\n");
                            f << QString("          new Scheduler.RepeatingCommand()\n");
                            f << QString("          {\n");
                            f << QString("            int counter= %1;\n").arg(counter);
                            f << QString("            @Override\n");
                            f << QString("              public boolean execute()\n");
                            f << QString("              {\n");
                            f << QString(" %1.setCounter(counter);\n").arg(list[i].second);
                            f << QString(" boolean stillRemaining = %1.extractNext(lh, CHUNK, api);\n").arg(list[i].second);
                            f << QString("                counter+=CHUNK;\n");
                            //   f << QString("		          if(!stillRemaining){ lh.onLoad(null);}\n");
                            f << QString("                return stillRemaining;\n");
                            f << QString("              }\n");
                            f << QString("          }\n");
                            f << QString("          ,milliseconds\n");
                            f << QString("        );\n");
                            f << QString("      }\n");
                            f << QString("      public void onFailure(Throwable caught)\n");
                            f << QString("      {\n");
                            f << QString("		   lh.onLoad(null);\n");
                            f << QString("      }\n");
                            f << QString("    }\n");
                            f << QString(");\n");
                            f << QString("return 0;\n");
                            counter += list[i].first;
                        }
                        f << ("    }\n");
                        f << ("    return 0;\n");
                        f << ("  }\n");
                        f << ("}\n");
                        return true;
                    }

                    bool writeSwingLoader(QTextStream& f, QString loaderJavaClassName, const std::vector<std::pair<int,QString> >&  list)
                    {
                        f << ("package "+package+"." + maxFileSeg +"." + pixelSeg +";\n");
                        f << ("\n");
                        f << ("import javax.swing.Timer;\n");
                        f << ("import java.awt.event.ActionEvent;\n");
                        f << ("import java.awt.event.ActionListener;\n");
                        f << ("import com.github.a2g.bridge.LoadHandler;\n");
                        f << ("import com.github.a2g.core.loader.ImageBundleLoaderAPI;\n");
                        f << ("import com.github.a2g.core.authoredscene.InternalAPI;\n");


                        f << ("\n");
                        f << ("public class "+loaderJavaClassName+" implements ImageBundleLoaderAPI\n");
                        f << ("{\n");
                        f << ("  Timer timer;\n");
                        f << ("  @Override\n");
                        f << QString("  public boolean isInventory(){ return %1;}\n").arg(IsInventory()? "true" : "false");
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
                        int counter = 0;
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
                            f << QString("            %1.setCounter(counter);\n").arg(list[i].second);
                            f << QString("            boolean stillRemaining = %1.extractNext(lh, CHUNK, api);\n").arg(list[i].second);
                            f << QString("            counter+=CHUNK;\n");
                            f << QString("            if(!stillRemaining)\n");
                            f << QString("               timer.stop();\n");
                            f << QString("          }\n");
                            f << QString("        }\n");
                            f << QString("      );\n");
                            f << QString("      timer.setInitialDelay(milliseconds);\n");
                            f << QString("      timer.start();\n");
                            f << QString("      return 0;\n");
                            counter += list[i].first;
                        }
                        f << ("    }\n");
                        f << ("    return 0;\n");
                        f << ("  }\n");
                        f << ("}\n");
                        return true;
                    }

                    bool writeToFile(QString find, QString replace)
                    {
                        CropImagesAndConstructDeclarations(find, replace);

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
                            //QString prefix = theJavaClassNamePrefix==FIRST
                            QString bundleJavaClassName = theJavaClassNamePrefix + names[i];
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

