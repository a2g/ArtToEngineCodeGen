#pragma once

#include <QDir>
#include <QFile>
#include <QImage>
#include <QString>
#include <QMessageBox>



namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {



                void makePath(QString fullPathToSaveTo)
                {
                    QDir dir;
                    bool isOK = dir.mkpath(fullPathToSaveTo);
                    if(!isOK)
                    {
                        QDir dd(fullPathToSaveTo);
                        if(!QFile::exists(dd.path()))
                            QMessageBox::warning(nullptr,"Dir of this file can't be created",fullPathToSaveTo);
                    }

                    QDir dir2(fullPathToSaveTo);
                    if(dir2.exists())
                    {
                        bool isOK2 = dir2.rmdir(fullPathToSaveTo);
                        if(!isOK2)
                        {
                            QMessageBox::warning(nullptr," couldnt remove",fullPathToSaveTo);
                        }
                    }
                }

            }
        }
    }
}

