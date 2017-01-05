#pragma once
#include <exception>

/*
This num is purely for FolderTraverser::generateFilesFromSourceFolderOrASubFolderThereof(QString path).

If the path parameter that is passed in does not contain a segment called "source" then this is returned.
*/
enum FolderTraverserResult
{
    IsOk=1
    ,SrcIsNotFoundInPathParameter
    ,SourceImagesIsNotFoundInPathParameter
};
