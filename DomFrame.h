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
#include <string>
//#include <QRect>
#include <qrect.h>
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class DomFrame
                {
                    std::string pngSaveTo;
                    QRect rect;
                public:
                    DomFrame() {}
                    DomFrame(const std::string& pngSaveTo)
                        :  pngSaveTo(pngSaveTo)
                    {

                    }

                    DomFrame(const DomFrame& f)
                    {
                        this->rect = f.rect;
                        this->pngSaveTo = f.pngSaveTo;
                    }
                    DomFrame& operator=(const DomFrame& f)
                    {
                        this->rect = f.rect;
                        this->pngSaveTo = f.pngSaveTo;
                        return *this;
                    }

                    const char* getPngSaveTo() const { 
                        return pngSaveTo.c_str(); }
                    QRect getRect() const { 
                        return rect; }
                    void setRect(const QRect& rect) { 
                        this->rect = rect; }
                    void setPngPath(const std::string& path) { 
                        this->pngSaveTo = path; }
 
                };
            }
        }
    }
}

