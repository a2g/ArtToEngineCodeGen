/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#pragma once





#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMap>
#include <QString>
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class AFile 
                {
                    mutable QString m_currentPath;
                    mutable QString m_animFolder;
                    mutable QString package;
                    QMap<QString,int> mapOfAnims;
                public:
                    AFile(){}
                    AFile(const QString& currentPath, const QString& animFolder, const QString& package) 
                            : m_currentPath(currentPath)
                            , m_animFolder(animFolder)
                            , package(package)
                    {
                    }
                    int getNumberOfEntries()
                    {
                        int number = mapOfAnims.size();
                        return number;
                    }
                    void insert(std::string objectPlusAnim,  int idForObjPlusAnim)
                    {
                        mapOfAnims.insert(QString::fromStdString(objectPlusAnim), idForObjPlusAnim);
                    }
                    ~AFile()
                    {
                        // writeToFile();
                    }
                private:
                    bool writeToFile(QString find, QString replace)
                    {
                        m_currentPath = m_currentPath.replace(find, replace);
                        package = package.replace(find,replace);

                        QDir dir;
                        dir.mkpath(m_currentPath+"/" + m_animFolder);
                        QFile file(m_currentPath+"/" + m_animFolder + "/a.java");
                        if (!file.open(QFile::WriteOnly | QFile::Truncate)) 
                            return false;

                        QTextStream f(&file);
                        f << ("package " + package + "._00_Animations;\n");
                        f << ("\n");
                        f << ("public class a\n");
                        f << ("{\n");

                        for(QMap<QString,int>::iterator iter = mapOfAnims.begin();iter!=mapOfAnims.end();iter++)
                        {
                            f << ("  public static final int ") <<  (iter.key().toUpper()) << " = " << (iter.value()) << (";\n");
                        }							
                        f << ("}\n");

                        file.close();
                        return true;
                    }

                    friend class OutputFiles;


                };
            }
        }
    }
}








