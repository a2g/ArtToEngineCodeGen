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

#include "LoaderAndResFilePair.h"
#include "IsInventory.h"
#include "GetWidthAndHeightFromImageName.h"


com::github::a2g::generator::LoaderAndResFilePair::LoaderAndResFilePair(){}
com::github::a2g::generator::LoaderAndResFilePair::LoaderAndResFilePair(const QString package, const QString psdFileSeg, const QString& sceneFolder,  QString animFolder)
        : package(package)
        , sceneFolder(sceneFolder)
        , animFolder(animFolder)
        , isGwt(false)

{



}


com::github::a2g::generator::LoaderAndResFilePair::~LoaderAndResFilePair()
{
    //  writeToFile();
}

void com::github::a2g::generator::LoaderAndResFilePair::setJavaClassNamePrefix(QString theJavaClassNamePrefix)
{
    this->theJavaClassNamePrefix = theJavaClassNamePrefix;
}

QString com::github::a2g::generator::LoaderAndResFilePair::getJavaClassNamePrefix()
{
    return theJavaClassNamePrefix;
}

QString com::github::a2g::generator::LoaderAndResFilePair::getJavaClassName()
{
    return javaClassName;
}

QString com::github::a2g::generator::LoaderAndResFilePair::makePath(QString fullOfSlashes)
{
    fullOfSlashes.replace('.','/');
    return fullOfSlashes;
}

void com::github::a2g::generator::LoaderAndResFilePair::addAnimImage(QString pngPath, int idForObj)
{
    animImages.push_back(QPair<QString,int>(pngPath,idForObj));
    if(lineThatSetsResolution.isEmpty())
        lineThatSetsResolution = buildLineThatSetsResolution(pngPath, false);
}

void com::github::a2g::generator::LoaderAndResFilePair::addInvImage(QString pngPath, int idForInv)
{
    invImages.push_back(QPair<QString,int>(pngPath,idForInv));
     if(lineThatSetsResolution.isEmpty())
        lineThatSetsResolution = buildLineThatSetsResolution(pngPath, true);
}

QString com::github::a2g::generator::LoaderAndResFilePair::buildLineThatSetsResolution(QString pngPath, bool isInventory)
{

    QPoint p = generator::getWidthAndHeightFromImageName(pngPath);
    if(isInventory)
    {
        lineThatSetsResolution = QString("    api.setInventoryImageSize(%1, %2);\n").arg(p.x()).arg(p.y());
    }
    else
    {
        lineThatSetsResolution = QString("    api.setScenePixelSize(%1, %2);\n").arg(p.x()).arg(p.y());
    }
    return lineThatSetsResolution;
}

void com::github::a2g::generator::LoaderAndResFilePair::cropImagesAndConstructDeclarations(QString find, QString replace)
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
        QString psdFileSeg = list[i-4];
        QString pixelSeg   = list[i-3];
        QString objectSeg  = list[i-2];
        QString animSeg    = list[i-1];
        QString pngPath    = pair.first;
        int idForObject    = pair.second;


        QString pngByItself = "cropped_" + QDir(pngPath).dirName();
        QString pngSaveTo = animFolder +"/" + pngByItself;
        pngSaveTo = pngSaveTo.replace(find,replace);
        // do the cropping


        QRect rect = crop(pngPath, pngSaveTo, Qt::red);

        // add case and resource
        QString objectPlusAnim = getObjectPlusAnim(objectSeg, animSeg);
        QString resourceName = QString("%1__%2_%3()").arg(getRealObjectSeg(objectSeg, isInventory())).arg(animSeg).arg(caseTally++);
        resourcePairs.push_back(QPair<QString,QString>(resourceName, pngSaveTo));

        addCaseStatementForAnim(objectSeg, rect, getRealObjectSeg(objectSeg, isInventory()), animSeg, idForObject, objectPlusAnim, resourceName);
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
        QString psdFileSeg = list[i-3];
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
        resourcePairs.push_back(QPair<QString,QString>(resourceName, pngPath));

        addCaseStatementForInv(objectSeg, idForObject, resourceName);
    }
}

bool com::github::a2g::generator::LoaderAndResFilePair::isEmpty()
{
    bool isEmpty = animImages.size()==0 && invImages.size()==0;
    return isEmpty;
}

bool com::github::a2g::generator::LoaderAndResFilePair::isNeeded()
{
    bool isNeeded = (psdFileSeg.toLower().contains("_objects")|| javaClassName!=FIRST);
    return isNeeded;
}

bool com::github::a2g::generator::LoaderAndResFilePair::isInventory()
{
    bool isInventory = com::github::a2g::generator::IsInventory(psdFileSeg);
    return isInventory;
}

void com::github::a2g::generator::LoaderAndResFilePair::addCaseStatementForAnim(QString objectSeg, QRect offset, QString realObjectSeg, QString animSeg, int idForObj, QString objectPlusAnim, QString resourceName)
{
    int prefix = objectSeg.mid(1,2).toInt();

    int x = offset.x();
    int y = offset.y();
    int w = offset.width();
    int h = offset.height();
    QString oPlusA = objectPlusAnim.toUpper();

    QString start = QString(IS_SWITCH? "        case %1:" : "else if(i==%1)").arg(caseStatements.size());
    QString caseStatement = start + QString(" return api.addImageForASceneObject(lh, %1,%2,%3,%4,%5, \"%6\",\"%7\",(short)%8,\"%9\", new PackagedImageFor%10(res.%11));\n").arg(prefix).arg(x).arg(y).arg(w).arg(h).arg(realObjectSeg.toUpper()).arg(animSeg.toUpper()).arg(idForObj).arg(oPlusA).arg(isGwt? "Html4" : "Java").arg(resourceName);
    caseStatements.push_back(caseStatement);

    if(firstResource.length()==0)
        firstResource=resourceName;
}

void com::github::a2g::generator::LoaderAndResFilePair::addCaseStatementForInv(QString invSeg, int idForInv,QString resourceName)
{
    QString start = QString(IS_SWITCH? "        case %1:" : "else if(i==%1)").arg(caseStatements.size());

    QString caseStatement = start + QString(" return api.addImageForAnInventoryItem(lh, \"%1\",%2,new PackagedImageFor%3(res.%4));\n").arg(invSeg.toUpper()).arg(idForInv).arg(isGwt? "Html4" : "Java").arg(resourceName);
    caseStatements.push_back(caseStatement);

    if(firstResource.length()==0)
        firstResource=resourceName;
}

void com::github::a2g::generator::LoaderAndResFilePair::removeAllButOne()
{

}
void com::github::a2g::generator::LoaderAndResFilePair::writeGwtBundle(QTextStream& f, QString bundleJavaClassName, int start, int end, bool isDummyRun)
{
    if(isDummyRun)
    {
        end= start + (end-start)/2;
    }
    QStringList packageSegs = package.split(".");

    f << ("package "+package+"." + psdFileSeg + ";\n");
    f << ("\n");

    f << ("import com.github.a2g.core.platforms.html4.PackagedImageForHtml4;\n");
    f << ("import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;\n");
    f << ("import com.google.gwt.resources.client.ClientBundle;\n");
    f << ("import com.google.gwt.event.dom.client.LoadHandler;\n");
    f << ("import com.google.gwt.core.client.GWT;\n");
    f << ("import com.google.gwt.resources.client.ImageResource;\n");

    f << ("\n");
    f << ("public class "+bundleJavaClassName+"\n");
    f << ("{\n");
    f << ("    public interface MyRes extends ClientBundle");
    f << ("    {\n");
    f << ("        public static final "+bundleJavaClassName+".MyRes RESOURCE =  GWT.create("+bundleJavaClassName+".MyRes.class);\n");
    f << ("        \n");
    for(int i=start;i<end;i++)
    {
        QString resourceName = resourcePairs[i].first;
        QString fullPath = resourcePairs[i].second;
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
    f << QString("    public static boolean addImage(IMasterPresenterFromBundle api, LoadHandler lh, int i)\n");
    f << "    {"                                                                               "\n";
    f << "        final MyRes res = MyRes.RESOURCE;"                                           "\n";
    f << (IS_SWITCH? "        switch(i){\n" : "        if(i==-1){}\n");

    for(int j=start;j<end;j++)
    {
        f << caseStatements[j];
    }
    f << (IS_SWITCH? "        }"    "\n" : "\n");

    f << "        return false;"                                                               "\n";
    f << "    }"                                                                               "\n";
    f << "}"                                                                                   "\n";

}

bool com::github::a2g::generator::LoaderAndResFilePair::writeGwtLoader(QTextStream& f, QString loaderJavaClassName, const std::vector<std::pair<int,QString> >&  list)
{
    f << ("package "+package+"." + psdFileSeg +"." +";\n");
    f << ("\n");
    f << ("import com.github.a2g.core.interfaces.internal.ILoad;\n");
    f << ("import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;\n");
    f << ("import com.google.gwt.event.dom.client.LoadHandler;\n");
    f << ("import com.google.gwt.core.client.GWT;\n");
    f << ("import com.google.gwt.core.client.RunAsyncCallback;\n");
    f << ("import com.google.gwt.core.client.Scheduler;\n");


    f << ("\n");
    f << ("public class "+loaderJavaClassName+" implements ILoad\n");
    f << ("{\n");
    f << ("  @Override\n");
    f << QString("  public boolean isInventory(){ return %1;}\n").arg(isInventory()? "true" : "false");
    f << ("  @Override\n");
    f << QString("  public int getNumberOfBundles(){ return %1;}\n").arg(list.size());
    f << ("  \n");
    f << ("  @Override\n");
    f << ("  public int getNumberOfImagesInBundle(int bundleNumber)\n");
    f << ("  {\n");
    f << (IS_SWITCH? "    switch(bundleNumber)\n{\n" : "    if(bundleNumber==-1){}\n");


    for(unsigned int i=0;i<list.size();i++)
    {
        if(IS_SWITCH)
        {
            f << QString("    case %1: return %2; \n").arg(i).arg(list[i].first);
        }else
        {
            f << QString("    else if(bundleNumber== %1){ return %2;} \n").arg(i).arg(list[i].first);
        }
    }
    if(IS_SWITCH)
    {
        f << ("    }\n");
    }
    f << ("    return 0;\n");
    f << ("  }\n");
    f << ("  @Override\n");
    f << ("  public int loadImageBundle(final LoadHandler lh, final IMasterPresenterFromBundle api, final int bundleNumber, final int CHUNK, final int milliseconds)\n");
    f << ("  {\n");
    f << lineThatSetsResolution;

    f << (IS_SWITCH? "    switch(bundleNumber)\n{\n" : "    if(bundleNumber==-1){}\n");
    int offset = 0;
    for(unsigned int i=0;i<list.size();i++)
    {
        if(IS_SWITCH)
        {
            f << QString("    case %1:{ \n").arg(i);
        }
        else
        {
            f << QString("    else if(bundleNumber== %1){ \n").arg(i);
        }
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
        f << ("}\n"); // end case or elseif or whatever it is
        offset += list[i].first;
    }
    if(IS_SWITCH)
    {
        f << ("}\n"); // end switch
    }
    f << ("return 0;\n");
    f << ("}\n");// end function
    f << ("}\n");// end class
    return true;
}


void com::github::a2g::generator::LoaderAndResFilePair::writeSwingBundle(QTextStream& f, QString bundleJavaClassName, int start, int end, bool isDummyRun)
{
    if(isDummyRun)
    {
        end= start + (end-start)/2;
    }
    f << ("package "+package+"." + psdFileSeg +";\n");
    f << ("\n");
    f << ("import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;\n");
    f << ("import com.github.a2g.core.platforms.java.PackagedImageForJava;\n");
    f << ("import com.google.gwt.event.dom.client.LoadHandler;\n");
    f << ("\n");
    f << ("public class "+bundleJavaClassName+"\n");
    f << ("{\n");
    f << ("    public static class MyRes");
    f << ("    {\n");
    f << ("        public static final MyRes RESOURCE = new MyRes();");
    f << ("        \n");

    for(int i=start;i<end;i++)
    {
        QString resourceName = resourcePairs.at(i).first;
        QString fullPath = resourcePairs.at(i).second;

        f << ("        public String "+resourceName+"{ return \"" + fullPath + "\";}\n");
    }
    f << "    }"                                                                                "\n";

    f << "    public static boolean addImage(IMasterPresenterFromBundle api, LoadHandler lh, int i)"      "\n";
    f << "    {"                                                                               "\n";
    f << "        final MyRes res = MyRes.RESOURCE;"                                           "\n";
    f << (IS_SWITCH?"        switch(i){\n": "if(i==-1){}\n");
    for(int j=start;j<end;j++)
    {
        f << caseStatements[j];
    }
    f << (IS_SWITCH? ("        }"    "\n") : ("\n"));                                                                         "\n";
    f << "        return false;"                                                               "\n";
    f << "    }"                                                                               "\n";
    f << "}"                                                                                   "\n";

}

bool com::github::a2g::generator::LoaderAndResFilePair::writeSwingLoader(QTextStream& f, QString loaderJavaClassName, const std::vector<std::pair<int,QString> >&  list)
{
    f << ("package "+package+"." + psdFileSeg +";\n");
    f << ("\n");
    f << ("import javax.swing.Timer;\n");
    f << ("import java.awt.event.ActionEvent;\n");
    f << ("import java.awt.event.ActionListener;\n");
    f << ("import com.google.gwt.event.dom.client.LoadHandler;\n");
    f << ("import com.github.a2g.core.interfaces.internal.ILoad;\n");
    f << ("import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;\n");


    f << ("\n");
    f << ("public class "+loaderJavaClassName+" implements ILoad\n");
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
    f << ("  public int loadImageBundle(final LoadHandler lh, final IMasterPresenterFromBundle api, final int bundleNumber, final int CHUNK, final int milliseconds)\n");
    f << ("  {\n");
    f << lineThatSetsResolution;
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
        f << QString("            lh.onLoad(null);\n");
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

bool com::github::a2g::generator::LoaderAndResFilePair::writeToFile(QString find, QString replace, bool isGwt, bool isDummyRun)
{
    this->isGwt=isGwt;
    cropImagesAndConstructDeclarations(find, replace);

    if(caseStatements.size()==0)
        return false;

    package = package.replace(find,replace);
    sceneFolder =sceneFolder.replace(find,replace);
    std::vector<std::pair<int,QString> > list;
    int total = resourcePairs.size();
    double numberOfBundles = 10.0;
    int upperLimitOnSingleBundleSpecialCase = 80;
    double fincrement = (total*1.0)/numberOfBundles;
    double fend = fincrement;
    int start = 0;
    int end = static_cast<int>(fend);
    if(total < upperLimitOnSingleBundleSpecialCase)
    {
        numberOfBundles = 1;
        end = total;
    }
    const char* names[10]={"0","1","2","3","4","5","6","7","8","9"};
    for(int i=0;i<numberOfBundles;i++)
    {
        QString bundleJavaClassName = theJavaClassNamePrefix + names[i] + "Bundle";
        QDir dir;
        dir.mkpath(sceneFolder);
        QFile file(sceneFolder+"/" + bundleJavaClassName + ".java");
        if (!file.open(QFile::WriteOnly | QFile::Truncate))
            return false;
        QTextStream f(&file);
        if(isGwt)
        {
            writeGwtBundle(f, bundleJavaClassName, start, end, isDummyRun);
        }
        else
        {
            writeSwingBundle(f, bundleJavaClassName, start, end, isDummyRun);
        }
        file.close();
        QString longName = package+"." + psdFileSeg +"."+bundleJavaClassName;
        list.push_back(std::pair<int,QString>(end-start, longName));
        fend = fend + fincrement;
        start = end;
        end = static_cast<int>(fend);
    }

    // now to write the loader
    QString loaderJavaClassName = theJavaClassNamePrefix + "Loader";
    QDir dir;
    dir.mkpath(sceneFolder);
    QFile loader( sceneFolder+"/"+loaderJavaClassName + ".java");
    if (!loader.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QTextStream f(&loader);
    bool result = isGwt? writeGwtLoader(f, loaderJavaClassName, list) : writeSwingLoader(f, loaderJavaClassName, list);
    (result=!!result);
    loader.close();

    return true;
}
